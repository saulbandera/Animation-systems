#include "Bone.h"
#include <maths/math_utils.h>
#include <iostream>

Bone::Bone() :
	length(0),
	xy(0.f,0.f),
	rotation(0.f),
    local_transform_(gef::Matrix33::kIdentity),
    world_transform_(gef::Matrix33::kIdentity)
{
}

std::map<std::string, Bone> Bone::ReadBoneDataFromJSON(rapidjson::Document& ske_doc)
{

	std::map<std::string, Bone> bones;
	const rapidjson::Value& bone_list = ske_doc["armature"][0]["bone"];

	for (int i = 0; i < static_cast<int>(bone_list.Size()); i++)
	{
		Bone* bone = new Bone();
		bone->name = bone_list[i]["name"].GetString();

		if (bone_list[i].HasMember("parent"))
			bone->parent = bone_list[i]["parent"].GetString();
		else
			bone->parent = "no_parent";

		if (bone_list[i].HasMember("transform"))
		{
			if (bone_list[i]["transform"].HasMember("x"))
				bone->xy.x = bone_list[i]["transform"]["x"].GetFloat();

			if (bone_list[i]["transform"].HasMember("y"))
				bone->xy.y = bone_list[i]["transform"]["y"].GetFloat();

			if (bone_list[i]["transform"].HasMember("skY"))
				bone->rotation = bone_list[i]["transform"]["skY"].GetFloat();
		}

		bone->BuildLocalBoneTransform(nullptr);

		bones[bone->name] = *bone;
		delete bone;
	}

	BuildWorldBoneTransforms(&bones);

	return bones;
}

void Bone::BuildLocalBoneTransform(AnimationData2D* anim_data)
{
	float new_x = xy.x;
	float new_y = xy.y;
	float new_rotation = rotation;

	gef::Matrix33 translation_m = gef::Matrix33::kIdentity;
	gef::Matrix33 rotation_m = gef::Matrix33::kIdentity;

	if (anim_data != nullptr)
	{
		// loop through all the bone keys in the animation
		for (auto& bone_key : anim_data->GetCurrentAnimation()->bone_keys)
		{

			// check that the bone name matches the bone key name
			if (bone_key.name == name)
			{
				
				// adding the interpolated position from the animation keys to the local bone position
				for (auto translation_key = bone_key.translation_keys.begin(); translation_key != bone_key.translation_keys.end(); ++translation_key)
				{
					// loops through all the translation keys 
					if (std::next(translation_key) != bone_key.translation_keys.end())
					{
						// obtain the next translation key
						const TranslationKey next_translation_key = *std::next(translation_key);

						if (anim_data->GetCurrentFrame() >= translation_key->start_time && anim_data->GetCurrentFrame() < next_translation_key.start_time)
						{

							translation_key->current_frame++;
							// interpolate the translation between the current translation key and the next translation key
							new_x += gef::Lerp(translation_key->x, next_translation_key.x, translation_key->time_to_next_key);
							new_y += gef::Lerp(translation_key->y, next_translation_key.y, translation_key->time_to_next_key);
							// time is moved closer to next translation key
							translation_key->time_to_next_key = static_cast<float>(translation_key->current_frame) / static_cast<float>(translation_key->duration);

							if (translation_key->time_to_next_key >= 1.f)
							{
								translation_key->time_to_next_key = 0.f;
								translation_key->current_frame = 0;
							}

							// avoid looping through all the translation keys once the current frame is on the next translation key
							break;
						}
					}
					else
					{
						new_x += translation_key->x;
						new_y += translation_key->y;
					}
				}
				// adding the interpolated rotation from the animation keys to the local bone rotation
				for (auto rotation_key = bone_key.rotation_keys.begin(); rotation_key != bone_key.rotation_keys.end(); ++rotation_key)
				{
					// loops through all the rotation keys 
					if (std::next(rotation_key) != bone_key.rotation_keys.end())
					{
						// obtain the next rotation key
						const RotationKey next_rotation_key = *std::next(rotation_key);


						if (anim_data->GetCurrentFrame() >= rotation_key->start_time && anim_data->GetCurrentFrame() < next_rotation_key.start_time)
						{
				
							rotation_key->current_frame++;
							// interpolate rotation between the current rotation key and the next rotation key
							new_rotation += LerpAngle(rotation_key->rotation, next_rotation_key.rotation, rotation_key->time_to_next_key);
							// time is moved closer to next rotation key
							rotation_key->time_to_next_key = static_cast<float>(rotation_key->current_frame) / static_cast<float>(rotation_key->duration);
							if (rotation_key->time_to_next_key >= 1)
							{
								rotation_key->time_to_next_key = 0.f;
								rotation_key->current_frame = 0;
							}
							// avoid looping through all the rotation keys once the current frame is on the next rotation key
							break;
						}
					}
					else
					{
						new_rotation += rotation_key->rotation;
					}
				}
			}
		}
	}

	translation_m.SetTranslation(gef::Vector2(new_x, new_y));
	rotation_m.Rotate(gef::DegToRad(new_rotation));

	local_transform_ = rotation_m * translation_m;
}

void Bone::BuildWorldBoneTransforms(std::map<std::string, Bone>* bones)
{
	// iterate through all the bones starting at the root bone and calculate the
	// world bone transform for each bone. 
	for (std::pair<std::string, Bone> bone : *bones)
	{
		if (bone.first != "root")
		{
			std::string parent_name;
			gef::Matrix33 transform = gef::Matrix33::kIdentity;

			(*bones)[bone.first].world_transform_.SetIdentity();

			transform = bone.second.local_transform_;
			parent_name = bone.second.parent;

			while (parent_name != "root")
			{
				transform = transform * (*bones)[parent_name].local_transform_;
				parent_name = (*bones)[parent_name].parent;
			}

			(*bones)[bone.first].world_transform_ = transform;
		}
	}
}


float Bone::LerpAngle(const float start_angle, const float end_angle, const float time)
{

	float angle_difference = end_angle - start_angle;

	if(angle_difference > 180.0f)
	{
		angle_difference -= 360.0f;
	}
	else if(angle_difference <- 180.0f)
	{
		angle_difference += 360;
	}

	const float angle = start_angle + time * angle_difference;

	return angle;

}

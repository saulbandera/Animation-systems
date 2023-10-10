#include "AnimationData2D.h"
#include "load_json.h"
#include "Bone.h"
#include <iostream>

AnimationData2D::AnimationData2D() :
	to_next_frame_(true),
	frame_rate_(0),
	current_frame_(0),
	elapsed_time_(0.f)
	
{
}

void AnimationData2D::Init(const char* ske_doc)
{

	// load JSON skeleton file containing animation data
	const char* json_ske = LoadJSON(ske_doc);

	rapidjson::Document ske_document;
	ske_document.Parse(json_ske);
	// read the loaded and parsed animation data into a map of animations
	ReadAnimationDataFromJSON(ske_document);

	current_frame_ = 0;
	elapsed_time_ = 0.f;
	to_next_frame_ = true;
}

void AnimationData2D::Update(float frame_time)
{


	// increase elapsed time
	elapsed_time_ += frame_time;
	
	if (elapsed_time_ >= 1.f / static_cast<float>(frame_rate_))
	{
		to_next_frame_ = true;
		// move to next frame in animation
		current_frame_++;
		elapsed_time_ = 0;

		if (current_frame_ >= animations_[current_animation_].duration)
		{
			// loop animation to beginning
			current_frame_ = 0;
		}

		if (animations_[current_animation_].bone_keys.empty())
		{
			// if bone keys is empty it means the animation is frame based, the sprite is animated by setting the current frame
			current_frame_name_ = animations_[current_animation_].frames_list[current_frame_];
		}
	} 
}



void AnimationData2D::ReadAnimationDataFromJSON(rapidjson::Document& ske_document)
{

	animations_.clear(); 
	animation_names_.clear();

	frame_rate_ = ske_document["armature"][0]["frameRate"].GetInt();
	animation_type_ = ske_document["armature"][0]["type"].GetString();

	if (animation_type_ == "Armature")
	{
		rapidjson::Value& animation_list = ske_document["armature"][0]["animation"];

		// read the animation array
		for (int i = 0; i < static_cast<int>(animation_list.Size()); i++)
		{
			// create new animation
			auto* anim = new Animation();
			
			anim->name = animation_list[i]["name"].GetString();
			anim->duration = animation_list[i]["duration"].GetInt();

			rapidjson::Value& bone_keys = animation_list[i]["bone"];

			// iterate through the bone keys 
			for (int j = 0; j < static_cast<int>(bone_keys.Size()); j++)
			{
				auto* bone_key = new BoneKey;
				bone_key->name = bone_keys[j]["name"].GetString();

				// check whether there is translation key data
				if (bone_keys[j].HasMember("translateFrame"))
				{
					int start_time_inc = 0;

					rapidjson::Value& translate_keys = bone_keys[j]["translateFrame"];

					// for each translateFrame store the translation key data
					for (int k = 0; k < static_cast<int>(translate_keys.Size()); k++)
					{
						auto* translation_key = new TranslationKey;

						translation_key->current_frame = 0;
						translation_key->time_to_next_key = 0.f;
						translation_key->duration = translate_keys[k]["duration"].GetInt();

						translation_key->start_time = start_time_inc;
						start_time_inc += translation_key->duration;

						if (translate_keys[k].HasMember("x"))
							translation_key->x = translate_keys[k]["x"].GetFloat();
						else
							translation_key->x = 0.f;
						if (translate_keys[k].HasMember("y"))
							translation_key->y = translate_keys[k]["y"].GetFloat();
						else
							translation_key->y = 0.f;

						bone_key->translation_keys.push_back(*translation_key);
						delete translation_key;
					}
				}

				// check whether there is rotation key data

				if (bone_keys[j].HasMember("rotateFrame"))
				{
					int start_time_inc = 0;
					// for each rotateFrame store the rotation key data
					rapidjson::Value& rotate_keys = bone_keys[j]["rotateFrame"];
					for (int k = 0; k < static_cast<int>(rotate_keys.Size()); k++)
					{
						auto* rotation_key = new RotationKey;

						rotation_key->current_frame = 0;
						rotation_key->time_to_next_key = 0.f;
						rotation_key->duration = rotate_keys[k]["duration"].GetInt();

						rotation_key->start_time = start_time_inc;
						start_time_inc += rotation_key->duration;

						if (rotate_keys[k].HasMember("rotate"))
							rotation_key->rotation = rotate_keys[k]["rotate"].GetFloat();
						else
							rotation_key->rotation = 0.f;

						bone_key->rotation_keys.push_back(*rotation_key);
						delete rotation_key;
						
					}
				}

				anim->bone_keys.push_back(*bone_key);
				delete bone_key;
			}

			animations_[anim->name] = (*anim);
			animation_names_.push_back(anim->name);
			delete anim;
		}
	}

	// if the animation is frame based, populate the list of frames
	else if(animation_type_ == "Sheet")
	{
		const rapidjson::Value& anims = ske_document["armature"];

		for (int i = 0; i < static_cast<int>(anims.Size()); i++)
		{
			auto* anim = new Animation;

			anim->name = anims[i]["animation"][0]["name"].GetString();
			anim->duration = anims[i]["animation"][0]["duration"].GetInt();

			const rapidjson::Value& frames = ske_document["armature"][i]["skin"][0]["slot"][0]["display"];
			for (int j = 0; j < static_cast<int>(frames.Size()); j++)
			{
				std::string frame_name = frames[j]["name"].GetString();
				anim->frames_list.push_back(frame_name);
			}

			animations_[anim->name] = *anim;
			animation_names_.push_back(anim->name);
			delete anim;
		}
	}

	
	current_animation_ = ske_document["armature"][0]["defaultActions"][0]["gotoAndPlay"].GetString();
}


std::string AnimationData2D::GetCurrentFrameName()
{
	return current_frame_name_;
}

int AnimationData2D::GetCurrentFrame() const
{
	return current_frame_;
}

std::string AnimationData2D::GetAnimationType()
{
	return animation_type_;
}

std::string AnimationData2D::GetAnimationName()
{
	return animations_[current_animation_].name;
}

Animation* AnimationData2D::GetCurrentAnimation()
{
	return &animations_[current_animation_];
}



void AnimationData2D::SwitchAnimation(const std::string& anim_name)
{

	// reset the translation and rotation keys for all bones of the current animation
	for (auto& bone_key : animations_[current_animation_].bone_keys)
	{
		for (auto& translation_key : bone_key.translation_keys)
		{
			translation_key.current_frame = 0;
			translation_key.time_to_next_key = 0.f;
		}

		for (auto& rotation_key : bone_key.rotation_keys)
		{
			rotation_key.current_frame = 0;
			rotation_key.time_to_next_key = 0.f;
		}
	}


	current_animation_ = anim_name;

	// reset the current frame to start of animation
	current_frame_ = 0;
	elapsed_time_ = 0.f;
	to_next_frame_ = true;


}

void AnimationData2D::SetChangingToNextFrame(bool to_next_frame)
{
	to_next_frame_ = to_next_frame;
}

bool AnimationData2D::IsChangingToNextFrame() const
{
	return to_next_frame_;
}

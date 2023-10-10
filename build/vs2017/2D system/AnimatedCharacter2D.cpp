#include "AnimatedCharacter2D.h"
#include "input/keyboard.h"
#include "input/input_manager.h"
#include "graphics/sprite_renderer.h"
#include "load_json.h"
#include "load_texture.h"
#include <iostream>

AnimatedCharacter2D::AnimatedCharacter2D():
tex_json("boy-anim_tex.json"),
ske_json("boy-anim_ske.json"),
skeleton_anim_(false)
{
}

void AnimatedCharacter2D::Init(gef::Platform& platform)
{

	animation_data_ = new AnimationData2D();
	animation_data_->Init(ske_json);

	texture_atlas_ = new TextureAtlas();


	// load json file containing the texture atlas metadata
	const char* json_tex = LoadJSON(tex_json);

	// parse and read the file into the texture atlas instance
	rapidjson::Document tex_document;
	tex_document.Parse(json_tex);
	texture_atlas_->ReadTextureAtlasFromJSON(tex_document);

	// load and parse the skeleton json file
	const char* json_ske = LoadJSON(ske_json);

	rapidjson::Document ske_document;
	ske_document.Parse(json_ske);

	// read the skeleton file to store the order of the parts, the skin slots and the bones data 
	parts_order_ = ReadPartsOrderFromJSON(ske_document);
	skin_slots_ = SkinSlot::ReadSkinSlotDataFromJSON(ske_document);
	bones_ = Bone::ReadBoneDataFromJSON(ske_document);

	// set the position of the sprite to the centre of the screen
	sprite_position_ = gef::Vector2(static_cast<float>(platform.width()) * 0.5f, static_cast<float>(platform.height()) * 0.5f);
	sprite_scale_ = 0.5f;

	// obtain the first sprite texture
	sprite_texture_ = CreateTextureFromPNG(texture_atlas_->imagePath.c_str(), platform);
	sprite_.set_texture(sprite_texture_);


}

void AnimatedCharacter2D::Update(const float frame_time)
{


	animation_data_->Update(frame_time);

	UpdateBoneTransforms();

}

void AnimatedCharacter2D::HandleInput(const gef::InputManager* input_manager, gef::Platform& platform)
{
	if (input_manager)
	{
		// keyboard input
		const gef::Keyboard* keyboard = input_manager->keyboard();
		if (keyboard)

		{

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_1))
			{
				if(skeleton_anim_)
					animation_data_->SwitchAnimation("stand");
				else
				animation_data_->SwitchAnimation("boy-fall");
			}

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_2))
			{
				if (skeleton_anim_)
					animation_data_->SwitchAnimation("walk");
				else
				animation_data_->SwitchAnimation("boy-move");

			}
			if(keyboard->IsKeyPressed(gef::Keyboard::KC_3))
			{
				if (skeleton_anim_)
					animation_data_->SwitchAnimation("jump");
				else
				animation_data_->SwitchAnimation("boy-stand");
			}

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_4))
			{
				if (skeleton_anim_)
					animation_data_->SwitchAnimation("fall");
				else
				animation_data_->SwitchAnimation("boy-win");
			}

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_5) && !skeleton_anim_)
			{
				animation_data_->SwitchAnimation("boy-attack");
			}


			if (keyboard->IsKeyPressed(gef::Keyboard::KC_S))
			{

				// switch between frame based and skeleton based animation
				skeleton_anim_ = !skeleton_anim_;


				if (skeleton_anim_)
				{
					tex_json = "Dragon_tex.json";
					ske_json = "Dragon_ske.json";
				}
				else
				{
					tex_json = "boy-anim_tex.json";
					ske_json = "boy-anim_ske.json";
				}

				// restart the 2D character with the new animation type
				Init(platform);
			}
		}
	}

}

void AnimatedCharacter2D::Render(gef::SpriteRenderer* renderer)
{

	if (animation_data_->GetAnimationType() == "Armature")
	{
		// render the skeleton based animated sprite
		for (auto& part : parts_order_)
		{
			SetSpriteSizeAndPositionForFrame(skin_slots_[part].part_name);

			renderer->DrawSprite(sprite_, GetFinalTransform(part, sprite_scale_, sprite_position_));

		}
	}


	else if (animation_data_->GetAnimationType() == "Sheet")
	{
		// render the frame based animated sprite
		SetSpriteSizeAndPositionForFrame(animation_data_->GetCurrentFrameName());

		renderer->DrawSprite(sprite_);
	}


}

void AnimatedCharacter2D::UpdateBoneTransforms()
{
	// rebuild the bone transforms whenever the animation moves to the next frame
	if (animation_data_->IsChangingToNextFrame())
	{
		for (auto& bone : bones_)
		{
			bone.second.BuildLocalBoneTransform(animation_data_);
		}

		Bone::BuildWorldBoneTransforms(&bones_);

		animation_data_->SetChangingToNextFrame(false);
	}


}

void AnimatedCharacter2D::SetSpriteSizeAndPositionForFrame(const std::string& subtex_name)
{
	const SubTexture* frame = &texture_atlas_->subtextures[subtex_name];
	const float sprite_x = frame->size.x * 0.5f - (frame->frame_size.x * 0.5f + frame->frame_xy.x);
	const float sprite_y = frame->size.y * 0.5f - (frame->frame_size.y * 0.5f + frame->frame_xy.y);

	sprite_.set_width(frame->size.x);
	sprite_.set_height(frame->size.y);
	sprite_.set_uv_width(frame->size.x / texture_atlas_->size.x);
	sprite_.set_uv_height(frame->size.y / texture_atlas_->size.y);

	const float u = frame->xy.x / texture_atlas_->size.x;
	const float v = frame->xy.y / texture_atlas_->size.y;
	sprite_.set_uv_position(gef::Vector2(u, v));

	sprite_.set_position(gef::Vector4(sprite_x + sprite_position_.x, sprite_y + sprite_position_.y, 0.f));
}

gef::Matrix33 AnimatedCharacter2D::GetFinalTransform(const std::string& part, float scale, const gef::Vector2& sprite_pos)
{
	// obtain the final transform used to render the sprite 

	const std::string frame_name = skin_slots_[part].part_name;

	gef::Matrix33 rig_transform = gef::Matrix33::kIdentity;
	rig_transform.Scale(gef::Vector2(scale, scale));
	rig_transform.SetTranslation(gef::Vector2(sprite_pos.x, sprite_pos.y));

	const gef::Matrix33 subtexture_transform = texture_atlas_->subtextures[frame_name].subtexture_transform;
	const gef::Matrix33 sprite_offset_transform = skin_slots_[part].sprite_offset;
	const gef::Matrix33 bone_transform = bones_[part].world_transform_;

	return subtexture_transform * sprite_offset_transform * bone_transform * rig_transform;
}

std::string AnimatedCharacter2D::GetCurrentAnimationName()
{
	return animation_data_->GetAnimationName();
}

bool AnimatedCharacter2D::IsSkeletonBased()
{
	return skeleton_anim_;
}



std::vector<std::string> AnimatedCharacter2D::ReadPartsOrderFromJSON(rapidjson::Document& ske_doc)
{

	std::vector<std::string> parts;

	rapidjson::Value& slots = ske_doc["armature"][0]["slot"];

	for (int i = 0; i < static_cast<int>(slots.Size()); i++)
	{
		std::string part_name = slots[i]["name"].GetString();
		parts.push_back(part_name);
	}

	return parts;
}

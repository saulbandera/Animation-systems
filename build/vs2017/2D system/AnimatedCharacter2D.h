#pragma once
#pragma once
#include <maths/vector2.h>
#include <graphics/texture.h>
#include <graphics/sprite.h>
#include <system/platform.h>
#include "AnimationData2D.h"
#include "TextureAtlas.h"
#include "SkinSlot.h"
#include "Bone.h"

class AnimatedCharacter2D
{

public:
	AnimatedCharacter2D();

	void Init(gef::Platform& platform);

	void Update(float frame_time);
	void HandleInput(const gef::InputManager* input_manager, gef::Platform& platform);
	void Render(gef::SpriteRenderer* renderer);

	void UpdateBoneTransforms();
	void SetSpriteSizeAndPositionForFrame(const std::string& subtex_name);

	gef::Matrix33 GetFinalTransform(const std::string& part, float scale, const gef::Vector2& sprite_pos);

	std::string GetCurrentAnimationName();
	bool IsSkeletonBased();
private:
	static std::vector<std::string> ReadPartsOrderFromJSON(rapidjson::Document& ske_doc);

	const char* tex_json;
	const char* ske_json;

	bool skeleton_anim_; // skeleton or framebased

	AnimationData2D* animation_data_;
	float sprite_scale_;

	gef::Vector2 sprite_position_;
	gef::Texture* sprite_texture_;
	gef::Sprite sprite_;

	TextureAtlas* texture_atlas_;

	std::map<std::string, SkinSlot> skin_slots_;
	std::map<std::string, Bone> bones_;

	std::vector<std::string> parts_order_;

	
	bool KC1Pressed = false;


};


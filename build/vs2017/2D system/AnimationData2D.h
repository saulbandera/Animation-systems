#pragma once
#include <vector>
#include <string>
#include <map>
#include "rapidjson/include/rapidjson/document.h"



struct TranslationKey {
	TranslationKey() :
		start_time(0.0f),
		duration(0),
		current_frame(0),
		time_to_next_key(0.f),
		x(0.f),
		y(0.f)
	{}

	int start_time;
	int duration;
	int current_frame;
	float time_to_next_key;
	float x;
	float y;
};

struct RotationKey {
	RotationKey() :
		start_time(0.0f),
		duration(0),
		current_frame(0),
		time_to_next_key(0.f),
		rotation(0.f)
	{}

	int start_time;
	int duration;
	int current_frame;
	float time_to_next_key;
	float rotation;
};

struct BoneKey {
	std::string name;
	std::vector<TranslationKey> translation_keys;
	std::vector<RotationKey> rotation_keys;
};

struct Animation
{
	Animation() :
		duration(0)
	{}

	int duration;
	std::string name;
	std::vector<BoneKey> bone_keys;
	std::vector<std::string> frames_list;
};

class AnimationData2D
{
public:
	AnimationData2D();

	void Init(const char* ske_doc);
	void Update(float frame_time);


	std::string GetCurrentFrameName();
	int GetCurrentFrame() const;
	std::string GetAnimationType();
	std::string GetAnimationName();
	Animation* GetCurrentAnimation();
	void SwitchAnimation(const std::string& anim_name);
	void SetChangingToNextFrame(bool to_next_frame);
	bool IsChangingToNextFrame() const;

private:
	void ReadAnimationDataFromJSON(rapidjson::Document& ske_doc);

	bool to_next_frame_;

	int frame_rate_;
	int current_frame_;

	float elapsed_time_ ;

	std::string animation_type_;
	std::string current_frame_name_;
	std::string current_animation_;

	std::map<std::string, Animation> animations_;
	std::vector<std::string> animation_names_;

};


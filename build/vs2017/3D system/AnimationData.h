#pragma once

#include "array"
#include <map>
// Forward declarations
namespace gef
{
	class Animation;
	class Platform;
	class Scene;

}

class AnimationData
{

public:
	AnimationData(gef::Platform& platform);
	~AnimationData();

	void LoadAllAnimations(gef::Platform& platform, std::array<const char*, 7> file_names);
	static gef::Animation* LoadAnimation(const char* anim_scene_filename, const char* anim_name, gef::Platform& platform);

	gef::Animation* GetClip(int clip_id);
	int GetNumOfClips();


private:

	std::map<int, gef::Animation*> animation_clips_;

	std::array<const char*, 7> file_names_;

};


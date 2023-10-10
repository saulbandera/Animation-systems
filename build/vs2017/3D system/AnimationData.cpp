#include "AnimationData.h"
#include <graphics/scene.h>
#include <system/platform.h>
#include "animation/animation.h"


AnimationData::AnimationData(gef::Platform& platform)
{
	file_names_[0] = "ybot/ybot@idle_standard2.scn";
	file_names_[1] = "ybot/ybot@idle_happy.scn";
	file_names_[2] = "ybot/ybot@walk_standard.scn";
	file_names_[3] = "ybot/ybot@walk_happy.scn";
	file_names_[4] = "ybot/ybot@run_standard.scn";
	file_names_[5] = "ybot/ybot@dance_hiphop.scn";
	file_names_[6] = "ybot/ybot@dance_silly.scn";

	LoadAllAnimations(platform, file_names_);
	
}


AnimationData::~AnimationData()
{

}


void AnimationData::LoadAllAnimations(gef::Platform& platform, std::array<const char*, 7> file_names)
{
	for (size_t i = 0; i < file_names.size(); i++)
	{
		gef::Animation* new_anim = LoadAnimation(file_names[i], "", platform);
		animation_clips_[static_cast<int>(i)] = new_anim;
	}
}


gef::Animation* AnimationData::LoadAnimation(const char* anim_scene_filename, const char* anim_name, gef::Platform& platform)
{
	gef::Animation* anim = nullptr;

	gef::Scene anim_scene;
	if (anim_scene.ReadSceneFromFile(platform, anim_scene_filename))
	{
		// if the animation name is specified then try and find the named anim
		// otherwise return the first animation if there is one
		std::map<gef::StringId, gef::Animation*>::const_iterator anim_node_iter;
		if (anim_name)
			anim_node_iter = anim_scene.animations.find(gef::GetStringId(anim_name));
		else
			anim_node_iter = anim_scene.animations.begin();

		if (anim_node_iter != anim_scene.animations.end())
			anim = new gef::Animation(*anim_node_iter->second);
	}

	return anim;
}


gef::Animation* AnimationData::GetClip(const int clip_id)
{
	return animation_clips_[clip_id];
}

int AnimationData::GetNumOfClips()
{
	return static_cast<int>(animation_clips_.size());
}

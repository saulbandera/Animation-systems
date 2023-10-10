#pragma once
#include "AnimationData.h"
#include "BlendTree.h"
#include "ClipNode.h"
#include "TwoDimensionalBlendNode.h"
#include "animation/skeleton.h"
#include <graphics/skinned_mesh_instance.h>

namespace gef
{
	class Platform;
	class Renderer3D;
	class Mesh;
	class Skeleton;
}

class AnimationControl
{

public:

	AnimationControl(gef::Platform& platform, const gef::SkeletonPose& bind_pose);

	void FrozenTransition(BlendTree& _tree, BlendNode* _destination_node, float duration);
	void SmoothTransition(BlendTree& _tree, BlendNode* _destination_node, float duration);
	void GradualTransition(BlendTree& _tree, BlendNode* _destination_node, float duration);
	void InitBlendTree(const gef::SkeletonPose& bind_pose);
	void Reset2DBlendNodePlaybackTime();
	void Update(float frame_time, gef::SkinnedMeshInstance* skinned_mesh);
	BlendTree& GetTree();


	enum clip_node : int {
		idle_standard = 0,
		idle_happy = 1,
		walk_standard = 2,
		walk_happy = 3,
		run = 4,
		dance_silly = 5,
		dance_wavy = 6
	};

	ClipNode* GetClipNode(clip_node node_);
	TwoDimensionalBlendNode* Get2DBlendNode();

	float GetHappiness();
	void SetHappiness(float new_value);
	float GetSpeed();
	void SetSpeed(float new_value);


private:

	BlendTree main_blend_tree_;

	TwoDimensionalBlendNode* blend_node_2D_;

	ClipNode* clip_nodes_[7];

	float speed_ = 0.0f;
	float happiness_ = 0.0f;

	AnimationData* anim_data_;


};


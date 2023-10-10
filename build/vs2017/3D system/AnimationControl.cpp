#include "AnimationControl.h"

#include <iostream>

#include "AnimationData.h"
#include "SmoothTransitionNode.h"
#include "FrozenTransitionNode.h"
#include  "GradualTransitionNode.h"
#include "animation/animation.h"

AnimationControl::AnimationControl(gef::Platform& platform, const gef::SkeletonPose& bind_pose) :
	anim_data_(nullptr)
{
	anim_data_ = new AnimationData(platform);
	InitBlendTree(bind_pose);
}

void AnimationControl::FrozenTransition(BlendTree& _tree, BlendNode* _destination_node, float duration)
{
	auto* transition_node = new FrozenTransitionNode(&_tree);
	_tree.variables_["transition_time"] = duration;
	transition_node->SetVariable(0, "transition_time");

	transition_node->SetInput(0, _tree.output_.inputs_[0].node);
	transition_node->SetInput(1, _destination_node);

	_tree.output_.SetInput(0, transition_node);

}

void AnimationControl::SmoothTransition(BlendTree& _tree, BlendNode* _destination_node, float duration)
{

	auto* transition_node = new SmoothTransitionNode(&_tree);
	_tree.variables_["transition_time"] = duration;
	transition_node->SetVariable(0, "transition_time");

	transition_node->SetInput(0, _tree.output_.inputs_[0].node);
	transition_node->SetInput(1, _destination_node);

	_tree.output_.SetInput(0, transition_node);

}

void AnimationControl::GradualTransition(BlendTree& _tree, BlendNode* _destination_node, float duration)
{
	const auto* clip_A = dynamic_cast<ClipNode*>(_tree.output_.inputs_[0].node);
	const auto* clip_B = dynamic_cast<ClipNode*>(_destination_node);

	auto* transition_walk_to_run_node = new GradualTransitionNode(&_tree, clip_A->clip_player_.clip()->duration(), clip_B->clip_player_.clip()->duration());

	_tree.variables_["transition_time"] = duration;
	transition_walk_to_run_node->SetVariable(0, "transition_time");

	transition_walk_to_run_node->SetInput(0, _tree.output_.inputs_[0].node);
	transition_walk_to_run_node->SetInput(1, _destination_node);

	_tree.output_.SetInput(0, transition_walk_to_run_node);
}


void AnimationControl::InitBlendTree(const gef::SkeletonPose& bind_pose)
{

	//! INITIALIZING THE BLEND TREE

	main_blend_tree_.Init(bind_pose);

	// initialize the clip nodes for the animations
	for(int i = 0; i < anim_data_->GetNumOfClips(); i++)
	{
		auto* clip_node = new ClipNode(&main_blend_tree_);
		clip_node->SetClip(anim_data_->GetClip(i));
		clip_nodes_[i] = clip_node;
	}


	// initialise a 2D blend node 
	blend_node_2D_ = new TwoDimensionalBlendNode(&main_blend_tree_, anim_data_->GetClip(2)->duration(), anim_data_->GetClip(3)->duration());

	main_blend_tree_.variables_["happiness_blend"] = happiness_;
	blend_node_2D_->SetVariable(0, "happiness_blend");

	main_blend_tree_.variables_["idle_walk_blend"] = speed_;
	blend_node_2D_->SetVariable(1, "idle_walk_blend");

	blend_node_2D_->SetInput(0, clip_nodes_[idle_standard]);
	blend_node_2D_->SetInput(1, clip_nodes_[idle_happy]);
	blend_node_2D_->SetInput(2, clip_nodes_[walk_standard]);
	blend_node_2D_->SetInput(3, clip_nodes_[walk_happy]);

	main_blend_tree_.output_.SetInput(0, blend_node_2D_);



}

void AnimationControl::Reset2DBlendNodePlaybackTime()
{

	// reset animation time and playback speed to initial values 
	auto* clip_A = dynamic_cast<ClipNode*>(blend_node_2D_->inputs_[2].node);
	auto* clip_B = dynamic_cast<ClipNode*>(blend_node_2D_->inputs_[3].node);

	clip_A->clip_player_.set_anim_time(0.0f);
	clip_A->clip_player_.set_playback_speed(1.0f);
	clip_B->clip_player_.set_anim_time(0.0f);
	clip_B->clip_player_.set_playback_speed(1.0f);

	blend_node_2D_->SetInput(2, clip_A);
	blend_node_2D_->SetInput(3, clip_B);

}

void AnimationControl::Update(float frame_time, gef::SkinnedMeshInstance* skinned_mesh)
{
	main_blend_tree_.Update(frame_time); // calls UpdateInternal on the nodes

	skinned_mesh->UpdateBoneMatrices(main_blend_tree_.output_.output_pose_);

	main_blend_tree_.variables_["happiness_blend"] = happiness_;
	main_blend_tree_.variables_["idle_walk_blend"] = speed_;


}

BlendTree& AnimationControl::GetTree()
{
	return main_blend_tree_;
}

ClipNode* AnimationControl::GetClipNode(clip_node node_)
{
	return clip_nodes_[node_];
}

TwoDimensionalBlendNode* AnimationControl::Get2DBlendNode()
{
	return blend_node_2D_;
}

float AnimationControl::GetHappiness()
{
	return happiness_;
}

void AnimationControl::SetHappiness(float new_value)
{
	happiness_ = new_value;
}

float AnimationControl::GetSpeed()
{
	return speed_;
}

void AnimationControl::SetSpeed(float new_value)
{
	speed_ = new_value;
}

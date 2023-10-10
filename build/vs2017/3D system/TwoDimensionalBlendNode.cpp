#include "TwoDimensionalBlendNode.h"
#include "BlendTree.h"
#include "ClipNode.h"
TwoDimensionalBlendNode::TwoDimensionalBlendNode(BlendTree* _tree, float duration_anim_A, float duration_anim_B) :
	BlendNode(_tree)
{
	inputs_.resize(4);
	variables_.resize(2);

	anim_A_speed_min = 1.0f;
	anim_A_speed_max = duration_anim_A / duration_anim_B;

	anim_B_speed_min = duration_anim_B / duration_anim_A;
	anim_B_speed_max = 1.0f;

}

bool TwoDimensionalBlendNode::UpdateInternal(float delta_time)
{


	float happy_blend_value = tree_->variables_[variables_[0]];
	gef::SkeletonPose pose_a = output_pose_;

	pose_a.Linear2PoseBlend(inputs_[0].node->output_pose_, inputs_[1].node->output_pose_, happy_blend_value);


	auto* clip_A = dynamic_cast<ClipNode*>(inputs_[2].node);
	auto* clip_B = dynamic_cast<ClipNode*>(inputs_[3].node);


	if (clip_A->clip_player_.looped_ && waiting_)
	{
		waiting_ = false;
		// set clip B animation time to the animation time of clip A 
		clip_B->clip_player_.set_anim_time(clip_A->clip_player_.anim_time());
	}
	else if (waiting_)
	{
		// play clip A until it loops 
		output_pose_ = pose_a;
	}

	if (!waiting_)
	{
		gef::SkeletonPose pose_b = output_pose_;
		float idle_to_walk_blend_value = tree_->variables_[variables_[1]]; 


		// Calculating the offset from the minimum speed of the walking animation and run animation based on the blend value
		// this will gradually increase as we transition from walking to running 
		float walk_speed_mod = (anim_A_speed_max - anim_A_speed_min) * happy_blend_value;
		float walk_happy_speed_mod = (anim_B_speed_max - anim_B_speed_min) * happy_blend_value;


		auto* clip_A = dynamic_cast<ClipNode*>(inputs_[2].node);
		auto* clip_B = dynamic_cast<ClipNode*>(inputs_[3].node);

		clip_A->clip_player_.set_playback_speed(anim_A_speed_min + walk_speed_mod);
		clip_B->clip_player_.set_playback_speed(anim_B_speed_min + walk_happy_speed_mod);

		SetInput(2, clip_A);
		SetInput(3, clip_B);

		pose_b.Linear2PoseBlend(inputs_[2].node->output_pose_, inputs_[3].node->output_pose_, happy_blend_value);

		output_pose_.Linear2PoseBlend(pose_a, pose_b, idle_to_walk_blend_value);
	}


	return true;
}
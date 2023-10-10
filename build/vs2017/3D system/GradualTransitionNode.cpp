#include "GradualTransitionNode.h"
#include "BlendTree.h"
#include <iostream>
#include "ClipNode.h"
#include "animation/animation.h"

GradualTransitionNode::GradualTransitionNode(BlendTree* _tree, float duration_anim_A, float duration_anim_B) :
	BlendNode(_tree)

{
	inputs_.resize(2);
	variables_.resize(1);

	anim_A_speed_min = 1.0f;
	anim_A_speed_max = duration_anim_A / duration_anim_B;

	anim_B_speed_min = duration_anim_B / duration_anim_A;
	anim_B_speed_max = 1.0f;

}




bool GradualTransitionNode::UpdateInternal(float delta_time)
{

	auto* clip_A = dynamic_cast<ClipNode*>(inputs_[0].node);
	auto* clip_B = dynamic_cast<ClipNode*>(inputs_[1].node);


	// wait for clip A to loop back to start and synchronize both clips: 
	if (clip_A->clip_player_.looped_ && waiting_)
	{
		waiting_ = false;
		// set clip B animation time to the animation time of clip A 
		clip_B->clip_player_.set_anim_time(clip_A->clip_player_.anim_time());

	}
	else if(waiting_)
	{
		// play clip A until it loops
		output_pose_ = inputs_[0].node->output_pose_;
	}

	if(!waiting_)
	{
		const float transition_time = tree_->variables_[variables_[0]];

		if (timer_ < transition_time)
		{
			const float fps_ = 1.0f / delta_time;
			timer_ += 1 / fps_;

			// first calculate the normalized time between the start and end of the transition:
			float anim_blend = timer_ / transition_time;

			// Calculating the offset from the minimum speed of the walking animation and run animation based on the blend value
			// this will gradually increase as we transition from walking to running 
			float walk_speed_mod = (anim_A_speed_max - anim_A_speed_min) * anim_blend;
			float run_speed_mod = (anim_B_speed_max - anim_B_speed_min) * anim_blend;

			auto* clip_A = dynamic_cast<ClipNode*>(inputs_[0].node);
			auto* clip_B = dynamic_cast<ClipNode*>(inputs_[1].node);

			clip_A->clip_player_.set_playback_speed(anim_A_speed_min + walk_speed_mod);
			clip_B->clip_player_.set_playback_speed(anim_B_speed_min + run_speed_mod);

			SetInput(0, clip_A);
			SetInput(1, clip_B);

			if (anim_blend > 1 || timer_ > transition_time)
			{
				anim_blend = 1.0f;
				timer_ = transition_time;
			}
			// perform the blend of the two animation poses of running and walking 0 blend anim is walking, 1 blend anim is running
			output_pose_.Linear2PoseBlend(inputs_[0].node->output_pose_, inputs_[1].node->output_pose_, anim_blend);
		}

		else
		{
			tree_->should_update_ = false;
			// reset playback speed of walk animation
			clip_A->clip_player_.set_playback_speed(1.0f);
			SetInput(0, clip_A);

			output_pose_ = inputs_[1].node->output_pose_;
		}
	}

	return true;
}

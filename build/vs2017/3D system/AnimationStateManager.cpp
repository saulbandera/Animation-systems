#include "AnimationStateManager.h"
#include <iostream>
AnimationStateManager::AnimationStateManager(gef::Platform& platform, const gef::SkeletonPose& bind_pose) :
anim_control_(nullptr)
{
	anim_control_ = new AnimationControl(platform, bind_pose);
}

void AnimationStateManager::Update(float frame_time, gef::SkinnedMeshInstance* skinned_mesh)
{

	if (current_state != INVERSE_KINEMATICS) {

		if (anim_control_->GetTree().should_update_) {
			// update the blend tree and update bone matrices for the skinned mesh
			anim_control_->Update(frame_time, skinned_mesh);

		}
	}

	if (previous_state == TRANSITION_TO_RUN && current_state == WALKING && in_transition_)
	{
		previous_state = current_state;
		current_state = TRANSITION_TO_RUN;
		transition_to_run_ = true;
	}

	// perform a gradual transition from walking to running
	if (transition_to_run_)
	{
		transition_to_run_ = false;
		anim_control_->GradualTransition(anim_control_->GetTree(), anim_control_->GetClipNode(AnimationControl::run), 1.0f);
	}


	if (to_transition_)
	{
		switch (current_state)
		{

		case TRANSITION_TO_RUN:
			if (previous_state != WALKING) {
				// transition to a standard walk first
				if (previous_state == STANDARD_TO_HAPPY_2D_BLEND)
				{
					// reduce the happiness value and increase the speed
					anim_control_->SetHappiness(anim_control_->GetHappiness() <= 0.0f ? 0.0f : anim_control_->GetHappiness() - 0.03f);

					anim_control_->SetSpeed(anim_control_->GetSpeed() >= 1.0f ? 1.0f : anim_control_->GetSpeed() + 0.03f);

					// if speed = 1 it means the animation is set to walking standard
					if (anim_control_->GetSpeed() == 1.0f && anim_control_->GetHappiness() == 0.0f)
					{
						to_transition_ = false;
						in_transition_ = true;
						anim_control_->GetTree().output_.SetInput(0, anim_control_->GetClipNode(AnimationControl::walk_standard));
						previous_state = current_state;
						current_state = WALKING;
					}
				}
				// perform a smooth transition to walking 
				if (previous_state == WAVY_DANCE || previous_state == SILLY_DANCE || previous_state == INVERSE_KINEMATICS)
					anim_control_->SmoothTransition(anim_control_->GetTree(), anim_control_->GetClipNode(AnimationControl::walk_standard), 1.f);
			}
			break;

		
		case TRANSITION_TO_WAVY_DANCE:
			// perform a frozen transition from the current animation to the wavy dance animation
			anim_control_->FrozenTransition(anim_control_->GetTree(), anim_control_->GetClipNode(AnimationControl::dance_wavy), 1.0f);;
			break;

		case TRANSITION_TO_SILLY_DANCE:
			// perform a frozen transition from the current animation to the silly dance animation
			anim_control_->FrozenTransition(anim_control_->GetTree(), anim_control_->GetClipNode(AnimationControl::dance_silly), 1.0f);
			break;

		case TRANSITION_TO_2D_BLEND:
			// transition from the current animation to the 2D blend animation
			anim_control_->Reset2DBlendNodePlaybackTime();

			if (previous_state == RUNNING)
				anim_control_->SmoothTransition(anim_control_->GetTree(), anim_control_->Get2DBlendNode(), 4.0f);
			else
				anim_control_->FrozenTransition(anim_control_->GetTree(), anim_control_->Get2DBlendNode(), 2.0f);


			break;

		}

		if (!(current_state == TRANSITION_TO_RUN && previous_state == STANDARD_TO_HAPPY_2D_BLEND))
		{
			// set in transition to true meaning the system is currently transitioning to another state
			to_transition_ = false;
			in_transition_ = true;
			if(current_state == INVERSE_KINEMATICS)
			{
				in_transition_ = false;
			}
		}
	}



	// should_update_ is set to false once a transition is finished
	if (anim_control_->GetTree().should_update_ == false)
	{
		switch (current_state)
		{

		case TRANSITION_TO_RUN:
			if (previous_state == WALKING)
			{
				// the current animation played is now the running animation
				previous_state = current_state;
				current_state = RUNNING;
				anim_control_->GetTree().output_.SetInput(0, anim_control_->GetClipNode(AnimationControl::run));
				break;
			}
			if (previous_state != WALKING)
			{
				// the current animation played is now the walking animation
				previous_state = current_state;
				current_state = WALKING;
				anim_control_->GetTree().output_.SetInput(0, anim_control_->GetClipNode(AnimationControl::walk_standard));
				break;
			}
			break;

		case TRANSITION_TO_2D_BLEND:
			// the current animation is now a 2D blend
			previous_state = current_state;
			current_state = STANDARD_TO_HAPPY_2D_BLEND;
			anim_control_->GetTree().output_.SetInput(0, anim_control_->Get2DBlendNode());
			break;

		case TRANSITION_TO_WAVY_DANCE:
			// current animation is wavy dance
			previous_state = current_state;
			current_state = WAVY_DANCE;
			anim_control_->GetTree().output_.SetInput(0, anim_control_->GetClipNode(AnimationControl::dance_wavy));
			break;

		case TRANSITION_TO_SILLY_DANCE:
			// current animation is now silly dance
			previous_state = current_state;
			current_state = SILLY_DANCE;
			anim_control_->GetTree().output_.SetInput(0, anim_control_->GetClipNode(AnimationControl::dance_silly));
			break;

		}
		// no longer in transition
		in_transition_ = false;

		if (current_state == WALKING && previous_state == TRANSITION_TO_RUN)
		{
			// this is set to true as to transition to run we must first transition to walk
			in_transition_ = true;
		}
		// set should update to true to continue updating the blend tree
		anim_control_->GetTree().should_update_ = true;
	}





}

int AnimationStateManager::GetCurrentState()
{
	return current_state;
}

void AnimationStateManager::SetCurrentState(int new_state)
{

	current_state = new_state;
}

void AnimationStateManager::SetPreviousState(int new_state)
{
	previous_state = new_state;

}

AnimationControl* AnimationStateManager::GetAnimController()
{
	return anim_control_;
}

bool AnimationStateManager::IsInTransition()
{
	return in_transition_;
}

bool AnimationStateManager::ToTransition()
{
	return to_transition_;
}

void AnimationStateManager::SetToTransition(bool to_transition)
{
	to_transition_ = to_transition;
}

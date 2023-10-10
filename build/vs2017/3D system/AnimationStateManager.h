#pragma once
#include "AnimationControl.h"
#include <graphics/skinned_mesh_instance.h>


class AnimationStateManager
{
public:
	AnimationStateManager(gef::Platform& platform, const gef::SkeletonPose& bind_pose);

	void Update(float frame_time ,gef::SkinnedMeshInstance* skinned_mesh);


	enum state :int {
		WAVY_DANCE = 1,
		SILLY_DANCE = 2,
		STANDARD_TO_HAPPY_2D_BLEND = 3,
		WALKING = 4,
		RUNNING = 5,
		TRANSITION_TO_RUN = 6,
		TRANSITION_TO_WAVY_DANCE = 7,
		TRANSITION_TO_SILLY_DANCE = 8,
		TRANSITION_TO_2D_BLEND = 9,
		TRANSITION_TO_STANDARD_WALK = 10,
		INVERSE_KINEMATICS = 11
	};


	int GetCurrentState();
	void SetCurrentState(int new_state);
	void SetPreviousState(int new_state);
	AnimationControl* GetAnimController();

	bool IsInTransition();
	bool ToTransition();
	void SetToTransition(bool to_transition);
private:


	AnimationControl* anim_control_;

	bool transition_to_run_ = false;
	bool in_transition_ = false;
	bool to_transition_ = false;;
	int current_state = 3;
	int previous_state = 3;
};


#include "ClipNode.h"
#include "BlendTree.h"
#include <iostream>

#include "animation/animation.h"

ClipNode::ClipNode(BlendTree* _tree) :
	BlendNode(_tree)
{
	if (tree_)
		clip_player_.Init(tree_->bind_pose_);

	clip_player_.set_looping(true);
}


void ClipNode::SetClip(const gef::Animation* anim)
{
	clip_player_.set_clip(anim);
}


void ClipNode::StartInternal()
{
	clip_player_.set_anim_time(0.0f);
}


bool ClipNode::UpdateInternal(float delta_time)
{

	if (should_update_) {

		bool valid = false;
		if (clip_player_.clip())
		{
			clip_player_.Update(delta_time, tree_->bind_pose_);
			output_pose_ = clip_player_.pose();
			valid = true;
		}
		return valid;
	}
	return true;
}

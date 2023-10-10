#include "SmoothTransitionNode.h"
#include "BlendTree.h"
SmoothTransitionNode::SmoothTransitionNode(BlendTree* _tree) :
	BlendNode(_tree)

{
	inputs_.resize(2);
	variables_.resize(1);
}


bool SmoothTransitionNode::UpdateInternal(float delta_time)
{
	const float transition_time = tree_->variables_[variables_[0]];

	if (timer_ < transition_time)
	{
		const float fps_ = 1.0f / delta_time;
		timer_ += 1 / fps_;


		// calculating the blend value according to a one-dimensional Bézier ease-in/ease-out curve:

		// first calculate the normalized time between the start and end of the transition:
		const float u = timer_ / transition_time;

		// calculate the inverse normalized time:
		const float v = 1 - u;

		// calculate the blend value:
		float blend_value = (3 * v * (u * u) + u * u * u);



		if (blend_value > 1 || timer_ > transition_time)
		{
			blend_value = 1.0f;
			timer_ = transition_time;
		}


		output_pose_.Linear2PoseBlend(inputs_[0].node->output_pose_, inputs_[1].node->output_pose_, blend_value);
	}

	else
	{

		tree_->should_update_ = false;

		output_pose_ = inputs_[1].node->output_pose_;
	}


	return true;
}

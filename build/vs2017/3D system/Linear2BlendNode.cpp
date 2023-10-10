#include "Linear2BlendNode.h"
#include "BlendTree.h"

Linear2BlendNode::Linear2BlendNode(BlendTree* _tree) :
	BlendNode(_tree)
{
	inputs_.resize(2);
	variables_.resize(1);

}

bool Linear2BlendNode::UpdateInternal(float delta_time)
{
	float blend_value = tree_->variables_[variables_[0]];
	output_pose_.Linear2PoseBlend(inputs_[0].node->output_pose_, inputs_[1].node->output_pose_, blend_value);

	return true;
}

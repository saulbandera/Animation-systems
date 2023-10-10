#include "OutputNode.h"

OutputNode::OutputNode(BlendTree* _tree) :
	BlendNode(_tree)
{
	inputs_.resize(1);
} 

bool OutputNode::UpdateInternal(float delta_time)
{
	output_pose_ = inputs_[0].node->output_pose_;
	return true;
}

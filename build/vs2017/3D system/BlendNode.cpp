#include "BlendNode.h"
#include "BlendTree.h"
#include <iostream>
BlendNode::BlendNode(BlendTree* _tree) :
tree_(_tree)
{
	if (tree_)
		output_pose_ = tree_->bind_pose_;

}

bool BlendNode::Update(float delta_time)
{
	bool all_inputs_valid = true;

	if(inputs_.size() > 0)
	{
		for(int input_num = 0; input_num < inputs_.size(); input_num++)
		{
			BlendNodeInput& input = inputs_[input_num];
			bool input_valid;
			if(input.node)
			{
				input_valid = input.node->Update(delta_time);
			}
			else
			{
				input_valid = false;
			}
			if (!input_valid)
				all_inputs_valid = false;
		}
	}



	bool all_variables_valid = true;
	if(variables_.size() > 0)
	{
		for(int variable_num=0; variable_num < variables_.size(); variable_num ++)
		{
			const std::string& variable = variables_[variable_num];
			bool variable_valid = tree_->variables_.find(variable) != tree_->variables_.end();

			if(!variable_valid && all_variables_valid)
			{
				all_variables_valid = false; 
			}
		}
	}

	bool output_valid = false;

	if (all_inputs_valid && all_variables_valid)
		output_valid = UpdateInternal(delta_time);

	return output_valid;

}

void BlendNode::Start()
{
	for (int input_num = 0; input_num < inputs_.size(); input_num++)
	{
		BlendNodeInput& input = inputs_[input_num];

		if (input.node)
			input.node->Start();
	}
	StartInternal();  

}


void BlendNode::SetInput(int input_num, BlendNode* node)
{

	if(node && input_num < static_cast<int>(inputs_.size()))
	{
		inputs_[input_num].node = node;
	}
}

void BlendNode::SetVariable(int variable_num, const std::string& variable)
{
	if(variable.size() > 0 && variable_num < static_cast<int>(variables_.size()))
	{
		variables_[variable_num] = variable;
	}

}





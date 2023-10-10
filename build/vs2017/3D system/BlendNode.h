#pragma once
#include <vector>
#include "animation/skeleton.h"
#include <string>

class BlendTree;


class BlendNode
{
public:

	struct BlendNodeInput
	{
		BlendNode* node;

		BlendNodeInput() :
			node(nullptr)
		{
		}
	};


	BlendNode(BlendTree* _tree);

	std::vector<BlendNodeInput> inputs_;
	std::vector<std::string> variables_;

	gef::SkeletonPose output_pose_;
	BlendTree* tree_;

	bool Update(float delta_time);
	void Start();
	virtual bool UpdateInternal(float delta_time) = 0;
	virtual void StartInternal() { }
	void SetInput(int input_num, BlendNode* node);
	void SetVariable(int variable_num, const std::string& variable);

	bool should_update_ = true;


};


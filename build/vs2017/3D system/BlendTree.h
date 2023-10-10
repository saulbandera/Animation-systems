#pragma once
#include <map>
#include "animation/skeleton.h"
#include "OutputNode.h"

class BlendTree
{
public:
	BlendTree();
	void Init(const gef::SkeletonPose& bind_pose);
	void CleanUp();
	void Start();
	void Update(float delta_time);

	OutputNode output_;

	gef::SkeletonPose bind_pose_;

	std::map<std::string, float> variables_;

	bool should_update_ = true;
};


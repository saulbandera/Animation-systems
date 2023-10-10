#include "BlendTree.h"



BlendTree::BlendTree() :
	output_(this)
{

}

void BlendTree::Init(const gef::SkeletonPose& bind_pose)
{
	bind_pose_ = bind_pose;
}

void BlendTree::CleanUp()
{
}

void BlendTree::Start()
{ 

	output_.Start();
}

void BlendTree::Update(float delta_time)
{
	bool valid = output_.Update(delta_time);

}

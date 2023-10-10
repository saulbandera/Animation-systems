#pragma once
#include "BlendNode.h"
class SmoothTransitionNode :
    public BlendNode
{


public:
    SmoothTransitionNode(BlendTree* _tree);
    bool UpdateInternal(float delta_time) override;

private:
    float timer_ = 0.0f; 
};


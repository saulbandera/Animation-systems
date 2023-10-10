#pragma once
#include "BlendNode.h"
class FrozenTransitionNode :
    public BlendNode
{

public:
    FrozenTransitionNode(BlendTree* _tree);
    bool UpdateInternal(float delta_time) override;

private:
    float timer_ = 0.0f;
};


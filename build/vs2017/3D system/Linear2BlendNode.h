#pragma once
#include "BlendNode.h"
class Linear2BlendNode :
    public BlendNode
{

public:
    Linear2BlendNode(BlendTree* _tree);
    bool UpdateInternal(float delta_time) override;
};


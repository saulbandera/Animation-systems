#pragma once
#include "BlendNode.h"
class TwoDimensionalBlendNode :
    public BlendNode
{


public:
    TwoDimensionalBlendNode(BlendTree* _tree, float duration_anim_A, float duration_anim_B);
    bool UpdateInternal(float delta_time) override;
private:
    float anim_A_speed_min;
    float anim_A_speed_max;

    float anim_B_speed_min;
    float anim_B_speed_max;

    bool waiting_ = true;
};


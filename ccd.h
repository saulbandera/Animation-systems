
#ifndef _CCD_H
#include <animation/skeleton.h>
#include <graphics/skinned_mesh_instance.h>

bool CalculateCCD(gef::SkeletonPose& pose, const gef::SkinnedMeshInstance& animated_mesh_instance, const gef::Vector4& dest_point, const std::vector<int>& bone_indices);

#endif _CCD_H

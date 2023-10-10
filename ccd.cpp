#include "ccd.h"
#include <algorithm>
#include <maths/math_utils.h>
#include <graphics/colour.h>



static float Clamp(float v, float from, float to) {
	if (v < from) v = from;
	if (v > to) v = to;
	return v;
}

static float Angle(const gef::Vector4& vec, const gef::Vector4& other) {

	float l1 = vec.LengthSqr();
	float l2 = other.LengthSqr();
	bool is_norm = l1 == 1.f && l2 == 1.f;

	float dot = vec.DotProduct(other);


	if (!is_norm) {
		
		float denominator = sqrtf(l1 * l2);
		dot /= denominator;

	}

	return acosf(Clamp(dot, -1.f, 1.f));
}

gef::Quaternion Quat(const gef::Vector4& axis, float angle) {
	gef::Quaternion q;
	float sin_angle = sinf(angle / 2.f);
	q.x = sin_angle * axis.x();
	q.y = sin_angle * axis.y();
	q.z = sin_angle * axis.z();
	q.w = cosf(angle / 2.f);
	return q;
}


bool CalculateCCD(gef::SkeletonPose& pose, const gef::SkinnedMeshInstance& animated_mesh_instance, const gef::Vector4& dest_point, const std::vector<int>& bone_indices)
{
	if (bone_indices.size() < 2) {
		return false;
	}

	std::vector<gef::Matrix44>& global_pose = pose.global_pose_ref();
	std::vector<gef::JointPose>& local_pose = pose.local_pose_ref();

	// Obtain the inverse of the animated mesh transform
	gef::Matrix44 world_to_model_tran;
	world_to_model_tran.Inverse(animated_mesh_instance.transform());

	// Transform the destination point from world space to model space
	const gef::Vector4 dest_point_model_space = dest_point.Transform(world_to_model_tran);

	// Get the end effectors position
	gef::Vector4 end_effector = global_pose[bone_indices.back()].GetTranslation();

	// calculate the distance between the end effectors position and the destination position
	float distance = (dest_point_model_space - end_effector).Length();


	float old_distance = distance;
	float distance_delta = distance;


	const float min_distance = 0.001f;

	// max number of iterations permitted in case no solution is found
	int max_iterations = 200;


	// Perform CCD algorithm while the distance between the end effector and the destination point
	// is larger than the minimum distance or until the max number of iterations is reached
	while (distance > min_distance && max_iterations > 0 && distance_delta > 0.01f)
	{

		
		for (int i = static_cast<int>(bone_indices.size() - 2); i >= 0; --i) {
			int bone = bone_indices[i];
			gef::Matrix44& bone_transform = global_pose[bone];
			const gef::Vector4& bone_position = bone_transform.GetTranslation();

			

			gef::Vector4 EB = end_effector - bone_position;
			gef::Vector4 DB = dest_point_model_space - bone_position;
			
			EB.Normalise();
			DB.Normalise();

			gef::Vector4 axis = EB.CrossProduct(DB);
			axis.Normalise();


			const float angle = Angle(EB, DB);
			

			gef::Quaternion ik_bone_rotation = Quat(axis, angle);
			ik_bone_rotation.Normalise();

			gef::Matrix44 m;
			m.Rotation(ik_bone_rotation);
			const gef::Matrix44 next_matrix = m * bone_transform;

			gef::Matrix44 old_parent_tran = bone_transform;
			gef::Matrix44 new_parent_tran = next_matrix;

			for (int j = i + 1; j < static_cast<int>(bone_indices.size()); ++j) {
				const int child = bone_indices[j];

				gef::Matrix44& child_glob = global_pose[child];

				gef::Matrix44 parent_inv;
				parent_inv.Inverse(old_parent_tran);
				gef::Matrix44 child_local = child_glob * parent_inv;

				old_parent_tran = child_glob;
				child_glob = child_local * new_parent_tran;
				new_parent_tran = child_glob;
			}

			bone_transform = next_matrix;
		}

		// re-calculate the end effector position
		end_effector = global_pose[bone_indices.back()].GetTranslation();

		// re-calculate distance between end effector and destination point
		distance = (dest_point_model_space - end_effector).Length();
		distance_delta = fabsf(old_distance - distance);
		old_distance = distance;

		// decrease the iteration counter 
		--max_iterations;
	}


	// calculate the new local pose for the bones in the IK chain
	for (size_t i = 0; i < bone_indices.size(); ++i)
	{
		const int bone_number = bone_indices[i];

		const gef::Joint& joint = pose.skeleton()->joint(bone_number);
		if (joint.parent == -1)
		{
			local_pose[bone_number] = global_pose[bone_number];
		}
		else
		{
			gef::Matrix44 parent_inv;
			parent_inv.Inverse(global_pose[joint.parent]);
			local_pose[bone_number] = global_pose[bone_number] * parent_inv;
		}
	}

	// re-calculate global pose based on new local pose
	for (int i = 0; i < pose.skeleton()->joint_count(); ++i)
	{
		const gef::Joint& joint = pose.skeleton()->joint(i);
		const gef::Matrix44 local = local_pose[i].GetMatrix();
		if (joint.parent == -1)
			global_pose[i] = local;
		else
			global_pose[i] = local * global_pose[joint.parent];
	}

	return max_iterations > 0;
}

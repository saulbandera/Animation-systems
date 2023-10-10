#pragma once

#include "AnimationStateManager.h"
#include <graphics/skinned_mesh_instance.h>
#include <input/input_manager.h>
#include "primitive_builder.h"
#include "primitive_renderer.h"
#include <maths/vector4.h>


namespace gef
{
	class Platform;
	class Renderer3D;
	class Mesh;
	class Skeleton;
}


class AnimatedCharacter3D
{
public:
	AnimatedCharacter3D(gef::Platform& platform);
	~AnimatedCharacter3D();

	void Update(float delta_time);
	void HandleInput(gef::InputManager* input_manager, gef::Renderer3D* renderer, gef::Platform& platform);
	void Render(gef::Renderer3D* renderer);
	void LoadSkinnedMesh(gef::Platform& platform, const char* scene_file_name);
	void RenderEndEffector();

	AnimationStateManager* GetAnimManager();
	gef::Skeleton* GetFirstSkeleton(gef::Scene* scene) const;
	gef::Mesh* GetFirstMesh(gef::Scene* scene, gef::Platform& platform) const;
	gef::SkinnedMeshInstance* GetSkinnedMesh() const;
	


private:


	AnimationStateManager* anim_manager_;

	gef::Mesh* mesh_;
	gef::Scene* model_scene_;
	gef::SkinnedMeshInstance* animated_mesh_;

	PrimitiveBuilder* primitive_builder_;
	PrimitiveRenderer* primitive_renderer_;
	gef::SkeletonPose ik_pose_;
	gef::Vector4 effector_position_;
	float ndc_zmin_;
};


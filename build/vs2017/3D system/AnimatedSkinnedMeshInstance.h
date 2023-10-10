#pragma once

#include "AnimationData.h"
//#include "AnimationPlayer.h"
#include <graphics/skinned_mesh_instance.h>


namespace gef
{
	class Platform;
	class Renderer3D;
	class Mesh;
	class Skeleton; 
}
class AnimatedSkinnedMeshInstance
{

public:
	AnimatedSkinnedMeshInstance(gef::Platform& platform);
	~AnimatedSkinnedMeshInstance();


	void Update(float delta_time, float speed);
	void Render(gef::Renderer3D* renderer);

	void LoadSkinnedMesh(gef::Platform& platform, const char* scene_file_name);

	gef::Skeleton* GetFirstSkeleton(gef::Scene* scene) const; 
	gef::Mesh* GetFirstMesh(gef::Scene* scene, gef::Platform& platform) const;

	gef::SkinnedMeshInstance* GetSkinnedMesh() const;
	AnimationData* GetAnimation() const;


	// TODO: 
    // set position/rotation/scale
	// get position/rotation



private:

	gef::Mesh* mesh_; 
	gef::Scene* model_scene_; 
	gef::SkinnedMeshInstance* animated_mesh_; 


	AnimationData* animation_;





	




};


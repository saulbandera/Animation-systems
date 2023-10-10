#include "AnimatedSkinnedMeshInstance.h"
#include <animation/skeleton.h>
#include <graphics/scene.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>

#include "animation/animation.h"
#include "system/platform.h"
#include "maths/math_utils.h"
#include <iostream>
#include <algorithm>



AnimatedSkinnedMeshInstance::AnimatedSkinnedMeshInstance(gef::Platform& platform) :
	mesh_(nullptr),
	model_scene_(nullptr),
	animated_mesh_(nullptr),
	animation_(nullptr)

{
	
	LoadSkinnedMesh(platform, "ybot/ybot.scn");

	if (animated_mesh_) {
		animation_ = new AnimationData(platform);

	}


}

AnimatedSkinnedMeshInstance::~AnimatedSkinnedMeshInstance()
{
	delete animation_;
	animation_ = nullptr;

	delete mesh_;
	mesh_ = nullptr;

	delete model_scene_;
	model_scene_ = nullptr;

	delete animated_mesh_;
	animated_mesh_ = nullptr;
}

void AnimatedSkinnedMeshInstance::Update(float delta_time, float speed)
{

	if (animated_mesh_)
	{
		// update the pose in the animation player from the animation
		//animation_->GetAnimationPlayer()->Play(delta_time); //COMMENTED OUT WHILE DOING ANIMATION BLENDING CODE ABOVE (b)

		// update the bone matrices that are used for rendering the character
		// from the newly updated pose in the animation player
		//animated_mesh_->UpdateBoneMatrices(animation_->GetAnimationPlayer()->GetPose()); // COMMENTED OUT WHILE DOING ANIMATION BLENDING CODE ABOVE (b)

		// build a transformation matrix that will position the character
		// use this to move the player around, scale it, etc.
		gef::Matrix44 player_transform;
		gef::Matrix44 player_scale;
		gef::Matrix44 player_rotate;
		gef::Matrix44 player_translate;


		player_transform.SetIdentity();
		player_scale.SetIdentity();
		player_rotate.SetIdentity();
		player_translate.SetIdentity();

		player_scale.Scale(gef::Vector4(.3f, .3f, .3f, 1.f));
		//player_rotate.RotationY(gef::DegToRad(45.f));
		player_translate.SetTranslation(gef::Vector4(25.f, -25.f, -100.f, 1));

		player_transform = player_scale * player_rotate * player_translate;

		animated_mesh_->set_transform(player_transform);


	}

}

void AnimatedSkinnedMeshInstance::Render(gef::Renderer3D* renderer)
{
	if (animated_mesh_)
	{
		renderer->DrawSkinnedMesh(*animated_mesh_, animated_mesh_->bone_matrices());
	}
}

void AnimatedSkinnedMeshInstance::LoadSkinnedMesh(gef::Platform& platform, const char* scene_file_name)
{
	// create a new scene object and read in the data from the file
	// no meshes or materials are created yet
	// we're not making any assumptions about what the data may be loaded in for
	model_scene_ = new gef::Scene();
	model_scene_->ReadSceneFromFile(platform, scene_file_name);

	// we do want to render the data stored in the scene file so lets create the materials from the material data present in the scene file
	model_scene_->CreateMaterials(platform);

	// if there is mesh data in the scene, create a mesh to draw from the first mesh
	mesh_ = GetFirstMesh(model_scene_, platform);

	// get the first skeleton in the scene
	gef::Skeleton* skeleton = GetFirstSkeleton(model_scene_);

	if (skeleton)
	{
		animated_mesh_ = new gef::SkinnedMeshInstance(*skeleton); // initialise the mesh with skeleton (to be animated)
		animated_mesh_->set_mesh(mesh_); // set the player mesh to the mesh from model_scene_ (tesla for now)
	}
}

gef::Skeleton* AnimatedSkinnedMeshInstance::GetFirstSkeleton(gef::Scene* scene) const
{
	gef::Skeleton* skeleton = nullptr;
	if (scene)
	{
		// check to see if there is a skeleton in the the scene file
		// if so, pull out the bind pose and create an array of matrices
		// that wil be used to store the bone transformations
		if (!scene->skeletons.empty()) {
			skeleton = scene->skeletons.front();
		}
	}

	return skeleton;
}

gef::Mesh* AnimatedSkinnedMeshInstance::GetFirstMesh(gef::Scene* scene, gef::Platform& platform) const
{
	gef::Mesh* mesh = nullptr;
	if (scene)
	{
		// now check to see if there is any mesh data in the file, if so lets create a mesh from it
		if (!scene->mesh_data.empty()) {
			mesh = model_scene_->CreateMesh(platform, scene->mesh_data.front());
		}
	}
	return mesh;
}

gef::SkinnedMeshInstance* AnimatedSkinnedMeshInstance::GetSkinnedMesh() const
{
	return animated_mesh_;
}

AnimationData* AnimatedSkinnedMeshInstance::GetAnimation() const
{
	return animation_;
}










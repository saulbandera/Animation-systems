#include "AnimatedCharacter3D.h"
#include <graphics/scene.h>
#include <graphics/renderer_3d.h>
#include <input/input_manager.h>
#include <input/keyboard.h>
#include "input/touch_input_manager.h"
#include <iostream>
#include "picking.h"
#include <system/platform.h>
#include "ccd.h"

AnimatedCharacter3D::AnimatedCharacter3D(gef::Platform& platform):
animated_mesh_(nullptr),
effector_position_(gef::Vector4::kZero)
{

	LoadSkinnedMesh(platform, "ybot/ybot.scn");

	if (animated_mesh_) {
		anim_manager_ = new AnimationStateManager(platform, animated_mesh_->bind_pose());
	}

	ik_pose_ = anim_manager_->GetAnimController()->GetTree().bind_pose_;

	primitive_builder_ = new PrimitiveBuilder(platform);
	primitive_renderer_ = new PrimitiveRenderer(platform);

}

AnimatedCharacter3D::~AnimatedCharacter3D()
{
}


void AnimatedCharacter3D::Update(float delta_time)
{

	if (animated_mesh_)
	{
		anim_manager_->Update(delta_time, animated_mesh_);
		gef::Matrix44 player_transform;
		gef::Matrix44 player_scale;
		gef::Matrix44 player_rotate;
		gef::Matrix44 player_translate;


		player_transform.SetIdentity();
		player_scale.SetIdentity();
		player_rotate.SetIdentity();
		player_translate.SetIdentity();

		//player_scale.Scale(gef::Vector4(.3f, .3f, .3f, 1.f));
		//player_rotate.RotationY(gef::DegToRad(45.f));
		//player_translate.SetTranslation(gef::Vector4(25.f, -25.f, 0.f, 1));

		player_transform = player_scale * player_rotate * player_translate;

		animated_mesh_->set_transform(player_transform);


	}


}

void AnimatedCharacter3D::HandleInput(gef::InputManager* input_manager, gef::Renderer3D* renderer, gef::Platform& platform)
{
	// read input devices
	if (input_manager)
	{

		// keyboard input
		gef::Keyboard* keyboard = input_manager->keyboard();
		if (keyboard)
		{
			if (!anim_manager_->IsInTransition()) {

				// transition to happy 2D blend
				if (keyboard->IsKeyDown(keyboard->KC_2) && anim_manager_->GetCurrentState() != AnimationStateManager::STANDARD_TO_HAPPY_2D_BLEND && !anim_manager_->ToTransition())
				{
					anim_manager_->SetPreviousState(anim_manager_->GetCurrentState());
					anim_manager_->SetCurrentState(AnimationStateManager::TRANSITION_TO_2D_BLEND);

					anim_manager_->GetAnimController()->SetSpeed(0.0f);
					anim_manager_->GetAnimController()->SetHappiness(0.0f);

					anim_manager_->SetToTransition(true);
				}

				// changing 2D blend values
				if (keyboard->IsKeyDown(keyboard->KC_UP))
					anim_manager_->GetAnimController()->SetSpeed(anim_manager_->GetAnimController()->GetSpeed() >= 1.0f ? 1.0f : anim_manager_->GetAnimController()->GetSpeed() + 0.01f);
				else if (keyboard->IsKeyDown(keyboard->KC_DOWN))
					anim_manager_->GetAnimController()->SetSpeed(anim_manager_->GetAnimController()->GetSpeed() <= 0.0f ? 0.0f : anim_manager_->GetAnimController()->GetSpeed() - 0.01f);
				if (keyboard->IsKeyDown(keyboard->KC_RIGHT))
					anim_manager_->GetAnimController()->SetHappiness(anim_manager_->GetAnimController()->GetHappiness() >= 1.0f ? 1.0f : anim_manager_->GetAnimController()->GetHappiness() + 0.01f);
				else if (keyboard->IsKeyDown(keyboard->KC_LEFT))
					anim_manager_->GetAnimController()->SetHappiness(anim_manager_->GetAnimController()->GetHappiness() <= 0.0f ? 0.0f : anim_manager_->GetAnimController()->GetHappiness() - 0.01f);


				// transition to wavy dance
				if (keyboard->IsKeyDown(keyboard->KC_T) && anim_manager_->GetCurrentState() != AnimationStateManager::WAVY_DANCE && !anim_manager_->ToTransition())
				{

					anim_manager_->SetPreviousState(anim_manager_->GetCurrentState());
					anim_manager_->SetCurrentState(AnimationStateManager::TRANSITION_TO_WAVY_DANCE);

					anim_manager_->SetToTransition(true);

				}

				// transition to silly dance
				if (keyboard->IsKeyDown(keyboard->KC_Y) && anim_manager_->GetCurrentState() != AnimationStateManager::SILLY_DANCE && !anim_manager_->ToTransition())
				{

					anim_manager_->SetPreviousState(anim_manager_->GetCurrentState());
					anim_manager_->SetCurrentState(AnimationStateManager::TRANSITION_TO_SILLY_DANCE);

					anim_manager_->SetToTransition(true);
				}

				// transition to run
				if (keyboard->IsKeyDown(keyboard->KC_R) && anim_manager_->GetCurrentState() != AnimationStateManager::RUNNING && !anim_manager_->ToTransition())
				{

					anim_manager_->SetPreviousState(anim_manager_->GetCurrentState());
					anim_manager_->SetCurrentState(AnimationStateManager::TRANSITION_TO_RUN);

					anim_manager_->SetToTransition(true);
				}

				// switch to inverse kinematics
				if(keyboard->IsKeyDown(keyboard->KC_I) && anim_manager_->GetCurrentState() != AnimationStateManager::INVERSE_KINEMATICS && !anim_manager_->ToTransition())
				{
					anim_manager_->SetPreviousState(anim_manager_->GetCurrentState());
					anim_manager_->SetCurrentState(AnimationStateManager::INVERSE_KINEMATICS);

					anim_manager_->SetToTransition(true);


				}
				// if current state is inverse kinematics
				if(anim_manager_->GetCurrentState() == anim_manager_->INVERSE_KINEMATICS)
				{

					// Use ik_pose which has been initialized to bindpose 
					animated_mesh_->UpdateBoneMatrices(ik_pose_);

					// Get mouse position on screen
					const gef::Vector2 mouse_position = input_manager->touch_manager()->mouse_position();

					const bool mb_down = input_manager->touch_manager()->is_button_down(1);

					// if mouse clicked
					if (mb_down)
					{
						gef::Vector4 mouse_ray_start_point, mouse_ray_direction;

						GetScreenPosRay(mouse_position, renderer->projection_matrix(), renderer->view_matrix(), mouse_ray_start_point, mouse_ray_direction, (float)platform.width(), (float)platform.height(), ndc_zmin_);

						if (RayPlaneIntersect(mouse_ray_start_point, mouse_ray_direction, gef::Vector4(0.0f, 0.0f, 0.0f), gef::Vector4(0.0f, 0.0f, 1.0f), effector_position_))
						{
							std::vector<int> bone_indices;
							bone_indices.push_back(16); // left shoulder
							bone_indices.push_back(17); // left elbow
							bone_indices.push_back(18); // left wrist

							// Perform CCD algorithm
							CalculateCCD(ik_pose_, *animated_mesh_, effector_position_, bone_indices);

							
						}
					}
				}
				
			}

		}
	}
}






void AnimatedCharacter3D::Render(gef::Renderer3D* renderer)
{

	// render the animated 3D mesh
	if (animated_mesh_)
	{
		renderer->DrawSkinnedMesh(*animated_mesh_, animated_mesh_->bone_matrices());
	}

	// render end effector 
	if(anim_manager_->GetCurrentState() == anim_manager_->INVERSE_KINEMATICS)
	{
		primitive_renderer_->Reset();

		RenderEndEffector();

		primitive_renderer_->Render(*renderer);
	}
}

void AnimatedCharacter3D::LoadSkinnedMesh(gef::Platform& platform, const char* scene_file_name)
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
		animated_mesh_->set_mesh(mesh_);
	}
}


void AnimatedCharacter3D::RenderEndEffector()
{
	const float effector_half_line_length = 20.0f;
	const gef::Colour effector_colour(0.0f, 1.0f, 0.0f);

	primitive_renderer_->AddLine(effector_position_ - gef::Vector4(-effector_half_line_length, 0.0f, 0.0f), effector_position_ + gef::Vector4(-effector_half_line_length, 0.0f, 0.0f), effector_colour);
	primitive_renderer_->AddLine(effector_position_ - gef::Vector4(0.0f, -effector_half_line_length, 0.0f), effector_position_ + gef::Vector4(0.0f, -effector_half_line_length, 0.0f), effector_colour);
	primitive_renderer_->AddLine(effector_position_ - gef::Vector4(0.0f, 0.0f, -effector_half_line_length), effector_position_ + gef::Vector4(0.0f, 0.0f, -effector_half_line_length), effector_colour);
}

AnimationStateManager* AnimatedCharacter3D::GetAnimManager()
{
	return anim_manager_;
}

gef::Skeleton* AnimatedCharacter3D::GetFirstSkeleton(gef::Scene* scene) const
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

gef::Mesh* AnimatedCharacter3D::GetFirstMesh(gef::Scene* scene, gef::Platform& platform) const
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

gef::SkinnedMeshInstance* AnimatedCharacter3D::GetSkinnedMesh() const
{
	return animated_mesh_;
}





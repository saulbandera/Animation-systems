#include "scene_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/texture.h>
#include <graphics/mesh.h>
#include <graphics/primitive.h>
#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/font.h>
#include <maths/vector2.h>
#include <input/input_manager.h>
#include <input/keyboard.h>
#include <maths/math_utils.h>
#include <graphics/renderer_3d.h>
#include <graphics/scene.h>
#include <iostream>



SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	renderer_3d_(NULL),
	font_(NULL)

{
}


void SceneApp::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	renderer_3d_ = gef::Renderer3D::Create(platform_);
	input_manager_ = gef::InputManager::Create(platform_);

	InitFont();
	SetupCamera();
	SetupLights();


	/*system_3d_ = new AnimationSystem3D();
	system_3d_->Init(platform_);*/
	system_2d_ = new AnimationSystem2D();
	system_2d_->Init(platform_);

}

void SceneApp::CleanUp()
{
	CleanUpFont();

	delete input_manager_;
	input_manager_ = nullptr;

	delete sprite_renderer_;
	sprite_renderer_ = nullptr;

	delete renderer_3d_;
	renderer_3d_ = nullptr;
}



bool SceneApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;

	bool mb_down = false;
	gef::Vector2 mouse_position(gef::Vector2::kZero);


	// read input devices
	if (input_manager_)
	{
		input_manager_->Update();

		const gef::Keyboard* keyboard = input_manager_->keyboard();
		if (keyboard)

		{

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_SPACE))
			{
				is_current_system_2d = !is_current_system_2d;

				if(is_current_system_2d)
				{
					delete system_3d_;
					system_2d_ = new AnimationSystem2D();
					system_2d_->Init(platform_);
				}
				else
				{
					delete system_2d_;
					system_3d_ = new AnimationSystem3D();
					system_3d_->Init(platform_);
				}
			}
		}



	}
	if(is_current_system_2d)
	{
		system_2d_->HandleInput(input_manager_, platform_);
		system_2d_->Update(frame_time);
	}

	else {
		system_3d_->HandleInput(input_manager_, renderer_3d_, platform_);
		system_3d_->Update(frame_time);
	}


	return true;
}



void SceneApp::Render()
{



	SetCameraMatrices();


	// draw meshes here
	renderer_3d_->Begin();

	if (!is_current_system_2d) {
		system_3d_->Render(renderer_3d_);

	}
	renderer_3d_->End();

	// setup the sprite renderer, but don't clear the frame buffer

	// draw 2D sprites here
	sprite_renderer_->Begin(false);
	if(is_current_system_2d)
	{
		system_2d_->Render(sprite_renderer_);
	}

	DrawHUD();
	sprite_renderer_->End();
}

void SceneApp::SetCameraMatrices()
{
	// setup view and projection matrices
	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;
	projection_matrix = platform_.PerspectiveProjectionFov(camera_fov_, (float)platform_.width() / (float)platform_.height(), near_plane_, far_plane_);
	view_matrix.LookAt(camera_eye_, camera_lookat_, camera_up_);

	if (renderer_3d_)
	{
		renderer_3d_->set_projection_matrix(projection_matrix);
		renderer_3d_->set_view_matrix(view_matrix);
	}
}



void SceneApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void SceneApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void SceneApp::DrawHUD()
{
	if (font_)
	{
		font_->RenderText(sprite_renderer_, gef::Vector4(670.0f, 10.0f, -0.9f), 1.0f, 0xff00ff00, gef::TJ_LEFT, "[Space] Switch 2D/3D");

		if (is_current_system_2d)
		{
			system_2d_->DrawHUD(font_, sprite_renderer_);
		}
		else {
			system_3d_->DrawHUD(font_, sprite_renderer_);
		}

	}
}

void SceneApp::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-300.0f, -500.0f, 100.0f));

	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.7f, 0.7f, 0.7f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
}

void SceneApp::SetupCamera()
{

	camera_eye_ = gef::Vector4(0.0f, 100.0f, 300.0f);
	camera_lookat_ = gef::Vector4(0.0f, 100.0f, 0.0f);
	camera_up_ = gef::Vector4(0.0f, 1.0f, 0.0f);
	camera_fov_ = gef::DegToRad(45.0f);
	near_plane_ = 0.01f;
	far_plane_ = 1000.0f;


	SetCameraMatrices();

}










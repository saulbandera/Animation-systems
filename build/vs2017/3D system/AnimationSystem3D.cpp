#include "AnimationSystem3D.h"
#include <graphics/font.h>
#include <string>

AnimationSystem3D::AnimationSystem3D()
{
}

AnimationSystem3D::~AnimationSystem3D()
{
}

void AnimationSystem3D::Init(gef::Platform& platform)
{
	ybot_ = new AnimatedCharacter3D(platform);
}

void AnimationSystem3D::Update(float frame_time)
{
	ybot_->Update(frame_time);
}

void AnimationSystem3D::HandleInput(gef::InputManager* input_manager, gef::Renderer3D* renderer, gef::Platform& platform)
{
	ybot_->HandleInput(input_manager, renderer,platform);
}

void AnimationSystem3D::Render(gef::Renderer3D* renderer)
{
	ybot_->Render(renderer);
}

void AnimationSystem3D::DrawHUD(gef::Font* font, gef::SpriteRenderer* sprite_renderer_)
{
	

	/*font->RenderText(sprite_renderer_, gef::Vector4(50, 10.0f, -0.9f), 2.0f, 0xffffffff, gef::TJ_LEFT, "[2] 2D blend standard to happy");
	font->RenderText(sprite_renderer_, gef::Vector4(50, 60.0f, -0.9f), 2.0f, 0xffffffff, gef::TJ_LEFT, "[R] Run");
	font->RenderText(sprite_renderer_, gef::Vector4(50, 110.0f, -0.9f), 2.0f, 0xffffffff, gef::TJ_LEFT, "[T] Silly Dance");
	font->RenderText(sprite_renderer_, gef::Vector4(50, 160.0f, -0.9f), 2.0f, 0xffffffff, gef::TJ_LEFT, "[Y] Wavy Dance");
	font->RenderText(sprite_renderer_, gef::Vector4(50, 220.0f, -0.9f), 2.0f, 0xffffffff, gef::TJ_LEFT, "[I] Inverse Kinematics");*/

	font->RenderText(sprite_renderer_, gef::Vector4(10.0f, 10.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "[2] 2D blend standard to happy");
	font->RenderText(sprite_renderer_, gef::Vector4(10.0f, 50.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "[R] Run");
	font->RenderText(sprite_renderer_, gef::Vector4(10.0f, 90.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "[T] Silly Dance");
	font->RenderText(sprite_renderer_, gef::Vector4(10.0f, 130.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "[Y] Wavy Dance");
	font->RenderText(sprite_renderer_, gef::Vector4(10.0f, 170.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "[I] Inverse Kinematics");


	if(ybot_->GetAnimManager()->GetCurrentState() == ybot_->GetAnimManager()->SILLY_DANCE)
	{
		const char* state = "silly dance";
		font->RenderText(sprite_renderer_, gef::Vector4(10.0f, 470.0f, -0.9f), 1.0f, 0xff00ff00, gef::TJ_LEFT, "Current state: %s", state);
	}
	else if (ybot_->GetAnimManager()->GetCurrentState() == ybot_->GetAnimManager()->WAVY_DANCE)
	{
		const char* state = "wavy dance";
		font->RenderText(sprite_renderer_, gef::Vector4(10.0f, 470.0f, -0.9f), 1.0f, 0xff00ff00, gef::TJ_LEFT, "Current state: %s", state);
	}
	else if (ybot_->GetAnimManager()->GetCurrentState() == ybot_->GetAnimManager()->RUNNING)
	{
		const char* state = "running";
		font->RenderText(sprite_renderer_, gef::Vector4(10.0f, 470.0f, -0.9f), 1.0f, 0xff00ff00, gef::TJ_LEFT, "Current state: %s", state);
	}
	else if (ybot_->GetAnimManager()->GetCurrentState() == ybot_->GetAnimManager()->INVERSE_KINEMATICS)
	{
		const char* state = "inverse kinematics";
		font->RenderText(sprite_renderer_, gef::Vector4(10.0f, 470.0f, -0.9f), 1.0f, 0xff00ff00, gef::TJ_LEFT, "Current state: %s", state);
	}
	else if (ybot_->GetAnimManager()->GetCurrentState() == ybot_->GetAnimManager()->STANDARD_TO_HAPPY_2D_BLEND)
	{
		const char* state = "standard to happy 2D blend";
		font->RenderText(sprite_renderer_, gef::Vector4(10.0f, 470.0f, -0.9f), 1.0f, 0xff00ff00, gef::TJ_LEFT, "Current state: %s", state);

		const char* instructions = "[left-right arrow keys] control happiness";
		const char* instructions2 = "[top-down arrow keys] control speed";
		font->RenderText(sprite_renderer_, gef::Vector4(10, 430.0f, -0.9f), 1.0f, 0xff00ff00, gef::TJ_LEFT, instructions);
		font->RenderText(sprite_renderer_, gef::Vector4(10, 400.0f, -0.9f), 1.0f, 0xff00ff00, gef::TJ_LEFT, instructions2);

	}




	else if (ybot_->GetAnimManager()->GetCurrentState() == ybot_->GetAnimManager()->TRANSITION_TO_RUN || ybot_->GetAnimManager()->GetCurrentState() == ybot_->GetAnimManager()->WALKING)
	{
		const char* state = "transition to run";
		font->RenderText(sprite_renderer_, gef::Vector4(10.0f, 470.0f, -0.9f), 1.0f, 0xff00ffff, gef::TJ_LEFT, "Current state: %s", state);

	}
	else if (ybot_->GetAnimManager()->GetCurrentState() == ybot_->GetAnimManager()->TRANSITION_TO_2D_BLEND)
	{
		const char* state = "transition to standard to happy 2D blend ";
		font->RenderText(sprite_renderer_, gef::Vector4(10.0f, 470.0f, -0.9f), 1.0f, 0xff00ffff, gef::TJ_LEFT, "Current state: %s", state);
	}
	else if (ybot_->GetAnimManager()->GetCurrentState() == ybot_->GetAnimManager()->TRANSITION_TO_SILLY_DANCE)
	{
		const char* state = "transition to silly dance";
		font->RenderText(sprite_renderer_, gef::Vector4(10.0f, 470.0f, -0.9f), 1.0f, 0xff00ffff, gef::TJ_LEFT, "Current state: %s", state);
	}
	else if (ybot_->GetAnimManager()->GetCurrentState() == ybot_->GetAnimManager()->TRANSITION_TO_WAVY_DANCE)
	{
		const char* state = "transition to wavy dance";
		font->RenderText(sprite_renderer_, gef::Vector4(10.0f, 470.0f, -0.9f), 1.0f, 0xff00ffff, gef::TJ_LEFT, "Current state: %s", state);
	}


}

#include "AnimationSystem2D.h"

AnimationSystem2D::AnimationSystem2D()
{

}

void AnimationSystem2D::Init(gef::Platform& platform)
{
	animated_character_ = new AnimatedCharacter2D();
	animated_character_->Init(platform);
}

void AnimationSystem2D::Update(float frame_time)
{

	animated_character_->Update(frame_time);
}

void AnimationSystem2D::HandleInput(const gef::InputManager* input_manager, gef::Platform& platform)
{
	animated_character_->HandleInput(input_manager, platform);
}

void AnimationSystem2D::Render(gef::SpriteRenderer* renderer)
{
	animated_character_->Render(renderer);
}

void AnimationSystem2D::DrawHUD(gef::Font* font, gef::SpriteRenderer* renderer)
{
	const char* anim = "";
	if (animated_character_->GetCurrentAnimationName() == "boy-attack")
	{
		anim = "boy-attack";
	}
	else if (animated_character_->GetCurrentAnimationName() == "boy-stand")
	{
		anim = "boy-stand";
	}
	else if (animated_character_->GetCurrentAnimationName() == "boy-move")
	{
		anim = "boy-move";
	}
	else if (animated_character_->GetCurrentAnimationName() == "boy-win")
	{
		anim = "boy-win";
	}
	else if (animated_character_->GetCurrentAnimationName() == "boy-fall")
	{
		anim = "boy-fall";
	}
	else if (animated_character_->GetCurrentAnimationName() == "stand")
	{
		anim = "stand";
	}
	else if (animated_character_->GetCurrentAnimationName() == "walk")
	{
		anim = "walk";
	}
	else if (animated_character_->GetCurrentAnimationName() == "jump")
	{
		anim = "jump";
	}
	else if (animated_character_->GetCurrentAnimationName() == "fall")
	{
		anim = "fall";
	}
	font->RenderText(renderer, gef::Vector4(10.0f, 470.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Current animation: %s", anim);

	font->RenderText(renderer, gef::Vector4(10.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "[S] Switch between frame based to skeleton based animation");


	if (animated_character_->IsSkeletonBased())
	{
		font->RenderText(renderer, gef::Vector4(10.0f, 10.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "[1] Stand");
		font->RenderText(renderer, gef::Vector4(10.0f, 50.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "[2] Walk");
		font->RenderText(renderer, gef::Vector4(10.0f, 90.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "[3] Jump");
		font->RenderText(renderer, gef::Vector4(10.0f, 130.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "[4] Fall");
	}
	else
	{
		font->RenderText(renderer, gef::Vector4(10.0f, 10.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "[1] Fall");
		font->RenderText(renderer, gef::Vector4(10.0f, 50.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "[2] Move");
		font->RenderText(renderer, gef::Vector4(10.0f, 90.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "[3] Stand");
		font->RenderText(renderer, gef::Vector4(10.0f, 130.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "[4] Win");
		font->RenderText(renderer, gef::Vector4(10.0f, 170.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "[5] Attack");

	}
}

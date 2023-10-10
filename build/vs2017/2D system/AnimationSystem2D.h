#pragma once
#include "AnimatedCharacter2D.h"
#include "graphics/font.h"
class AnimationSystem2D
{
public:
	AnimationSystem2D();

	void Init(gef::Platform& platform);
	void Update(float frame_time);
	void HandleInput(const gef::InputManager* input_manager, gef::Platform& platform);
	void Render(gef::SpriteRenderer* renderer);
	void DrawHUD(gef::Font* font, gef::SpriteRenderer* renderer);

private:


	AnimatedCharacter2D* animated_character_;

};


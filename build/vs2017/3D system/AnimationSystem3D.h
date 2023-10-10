#pragma once
#include "AnimatedCharacter3D.h"

namespace gef
{
	class Platform;
	class Font;
	class SpriteRenderer;
}

class AnimationSystem3D
{

public:
	AnimationSystem3D();
	~AnimationSystem3D();

	void Init(gef::Platform& platform);
	void Update(float frame_time);
	void HandleInput(gef::InputManager* input_manager, gef::Renderer3D* renderer, gef::Platform& platform);
	void Render(gef::Renderer3D* renderer);
	void DrawHUD(gef::Font* font, gef::SpriteRenderer* sprite_renderer_);

private:

	AnimatedCharacter3D* ybot_; 

};


#include "SubTexture.h"
#include <iostream>

SubTexture::SubTexture() :
	xy(0.f,0.f),
	size(0.f, 0.f),
	frame_xy(0.f, 0.f),
	frame_size(0.f, 0.f),
    subtexture_transform(gef::Matrix33::kIdentity)

{
}

void SubTexture::ReadSubTextureDataFromJSON(const rapidjson::Value& doc)
{
	name = doc["name"].GetString();

	xy.x = doc["x"].GetFloat();
	xy.y = doc["y"].GetFloat();
	size.x = doc["width"].GetFloat();
	size.y = doc["height"].GetFloat();

	if (doc.HasMember("frameX"))
		frame_xy.x = doc["frameX"].GetFloat();
	else
		frame_xy.x = 0;

	if (doc.HasMember("frameY"))
		frame_xy.y = doc["frameY"].GetFloat();
	else
		frame_xy.y = 0;

	if (doc.HasMember("frameWidth"))
		frame_size.x = doc["frameWidth"].GetFloat();
	else
		frame_size.x = doc["width"].GetFloat();

	if (doc.HasMember("frameHeight"))
		frame_size.y = doc["frameHeight"].GetFloat();
	else
		frame_size.y = doc["height"].GetFloat();


	BuildTransform();
}

void SubTexture::BuildTransform()
{

	gef::Matrix33 translation_m = gef::Matrix33::kIdentity;
	gef::Matrix33 scale_m = gef::Matrix33::kIdentity;

	const float sprite_x = size.x * 0.5f - (frame_size.x * 0.5f + frame_xy.x);
	const float sprite_y = size.y * 0.5f - (frame_size.y * 0.5f + frame_xy.y);

	subtexture_transform.SetIdentity();

	translation_m.SetTranslation(gef::Vector2(sprite_x, sprite_y));
	scale_m.Scale(gef::Vector2(size.x, size.y));

	subtexture_transform = scale_m * translation_m;
}
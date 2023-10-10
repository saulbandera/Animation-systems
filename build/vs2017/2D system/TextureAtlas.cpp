#include "TextureAtlas.h"
#include <iostream>

TextureAtlas::TextureAtlas() :
	size(0.f, 0.f)
{
}

void TextureAtlas::ReadTextureAtlasFromJSON(rapidjson::Document& doc)
{
	name = doc["name"].GetString();
	imagePath = doc["imagePath"].GetString();
	size.x = doc["width"].GetFloat();
	size.y = doc["height"].GetFloat();

	const rapidjson::Value& subtextures_ = doc["SubTexture"];
	for (int subtexture_num = 0; subtexture_num < static_cast<int>(subtextures_.Size()); ++subtexture_num)
	{
		auto* subtexture = new SubTexture();
		subtexture->ReadSubTextureDataFromJSON(subtextures_[subtexture_num]);
		subtextures[subtexture->name] = *subtexture;
		delete subtexture;
	}
}
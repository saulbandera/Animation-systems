#pragma once
#include <string>
#include <map>
#include "SubTexture.h"
#include "rapidjson/include/rapidjson/document.h"

class TextureAtlas
{
public:
	TextureAtlas();

	void ReadTextureAtlasFromJSON(rapidjson::Document& doc);

	gef::Vector2 size;

	std::string imagePath;
	std::string name;
	std::string type;

	std::map<std::string, SubTexture> subtextures;
};


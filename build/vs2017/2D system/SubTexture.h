#pragma once
#include <maths/matrix33.h>
#include <string>
#include "rapidjson/include/rapidjson/document.h"

class SubTexture
{
public:
	SubTexture();

	void ReadSubTextureDataFromJSON(const rapidjson::Value&);
	void BuildTransform();


	gef::Vector2 xy;
	gef::Vector2 size;
	gef::Vector2 frame_xy;
	gef::Vector2 frame_size;


	std::string name;
	gef::Matrix33 subtexture_transform;
};


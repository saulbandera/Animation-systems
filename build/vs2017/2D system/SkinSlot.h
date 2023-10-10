#pragma once
#include <maths/matrix33.h>
#include <string>
#include <map>
#include "rapidjson/include/rapidjson/document.h"

class SkinSlot
{
public:
	SkinSlot();

	static std::map<std::string, SkinSlot> ReadSkinSlotDataFromJSON(rapidjson::Document& ske_doc);
	void BuildSpriteOffsetTransform();


	gef::Vector2 xy;

	float rotation;

	std::string name;
	std::string part_name;

	gef::Matrix33 sprite_offset;
};


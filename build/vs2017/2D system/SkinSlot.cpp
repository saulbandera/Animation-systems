#include "SkinSlot.h"
#include <iostream>
#include <maths/math_utils.h>

SkinSlot::SkinSlot() :
	xy(0.f, 0.f),
	rotation(0.f),
	sprite_offset(gef::Matrix33::kIdentity)
{
}

std::map<std::string, SkinSlot> SkinSlot::ReadSkinSlotDataFromJSON(rapidjson::Document& ske_doc)
{
	std::map<std::string, SkinSlot> skin_slots;
	const rapidjson::Value& slots = ske_doc["armature"][0]["skin"][0]["slot"];

	for (int i = 0; i < static_cast<int>(slots.Size()); i++)
	{
		auto* skin_slot = new SkinSlot();

		skin_slot->name = ske_doc["armature"][0]["skin"][0]["slot"][i]["name"].GetString();

		const rapidjson::Value& slot = ske_doc["armature"][0]["skin"][0]["slot"][i]["display"][0];

		skin_slot->part_name = slot["name"].GetString();

		if (slot.HasMember("transform"))
		{
			skin_slot->xy.x = slot["transform"]["x"].GetFloat();
			skin_slot->xy.y = slot["transform"]["y"].GetFloat();

			if (slot["transform"].HasMember("skY"))
				skin_slot->rotation = slot["transform"]["skY"].GetFloat();
		}

		skin_slot->BuildSpriteOffsetTransform();

		skin_slots[skin_slot->name] = *skin_slot;
		delete skin_slot;
	}

	return skin_slots;
}

void SkinSlot::BuildSpriteOffsetTransform()
{

	sprite_offset.SetIdentity();

	gef::Matrix33 translation_m = gef::Matrix33::kIdentity;
	gef::Matrix33 rotation_m = gef::Matrix33::kIdentity;

	translation_m.SetTranslation(gef::Vector2(xy.x, xy.y));
	rotation_m.Rotate(gef::DegToRad(rotation));

	sprite_offset = rotation_m * translation_m;
}
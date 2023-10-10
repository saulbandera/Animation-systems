#pragma once
#include <maths/matrix33.h>
#include <string>
#include <map>

#include "AnimationData2D.h"
#include "rapidjson/include/rapidjson/document.h"

class Bone
{
public:
	Bone();

	static std::map<std::string, Bone> ReadBoneDataFromJSON(rapidjson::Document& ske_doc);
	static void BuildWorldBoneTransforms(std::map<std::string, Bone>* bones);
	void BuildLocalBoneTransform(AnimationData2D* anim_data);

	float LerpAngle(float start_angle, float end_angle, float time);

	int length;

	gef::Vector2 xy;

	float rotation;

	std::string name, parent;

	gef::Matrix33 local_transform_;
	gef::Matrix33 world_transform_;
};


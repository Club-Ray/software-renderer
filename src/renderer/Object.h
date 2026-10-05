#pragma once
#include "Mesh.h"

class Object {
public:
	Mesh mesh_;

	Vec2 scale_ = Vec2(1,1);
	float rotation_ = 0.0f;
	Vec2 position_ = Vec2(0,0);
};

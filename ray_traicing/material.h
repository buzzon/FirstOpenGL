#pragma once
#include "geometry.h"
class material
{
public:
	material(const vec3f& color) : diffuse_color(color) {}
	material() : diffuse_color() {}
	vec3f diffuse_color;
};
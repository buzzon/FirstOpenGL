#pragma once
#include "geometry.h"
class material
{
public:
	material(const vec2f& a, const vec3f& color, const float& spec) : albedo(a), diffuse_color(color), specular_exponent(spec) {}
	material() : albedo(1, 0), diffuse_color(), specular_exponent() {}
	vec2f albedo;
	vec3f diffuse_color;
	float specular_exponent;
};
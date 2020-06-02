#pragma once
#include "geometry.h"
#include "material.h"

class IObject3d
{
public:
	virtual bool intersect(const vec3f& origin, const vec3f norm_direction, float& t0) const = 0;
	virtual material get_material() const = 0;
	virtual vec3f get_normal(vec3f hit) const = 0;
};
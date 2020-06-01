#pragma once
#include "geometry.h"

class sphere
{
public:
	sphere(const vec3f& p, const float& r) : position(p), radius(r) {}
	virtual bool intersect(const vec3f& origin, const vec3f norm_direction, float& t0) const;
private:
	vec3f position;
	float radius;
};
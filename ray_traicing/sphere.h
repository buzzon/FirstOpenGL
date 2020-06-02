#pragma once
#include "IObject3d.h"

class sphere : public IObject3d
{
public:
	sphere(const vec3f& p, const float& r, const material& m) : position(p), radius(r), material(m) {}
	virtual bool intersect(const vec3f& origin, const vec3f norm_direction, float& t0) const;
	virtual material get_material() const;
	virtual vec3f get_normal(vec3f hit) const;
private:
	vec3f position;
	float radius;
	material material;
};
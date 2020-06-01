#include "sphere.h"

// пересекается ли заданный луч длинны length (исходящий из origin в направлении direction) со сферой?
bool sphere::intersect(const vec3f& origin, const vec3f norm_direction, float& length) const
{
	//// вектор исходящий из origin в направлении direction
	//vec3f ray = norm_direction - origin;


    vec3f L = position - origin;
    float tca = L * norm_direction;
    float d2 = L * L - tca * tca;
    if (d2 > radius * radius) return false;
    float thc = sqrtf(radius * radius - d2);
    length = tca - thc;
    float t1 = tca + thc;
    if (length < 0) length = t1;
    if (length < 0) return false;
    return true;
}

material sphere::get_material() const
{
    return material;
}
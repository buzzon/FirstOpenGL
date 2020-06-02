#pragma once
#include "geometry.h"

struct light {
    light(const vec3f& p, const float& i) : position(p), intensity(i) {}
    vec3f position;
    float intensity;
};
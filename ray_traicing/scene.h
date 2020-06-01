#pragma once
#include "geometry.h"

class scene
{
public:
	scene() : background_color(vec3f(0.5f, 0.5f, 0.5f)) {}
	vec3f get_background_color();
private:
	vec3f background_color;
	// список объектов
};


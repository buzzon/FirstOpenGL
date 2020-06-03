#pragma once
#include "GL/glut.h"
#include "sphere.h"
#include "light.h"
#include <vector>
#include <algorithm> 

#define height 600
#define width 800

class engine
{
public:
	static void initGL(int argc, char** argv);  // Инициализация графической библиотеки
	static void display(void);					// Функция отрисовки
private:
	static void build_and_render(GLubyte(&framebuffer)[height][width][3]);	   // Рисование сцены
	static void render(const std::vector<IObject3d*>& composition, const std::vector<light>& lights, GLubyte(&framebuffer)[600][800][3]);
	static void generateTexture();										   // Генерация текстуры

	static vec3f cast_ray(const vec3f &origin, const vec3f norm_direction, const std::vector<IObject3d*>& composition, const std::vector<light>& lights);
	static bool scene_intersect(const vec3f& orig, const vec3f& dir, const std::vector<IObject3d*>& composition, vec3f& hit, vec3f& N, Material& material);
	static vec3f reflect(const vec3f& dir, const vec3f& normal);
};
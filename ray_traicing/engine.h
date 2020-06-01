#pragma once
#include "GL/glut.h"
#include "sphere.h"
#define height 600
#define width 800

class engine
{
public:
	static void initGL(int argc, char** argv);  // Инициализация графической библиотеки
	static void display(void);					// Функция отрисовки
private:
	static void renderScene(GLubyte(&framebuffer)[height][width][3]);	   // Рисование сцены
	static void generateTexture();										   // Генерация текстуры
	static vec3f cast_ray(const vec3f &origin, const vec3f norm_direction, const sphere &sphere);
};
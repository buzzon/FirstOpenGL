#pragma once
#include "GL/glut.h"
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
};
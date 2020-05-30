#include "GL/glut.h"
#include "engine.h"

void display(void)
{
	engine::display();
}

int main(int argc, char** argv)
{
	engine::initGL(argc, argv);						  // Инициализация графической библиотеки
	glutDisplayFunc(display);						  // Установка функции отрисовки
	glutMainLoop();									  // Запуск игрового цикла
	return 0;
}
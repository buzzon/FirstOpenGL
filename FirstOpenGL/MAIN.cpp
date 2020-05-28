#include <windows.h> 
#include "paint.h"
#include <stdlib.h>
#include <stdio.h>
#include <xlocale>

void init(int argc, char** argv);						// инициализация GL
void moving();											// выполняется когда не нужна отрисовка, пересчитывает положение наблюдателя
void display();											// выполняется при необходимости в отрисовке
void reshape(int w, int h);								// выполняется при изменении размеров окна
void keyboard_special(int key, int x, int y);			// выполняется при нажатии спец клавиш
void keyboard_special_up(int key, int x, int y);		// выполняется при отпускании спец клавиш

// Позиция и вращения наблюдателя
GLfloat camera_rotation[2] = { 30.0, 145.0 };
GLfloat speed_rotation[2] = { 0.0, 0.0 };
GLfloat speed_rotation_value = 2;

GLfloat camera_distance = -8;
GLfloat speed_distance = 0;
GLfloat speed_distance_value = 0.05;

// Свет (цвет и позиция)
GLfloat light_diffuse[] = { 1.0, 0.96, 0.83, 1.0 };		// Цвет источника света
GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };		// Положение источника света, если w == 0 то источник света считается направленным

int main(int argc, char** argv)
{
	init(argc, argv);										// Инициализация GL
	glutIdleFunc(moving);									// Устанавливаю функцию отрабатывающую когда все действия выполненны (вращает камеру)
	glutDisplayFunc(display);								// Устанавливаю функцию отрабатывающую когда нужно отрисовать картинку
	glutReshapeFunc(reshape);								// Устанавливаю функцию отрабатывающую когда изменился размер экрана
	glutSpecialFunc(keyboard_special);						// Устанавливаю функцию отрабатывающую когда нажата спец клавиша
	glutSpecialUpFunc(keyboard_special_up);					// Устанавливаю функцию отрабатывающую когда отпущена спец клавиша
	glutMainLoop();											// Запускаю главный игровой цикл
	return 0;
}

void moving()
{
	camera_rotation[0] = camera_rotation[0] + speed_rotation[0];	// Задаю необходимый поворот вертикально
	camera_rotation[1] = camera_rotation[1] + speed_rotation[1];	// Задаю необходимый поворот горизонтально
	camera_distance = camera_distance + speed_distance;				// Задаю дистанцию камеры от нулевых координат

	glutPostRedisplay();											// Перерисовываю кадр
}

void init(int argc, char** argv)
{
	glutInit(&argc, argv);												   // Инициализация GL utility tools
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);			   // Говорю что используется двойная буферизация кадра,
																		   // 1 пиксель 3 байта красный(0-255) зелёный(0-255) синий(0-255)
																		   // включаю тест глубины

	glutInitWindowSize(1600, 900);										   //
	glutInitWindowPosition(160, 60);									   //
	glutCreateWindow("panzer");											   //

	glClearColor(0.2, 0.2, 0.2, 0.0);									   //

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);					   //
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_diffuse);					   //

	glEnable(GL_LIGHTING);												   // Вклучаю освещение
	glEnable(GL_LIGHT0);												   // Включаю нулевой источник света
	glEnable(GL_DEPTH_TEST);											   // Включаю тест глубины
	glEnable(GL_CULL_FACE);												   // Включаю отвечение граней

	GLfloat ambient[] = { 0.5, 0.5, 0.5, 1.0 };							   // Интенсивность фонового освещения
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);					   // Задаю цвет фонового освещения
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Чистю буффер света и глубины

	glPushMatrix();
		glLightfv(GL_LIGHT0, GL_POSITION, light_position); // Ставлю свет на своё законное

		if (camera_distance > 0) glRotatef(180, 0.0, 1.0, 0.0);	   // Размещаю наблюдателя
		glTranslatef(0.0, 0.0, camera_distance);				   // Размещаю наблюдателя
		glRotatef(camera_rotation[0], 1.0, 0.0, 0.0);			   // Размещаю наблюдателя
		glRotatef(camera_rotation[1], 0.0, 1.0, 0.0);			   // Размещаю наблюдателя

		b_engine::draw_marking();								   // Рисую мировую сетку и центр координат
		paint::draw_panzer();									   // И танк, кудаже без него

		glPopMatrix();
	glutSwapBuffers(); // Меняю буыеры местами, показываю новоиспеченный кадр
}

void reshape(const int w, const int h)
{
	glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));						 // Задаю новый размер видимой области
	glMatrixMode(GL_PROJECTION);															 // Выбираю матрицу проекции
	glLoadIdentity();																		 // Загружаю единичную матрицу
	gluPerspective(65.0, static_cast<GLfloat>(w) / static_cast<GLfloat>(h), 0.001, 50.0);	 // Настройка перспективы просмотра (угол обзара, ширина, высота, ближнее отсечение, дальнее отсечение)
	glMatrixMode(GL_MODELVIEW);																 // Загружаю модельную матрицу
	glLoadIdentity();																		 // Востанавливаю её к единичному виду
}

void keyboard_special(const int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		speed_rotation[0] = speed_rotation_value;   break;			// задаю скорость вращения вертикальнро
	case GLUT_KEY_DOWN:
		speed_rotation[0] = -speed_rotation_value;  break;			// задаю скорость вращения вертикальнро
	case GLUT_KEY_LEFT:
		speed_rotation[1] = speed_rotation_value;   break;			// задаю скорость вращения горизонтально
	case GLUT_KEY_RIGHT:
		speed_rotation[1] = -speed_rotation_value;  break;			// задаю скорость вращения горизонтально

	case GLUT_KEY_PAGE_UP:
		speed_distance = speed_distance_value;  break;				// задаю скорость приближения камеры
	case GLUT_KEY_PAGE_DOWN:										
		speed_distance = -speed_distance_value; break;				// задаю скорость отдаления камеры

	default:
		printf("unassigned button" + key);
	}
}

void keyboard_special_up(const int key, int x, int y)
{
	// при отпускании ставлю скорости перемещения на 0
	switch (key)
	{
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
		speed_rotation[0] = 0.0;    break;
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT:
		speed_rotation[1] = 0.0;    break;

	case GLUT_KEY_PAGE_UP:
	case GLUT_KEY_PAGE_DOWN:
		speed_distance = 0;   break;

	default:
		printf("unassigned button" + key);
	}
}
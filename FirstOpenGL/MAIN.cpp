#include <windows.h> 
#include "paint.h"
#include <stdlib.h>
#include <stdio.h>
#include <xlocale>

void init(int argc, char** argv); // ������������� GL
void moving();// ����������� ����� �� ����� ���������, ������������� ��������� �����������
void display(); // ����������� ��� ������������� � ���������
void reshape(int w, int h); // ����������� ��� ��������� �������� ����
void keyboard_special(int key, int x, int y); // ����������� ��� ������� ���� ������
void keyboard_special_up(int key, int x, int y); // ����������� ��� ���������� ���� ������

//camera
GLfloat camera_rotation[2] = { 30.0, 145.0 };
GLfloat speed_rotation[2] = { 0.0, 0.0 };
GLfloat speed_rotation_value = 2;

GLfloat camera_distance = -8;
GLfloat distance = 0;
GLfloat distance_value = 0.05;

GLfloat light_diffuse[] = { 1.0, 0.96, 0.83, 1.0 }; // ���� ��������� �����
GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 }; // ��������� ��������� �����, ���� w == 0 �� �������� ����� ��������� ������������

int main(int argc, char** argv)
{
	init(argc, argv);
	glutIdleFunc(moving);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(keyboard_special);
	glutSpecialUpFunc(keyboard_special_up);
	glutMainLoop();
	return 0;
}

void moving()
{
	camera_rotation[0] = camera_rotation[0] + speed_rotation[0];
	camera_rotation[1] = camera_rotation[1] + speed_rotation[1];
	camera_distance = camera_distance + distance;

	glutPostRedisplay();
}

void init(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(1600, 900);
	glutInitWindowPosition(160, 60);
	glutCreateWindow("panzer");


	glClearColor(0.2, 0.2, 0.2, 0.0);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_diffuse);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	GLfloat ambient[] = { 0.5, 0.5, 0.5, 1.0 }; // ������������� �������� ���������
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	if (camera_distance > 0) glRotatef(180, 0.0, 1.0, 0.0);

	glTranslatef(0.0, 0.0, camera_distance);
	glRotatef(camera_rotation[0], 1.0, 0.0, 0.0);
	glRotatef(camera_rotation[1], 0.0, 1.0, 0.0);

	b_engine::draw_marking();

	paint::draw_panzer();

	glPopMatrix();

	glutSwapBuffers();
}

void reshape(const int w, const int h)
{
	glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, static_cast<GLfloat>(w) / static_cast<GLfloat>(h), 0.001, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard_special(const int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		speed_rotation[0] = speed_rotation_value;   break;
	case GLUT_KEY_DOWN:
		speed_rotation[0] = -speed_rotation_value;  break;
	case GLUT_KEY_LEFT:
		speed_rotation[1] = speed_rotation_value;   break;
	case GLUT_KEY_RIGHT:
		speed_rotation[1] = -speed_rotation_value;  break;

	case GLUT_KEY_PAGE_UP:
		distance = distance_value;  break;
	case GLUT_KEY_PAGE_DOWN:
		distance = -distance_value; break;

	default:
		printf("unassigned button");
	}
}

void keyboard_special_up(const int key, int x, int y)
{
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
		distance = 0;   break;

	default:
		printf("unassigned button");
	}
}
#include "GL/glut.h"
#include "engine.h"

void display(void)
{
	engine::display();
}

int main(int argc, char** argv)
{
	engine::initGL(argc, argv);						  // ������������� ����������� ����������
	glutDisplayFunc(display);						  // ��������� ������� ���������
	glutMainLoop();									  // ������ �������� �����
	return 0;
}
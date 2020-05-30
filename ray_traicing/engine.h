#pragma once
#include "GL/glut.h"
#define height 600
#define width 800

class engine
{
public:
	static void initGL(int argc, char** argv);  // ������������� ����������� ����������
	static void display(void);					// ������� ���������
private:
	static void renderScene(GLubyte(&framebuffer)[height][width][3]);	   // ��������� �����
	static void generateTexture();										   // ��������� ��������
};
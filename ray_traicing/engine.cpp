#include "engine.h"

void engine::initGL(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("ray traicing");

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
}

void engine::display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	generateTexture();											// √енерируютекстуру

	glBegin(GL_QUADS);											// –исую пр€моуголькик заполн€ющий весь экран
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, 0.0);	// к нему примен€ю новоиспеченную текстуру
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 1.0, 0.0);		//
		glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, 0.0);		//
		glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -1.0, 0.0);		//
	glEnd();													//

	glFlush();
}

void engine::generateTexture()
{
	GLuint texName;
	static GLubyte framebuffer[height][width][3];

	renderScene(framebuffer);     // –ендерю картинку и записываю еЄ в framebuffer

	glGenTextures(1, &texName);																		  // √енерирую текстуру и назначаю еЄ активной
	glBindTexture(GL_TEXTURE_2D, texName);															  //
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);								  // 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);								  // 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);  //
}

void engine::renderScene(GLubyte(&framebuffer)[height][width][3])
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			framebuffer[i][j][0] = i / (float)height * 255;
			framebuffer[i][j][1] = j / (float)width * 255;
			framebuffer[i][j][2] = 0;
		}
	}
}
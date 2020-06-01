#include "engine.h"
#include "sphere.h"

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
	glClear(GL_COLOR_BUFFER_BIT);								// „истим экран

	generateTexture();											// √енерируютекстуру

	glBegin(GL_QUADS);											// –исую пр€моуголькик заполн€ющий весь экран
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, 0.0);	// к нему примен€ю новоиспеченную текстуру
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 1.0, 0.0);		//
		glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, 0.0);		//
		glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -1.0, 0.0);		//
	glEnd();													//

	glFlush();													// ”буждаемс€ что буфер выведен
}

void engine::generateTexture()
{
	GLuint texName;
	static unsigned char framebuffer[height][width][3];

	renderScene(framebuffer);     // –ендерю картинку и записываю еЄ в framebuffer

	glGenTextures(1, &texName);																		  // √енерирую текстуру и назначаю еЄ активной
	glBindTexture(GL_TEXTURE_2D, texName);															  //
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);								  // 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);								  // 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);  //
}

vec3f engine::cast_ray(const vec3f& origin, const vec3f norm_direction, const sphere& sphere)
{
	float sphere_dist = std::numeric_limits<float>::max();
	if (!sphere.intersect(vec3f(0, 0, 0), norm_direction, sphere_dist))
		return vec3f(0.2, 0.2, 0.2); //get_background_color();
	return vec3f(0.5, 1.0, 0.5);
}

void engine::renderScene(GLubyte(&framebuffer)[height][width][3])
{
	sphere sphere(vec3f(-3, 0, -16), 2);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			float fov = 1;
			float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * width / (float)height;
			float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
			vec3f dir = vec3f(x, y, -1).normalize();

			vec3f color = cast_ray(vec3f(0, 0, 0), dir, sphere);

			framebuffer[i][j][0] = color[0] * 255;
			framebuffer[i][j][1] = color[1] * 255;
			framebuffer[i][j][2] = color[2] * 255;
		}
	}
}
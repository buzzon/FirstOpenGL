#include "GL/glut.h"
#include "geometry.h"
#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

const int width = 1024;
const int height = 768;
const int fov = 45;

GLubyte framebuffer[height][width][3];
GLuint texName;

struct Sphere {
	Vec3f center;
	float radius;

	Sphere(const Vec3f& c, const float& r) : center(c), radius(r) {}

	bool ray_intersect(const Vec3f& orig, const Vec3f& dir, float& t0) const {
		Vec3f L = center - orig;
		float tca = L * dir;
		float d2 = L * L - tca * tca;
		if (d2 > radius * radius) return false;
		float thc = sqrtf(radius * radius - d2);
		t0 = tca - thc;
		float t1 = tca + thc;
		if (t0 < 0) t0 = t1;
		if (t0 < 0) return false;
		return true;
	}
};

Vec3f cast_ray(const Vec3f& orig, const Vec3f& dir, const Sphere& sphere) {
	float sphere_dist = std::numeric_limits<float>::max();
	if (!sphere.ray_intersect(orig, dir, sphere_dist)) {
		return Vec3f(0.2, 0.7, 0.8); // background color
	}

	return Vec3f(0.4, 0.4, 0.3);
}

void ray_traicing(const Sphere& sphere)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * width / (float)height;
			float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
			Vec3f dir = Vec3f(x, y, -1).normalize();

			auto cast = cast_ray(Vec3f(0, 0, 0), dir, sphere);

			framebuffer[i][j][0] = cast.x * 255;
			framebuffer[i][j][1] = cast.y * 255;
			framebuffer[i][j][2] = cast.z * 255;
		}
	}
}

void gradient() 
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			framebuffer[i][j][0] = (i / (float)height) * 255;
			framebuffer[i][j][1] = (j / (float)width) * 255;
			framebuffer[i][j][2] = 0;
		}
	}
}

void renderScene()
{
	Sphere sphere(Vec3f(0, 0, -10), 1);
	ray_traicing(sphere);
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	renderScene();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, texName);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0,  1.0, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f( 1.0,  1.0, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f( 1.0, -1.0, 0.0);
	glEnd();

	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("ray traicing");

	init();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
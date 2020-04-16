#pragma once
#include <cmath>
#include <stdio.h>
#include <GL/glut.h>

#define PI 3.14159265 

class b_engine
{
public:
	static void draw_triangle(float* v1, float* v2, float* v3);
	static void draw_triangle2(float* v1, float* v2, float* v3);
	static void draw_triangle_sim_x(float* v1, float* v2, float* v3);
	static void draw_triangle_subdivide(float* v1, float* v2, float* v3, int depth);
	static void normalize(float v[3]);
	static void normcrossprod(const float v1[3], const float v2[3], float out[3]); 	// вычисляет нормализованное векторное произведение двух векторов
	static void get_vectors(GLfloat foot[][3], GLfloat d1[3], GLfloat d2[3]); // вычисляет 2 вектора лежащих на плоскости

	static void draw_icosahedron(int subdivide);
	static void draw_cylinder(int vertices, float radius_up, float radius_down, float depth);
	static void draw_light_source(GLenum light, GLfloat pos[4], GLfloat color[3]);
	static void draw_marking();
};


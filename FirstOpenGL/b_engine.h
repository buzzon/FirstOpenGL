#pragma once
#include <cmath>
#include <stdio.h>
#include <GL/glut.h>


class b_engine
{
public:
	static void draw_triangle(float* v1, float* v2, float* v3);
	static void draw_triangle_subdivide(float* v1, float* v2, float* v3, int depth);
	static void normalize(float v[3]);
	static void normcrossprod(const float v1[3], const float v2[3], float out[3]); 	// вычисляет нормализованное векторное произведение двух векторов

	static void draw_icosahedron(int subdivide);
};


#pragma once
#include <cmath>
#include <xlocale>
#include <stdlib.h>
#include <windows.h> 
#include <GL/glut.h>

#define PI 3.14159265 

class b_engine
{
public:
	// рисует простейший полигон с нормалью 
	static void draw_triangle(float* v1, float* v2, float* v3);
	// рисует два полигона с нормалями симметрично по x оси
	static void draw_triangle_sim_x(float* v1, float* v2, float* v3);
	// рисует объект
	static void draw_object(GLint  triangles[][3], int triangles_count, GLfloat  vertexs[][3]);

	// нормалицвзия вектора
	static void normalize(float v[3]);
	// вычисляет нормализованное векторное произведение двух векторов
	static void normcrossprod(const float v1[3], const float v2[3], float out[3]); 	
	// вычисляет нормаль полигона
	static void get_normal(float* v1, float* v2, float* v3, float out[3]);

	/// <summary>
	/// Рисует цилиндр
	/// </summary>
	/// <param name="vertices">Кол-во точек в основании цилиндра</param>
	/// <param name="radius_up">радиус верхнего основания</param>
	/// <param name="radius_down">радиус нижнего основания</param>
	/// <param name="depth">высота</param>
	static void draw_cylinder(int vertices, float radius_up, float radius_down, float depth);

	// Рисует базовую разметку невоспреимчивую к свету
	static void draw_marking();
	// Рисует стержень в центре мира с цветным обозначением направлений осей
	static void draw_pivot();
	/// <summary>
	/// Рисует горизонтальную сетку в нулевых координатах
	/// </summary>
	/// <param name="count_lines">количество линий по оси</param>
	static void draw_grid(int count_lines);
};
#pragma once
#include <cmath>
#include <stdio.h>
#include <GL/glut.h>

#define PI 3.14159265 

class b_engine
{
public:
	// ������ ���������� ������� � �������� 
	static void draw_triangle(float* v1, float* v2, float* v3);
	// ������ ��� �������� � ��������� ����������� �� x ���
	static void draw_triangle_sim_x(float* v1, float* v2, float* v3);

	// ������������ �������
	static void normalize(float v[3]);
	// ��������� ��������������� ��������� ������������ ���� ��������
	static void normcrossprod(const float v1[3], const float v2[3], float out[3]); 	

	/// <summary>
	/// ������ �������
	/// </summary>
	/// <param name="vertices">���-�� ����� � ��������� ��������</param>
	/// <param name="radius_up">������ �������� ���������</param>
	/// <param name="radius_down">������ ������� ���������</param>
	/// <param name="depth">������</param>
	static void draw_cylinder(int vertices, float radius_up, float radius_down, float depth);

	// ������ ������� �������� ��������������� � �����
	static void draw_marking();
	// ������ �������� � ������ ���� � ������� ������������ ����������� ����
	static void draw_pivot();
	/// <summary>
	/// ������ �������������� ����� � ������� �����������
	/// </summary>
	/// <param name="count_lines">���������� ����� �� ���</param>
	static void draw_grid(int count_lines);
};
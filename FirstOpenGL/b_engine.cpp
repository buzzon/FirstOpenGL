#include "b_engine.h"

void b_engine::get_normal(float* v1, float* v2, float* v3, float out[3])
{
	GLfloat d1[3], d2[3];

	for (int i = 0; i < 3; ++i)
	{
		d1[i] = v1[i] - v2[i];
		d2[i] = v2[i] - v3[i];
	}

	normcrossprod(d1, d2, out);
}

void b_engine::draw_triangle(float* v1, float* v2, float* v3)
{
	GLfloat norm[3];
	get_normal(v1, v2, v3, norm);

	glBegin(GL_TRIANGLES);
		glNormal3fv(norm);
		glVertex3fv(v1);
		glVertex3fv(v2);
		glVertex3fv(v3);
	glEnd();
}

void b_engine::draw_triangle_sim_x(float* v1, float* v2, float* v3)
{
	GLfloat norm[3];
	get_normal(v1, v2, v3, norm);

	glPushMatrix();
	
	glBegin(GL_TRIANGLES);
		glNormal3fv(norm);
		glVertex3fv(v1);
		glVertex3fv(v2);
		glVertex3fv(v3);
	glEnd();
	
	glScalef(-1, 1, 1);
	
	glBegin(GL_TRIANGLES);
		glNormal3fv(norm);
		glVertex3fv(v1);
		glVertex3fv(v3);
		glVertex3fv(v2);
	glEnd();

	glPopMatrix();
}

void b_engine::draw_object(GLint triangles[][3], int triangles_count, GLfloat vertexs[][3])
{
	for (int i = 0; i < triangles_count; i++)
		b_engine::draw_triangle(&vertexs[triangles[i][0]][0], &vertexs[triangles[i][1]][0], &vertexs[triangles[i][2]][0]);
}

void b_engine::normalize(float out[3])
{
	const auto d = std::sqrt(out[0] * out[0] + out[1] * out[1] + out[2] * out[2]);
	if (d == 0.0) {
		printf("b_engine::normalize\t\t| The length of the vector is 0\n");
		return;
	}
	out[0] /= d;
	out[1] /= d;
	out[2] /= d;
}

void b_engine::normcrossprod(const float v1[3], const float v2[3], float out[3])
{
	out[0] = v1[1] * v2[2] - v1[2] * v2[1];
	out[1] = v1[2] * v2[0] - v1[0] * v2[2];
	out[2] = v1[0] * v2[1] - v1[1] * v2[0];
	normalize(out);
}

void b_engine::draw_cylinder(const int vertices, const float radius_up, const float radius_down, float depth)
{
	const auto array_up = new float* [vertices];
	const auto array_down = new float* [vertices];
	
	for (auto i = 0; i < vertices; ++i)
	{
		array_up[i] = new float[3];
		array_down[i] = new float[3];
	}

	float middle_up[3] = { 0.0, depth / 2, 0.0 };
	float middle_down[3] = { 0.0, -depth / 2, 0.0 };
	
	for (int i = 0; i < vertices; ++i)
	{
		const auto rad = static_cast<float>(i) / 180 * 3.14 * 360 / vertices;
		const float x = cos(rad);
		const float z = sin(rad);
		
		array_up[i][0] = radius_up * x;
		array_up[i][1] = middle_up[1];
		array_up[i][2] = radius_up * z;

		array_down[i][0] = radius_down * x;
		array_down[i][1] = middle_down[1];
		array_down[i][2] = radius_down * z;
	}
	
	for (int i = 0; i < vertices; ++i)
		draw_triangle(middle_up, &array_up[(i + 1) % vertices][0], &array_up[i][0]);

	for (int i = 0; i < vertices; ++i)
		draw_triangle(middle_down, &array_down[i][0], &array_down[(i + 1) % vertices][0]);

	for (int i = 0; i < vertices; ++i)
	{
		draw_triangle(&array_up[i % vertices][0], &array_up[(i + 1) % vertices][0], &array_down[i % vertices][0]);
		draw_triangle(&array_up[(i + 1) % vertices][0], &array_down[(i + 1) % vertices][0], &array_down[i % vertices][0]);
	}
}

void b_engine::draw_marking()
{
	glDisable(GL_LIGHTING);

	draw_pivot();
	draw_grid(20);

	glEnable(GL_LIGHTING);
}

void b_engine::draw_pivot()
{
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(0.05, 8, 8);

	glLineWidth(3);

	// X
	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);
	glEnd();

	// Y
	glBegin(GL_LINES);
		glColor3f(0, 1, 0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 1.0, 0.0);
	glEnd();

	// Z
	glBegin(GL_LINES);
		glColor3f(0, 0, 1);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 1.0);
	glEnd();
}

void b_engine::draw_grid(int count_lines)
{
	count_lines = count_lines / 2;
	glLineWidth(1);

	glColor3f(0.5, 0.5, 0.5);

	for (int i = -count_lines; i <= count_lines; ++i)
	{
		glBegin(GL_LINES);
		glVertex3f(-count_lines, 0.0, i);
		glVertex3f(count_lines, 0.0, i);
		glEnd();
	}

	for (int i = -count_lines; i <= count_lines; ++i)
	{
		glBegin(GL_LINES);
		glVertex3f(i, 0.0, -count_lines);
		glVertex3f(i, 0.0, count_lines);
		glEnd();
	}
}
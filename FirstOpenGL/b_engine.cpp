#include "b_engine.h"

void b_engine::draw_triangle(float* v1, float* v2, float* v3)
{
	GLfloat d1[3], d2[3], norm[3];

	for (int i = 0; i < 3; ++i)
	{
		d1[i] = v1[i] - v2[i];
		d2[i] = v2[i] - v3[i];
	}

	normcrossprod(d1, d2, norm);

	glBegin(GL_TRIANGLES);
		glNormal3fv(norm);
		glVertex3fv(v1);
		glVertex3fv(v2);
		glVertex3fv(v3);
	glEnd();
}

void b_engine::draw_triangle2(float* v1, float* v2, float* v3)
{
	GLfloat d1[3], d2[3], norm[3];

	for (int i = 0; i < 3; ++i)
	{
		d1[i] = v1[i] - v2[i];
		d2[i] = v2[i] - v3[i];
	}

	normcrossprod(d1, d2, norm);

	glBegin(GL_TRIANGLES);
	glNormal3fv(norm);
	glVertex3fv(v1);
	glVertex3fv(v3);
	glVertex3fv(v2);
	glEnd();
}

void b_engine::draw_triangle_sim_x(float* v1, float* v2, float* v3)
{
	GLfloat d1[3], d2[3], norm[3];

	for (int i = 0; i < 3; ++i)
	{
		d1[i] = v1[i] - v2[i];
		d2[i] = v2[i] - v3[i];
	}

	normcrossprod(d1, d2, norm);

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

void b_engine::draw_triangle_subdivide(float* v1, float* v2, float* v3, const int depth)
{
	if (depth == 0)
	{
		draw_triangle(v1, v2, v3);
		return;
	}

	GLfloat v12[3], v23[3], v31[3];

	for (GLint i = 0; i < 3; ++i)
	{
		v12[i] = (v1[i] + v2[i]) / 2.0;
		v23[i] = (v2[i] + v3[i]) / 2.0;
		v31[i] = (v3[i] + v1[i]) / 2.0;
	}

	normalize(v12);
	normalize(v23);
	normalize(v31);

	draw_triangle_subdivide(v1, v12, v31, depth - 1);
	draw_triangle_subdivide(v2, v23, v12, depth - 1);
	draw_triangle_subdivide(v3, v31, v23, depth - 1);
	draw_triangle_subdivide(v12, v23, v31, depth - 1);
}

void b_engine::normalize(float v[3])
{
	const auto d = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	if (d == 0.0) {
		printf("b_engine::normalize | The length of the vector is 0\n");
		return;
	}
	v[0] /= d;
	v[1] /= d;
	v[2] /= d;
}

void b_engine::normcrossprod(const float v1[3], const float v2[3], float out[3])
{
	out[0] = v1[1] * v2[2] - v1[2] * v2[1];
	out[1] = v1[2] * v2[0] - v1[0] * v2[2];
	out[2] = v1[0] * v2[1] - v1[1] * v2[0];
	normalize(out);
}

void b_engine::get_vectors(GLfloat foot[][3], GLfloat d1[3], GLfloat d2[3])
{
	for (int i = 0; i < 3; ++i)
	{
		d1[i] = foot[0][i] - foot[1][i];
		d2[i] = foot[1][i] - foot[2][i];
	}
}

void b_engine::draw_icosahedron(const int subdivide)
{
#define X .525731112119133606
#define Z .850650808352039932

	GLfloat vdata[12][3] = {
	 {-X,0.0,Z},{X,0.0,Z},{-X,0.0,-Z},{X,0.0,-Z},
	 {0.0,Z,X},{0.0,Z,-X},{0.0,-Z,X},{0.0,-Z,-X},
	 {Z,X,0.0},{-Z,X,0.0},{Z,-X,0.0},{-Z,-X,0.0}
	};

	GLuint tindices[20][3] = {
	 {1,4,0},{4,9,0},{4,5,9},{8,5,4},{1,8,4},
	 {1,10,8},{10,3,8},{8,3,5},{3,2,5},{3,7,2},
	 {3,10,7},{10,6,7},{6,11,7},{6,0,11},{6,1,0},
	 {10,1,6},{11,0,9},{2,11,9},{5,2,9},{11,2,7}
	};

	for (GLint i = 0; i < 20; ++i)
		draw_triangle_subdivide(&vdata[tindices[i][0]][0], &vdata[tindices[i][1]][0], &vdata[tindices[i][2]][0], subdivide);
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

void b_engine::draw_light_source(const GLenum light, GLfloat pos[4], GLfloat color[3])
{
	glPushMatrix();
	glLightfv(light, GL_POSITION, pos);
	glTranslated(pos[0], pos[1], pos[2]);
	glDisable(GL_LIGHTING);
	glColor3f(color[0], color[1], color[2]);
	glutWireCube(0.1);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void b_engine::draw_marking()
{
	glutSolidSphere(0.05, 8, 8);
	glDisable(GL_LIGHTING);

	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0, 1, 0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 1.0);
	glEnd();
	
	glLineWidth(1);
	glColor3f(0.5, 0.5, 0.5);
	for (int i = -10; i < 11; ++i)
	{
		glBegin(GL_LINES);
		glVertex3f(-10.0, 0.0, i);
		glVertex3f(10.0, 0.0, i);
		glEnd();
	}
	
	for (int i = -10; i < 11; ++i)
	{
		glBegin(GL_LINES);
		glVertex3f(i, 0.0, -10);
		glVertex3f(i, 0.0, 10);
		glEnd();
	}

	glEnable(GL_LIGHTING);
}
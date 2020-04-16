#include <windows.h> 
#include "b_engine.h"
#include <stdlib.h>
#include <stdio.h>
#include <xlocale>

void init();
void moving();
void display();
void reshape(int w, int h);
void keyboard_special(int key, int x, int y);
void keyboard_special_up(int key, int x, int y);

//camera
GLfloat camera_rotation[2] = { 30.0, 145.0 };
GLfloat speed_rotation[2] = { 0.0, 0.0 };
GLfloat speed_rotation_value = 2;

GLfloat camera_distance = -8;
GLfloat distance = 0;
GLfloat distance_value = 0.05;

GLfloat light_diffuse[] = { 1.0, 0.96, 0.83, 1.0 };
GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 }; // Положение источника света w == 0, источник света считается направленным

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(1600, 900);
	glutInitWindowPosition(160, 60);
	glutCreateWindow("panzer");
	init();
	glutIdleFunc(moving);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(keyboard_special);
	glutSpecialUpFunc(keyboard_special_up);
	glutMainLoop();
	return 0;
}

void moving()
{
	camera_rotation[0] = camera_rotation[0] + speed_rotation[0];
	camera_rotation[1] = camera_rotation[1] + speed_rotation[1];
	camera_distance = camera_distance + distance;

	glutPostRedisplay();
}

void init()
{
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glClearColor(0.2, 0.2, 0.2, 0.0);
	glShadeModel(GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_diffuse);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 2.0);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 1.0);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.5);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	GLfloat ambient[] = { 0.5, 0.5, 0.5, 1.0 }; // Интенсивность фонового освещения
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

}

void draw_rocket_launcher()
{
	glPushMatrix();
		glutSolidCube(0.4);
		glTranslatef(0.15, 0.10, -0.2);
		glRotatef(90, 1, 0, 0);
		for (int i = 0; i < 4;++i)
		{
			b_engine::draw_cylinder(8, 0.05, 0.02, 0.12);
			glTranslatef(-0.1, 0, 0);
		}
	glPopMatrix();
}

void draw_turret()
{
	GLfloat turret[24][3] =
	{
		// крышка
		{0.0, 0.6, -0.9},	// 0
		{-0.5, 0.6, -0.9},
		{-0.6, 0.6, -0.5},
		{-0.8, 0.6, -0.3},
		{-0.8, 0.6, 0.3},
		{-0.6, 0.6, 0.5},
		{-0.4, 0.6, 2.6},
		{ 0.0, 0.6, 2.6},	// 7

		// фасад
		{ 0.0, 0.2, -1.3}, // 8
		{ -0.6, 0.2, -1.3},
		{ -0.8, 0.2, -1.0},
		{ -1.5, 0.2, -0.5},
		{ -1.5, 0.2, 0.5},
		{ -0.8, 0.2, 0.7},
		{ 0.0, 0.2, 1.2},
		{ -0.6, 0.3, 2.9},
		{ 0.0, 0.3, 2.9},	// 15

		// дно
		{ 0.0, 0.0, -1.2},	// 17
		{ -0.5, 0.0, -1.2},
		{ -0.7, 0.0, -0.9},
		{ -1.4, 0.0, -0.4},
		{ -1.4, 0.0, 0.4},
		{ -0.7, 0.0, 0.6},
		{ 0.0, 0.0, 1.1}	//24

	};

	GLint turret_triangles[40][3] =
	{
		// крышка
		{0,1,2},
		{0,2,3},
		{0,3,4},
		{0,4,5},
		{0,5,6},
		{0,6,7},

		//фасад верх
		{8,1,0},
		{8,9,1},
		{9,10,1},
		{10,2,1},
		{10,3,2},
		{10,11,3},
		{11,12,3},
		{12,4,3},
		{12,13,4},
		{13,5,4},
		{13,6,5},
		{13,15,6},
		{15,16,6},
		{16,7,6},

		//фасад верх-низ
		{13,14,15},
		{14,16,15},

		//фасад низ
		{17,18,8},
		{8,18,9},
		{9,18,10},
		{18,19,10},
		{19,10,9},
		{19,20,10},
		{10,20,11},
		{11,20,21},
		{21,12,11},
		{12,21,13},
		{13,21,22},
		{13,22,23},
		{13,23,14},

		//дно
		{17,19,18},
		{17,20,19},
		{17,21,20},
		{17,22,21},
		{17,23,22}
	};

	for (auto& triangle : turret_triangles)
		b_engine::draw_triangle_sim_x(&turret[triangle[0]][0], &turret[triangle[1]][0], &turret[triangle[2]][0]);

	//какой-то обвес на крыше
	glPushMatrix();
		glTranslatef(0.3, 0.7, 0);
		b_engine::draw_cylinder(16, 0.2, 0.3, 0.2);
	glPopMatrix();


	// ракетница 1
	glPushMatrix();
		glTranslatef(1, 0.32, 0);
		glRotatef(-30, 0,1, 1 );
		glRotatef(30, 1, 0, 0);
		draw_rocket_launcher();
	glPopMatrix();
	
	// ракетница 2
	glPushMatrix();
	glTranslatef(-1, 0.32, 0);
	glRotatef(30, 0, 1, 1);
	glRotatef(30, 1, 0, 0);
	draw_rocket_launcher();
	glPopMatrix();
}

void draw_hatch()
{
	glPushMatrix();
		b_engine::draw_cylinder(16, 0.5, 0.5, 0.1);
		glTranslatef(0, 0.075, 0);
		b_engine::draw_cylinder(16, 0.4, 0.5, 0.05);
		b_engine::draw_cylinder(16, 0.1, 0.5, 0.1);
	glPopMatrix();
}

void draw_base()
{
	GLfloat base[25][3] =
	{
		//крышка
		{0.0,0.0,-3.2},	// 0
		{-2.2,0.0,-3.2},
		{-2.2,0.2,-1.4},
		{-2.1,0.0,-1.2},
		{-2.0,0.0,3.4},
		{0.0,0.0,3.4},
		{0.0,0.0,-1.2},
		{0.0,0.2,-1.4}, // 7

		// фасад
		{0.0,-0.5,-3.4}, // 8
		{-1.3,-0.5,-3.4},
		{-2.2,-0.4,-3.4},
		{-2.3,-0.4,-3.4},
		{-2.3,-0.2,-1.3},
		{-2.3,-0.2,3.6},
		{0,-0.2,3.6},	//14

		// дно
		{0.0,-1.0,-2.5}, //15
		{-1.3,-1.0,-2.5},
		{-1.3,-1.0,3.8},
		{0.0,-1.0,3.9},
		{-2.3,-1.0,3.9},
		{-2.2,-1.0,3.8},
		{-2.3,-1.0,-2.5},
		{-2.2,-1.0,-2.5}, //22

		//под крылом
		{-1.3,-0.3,3.5}, //23
		{-2.2,-0.3,3.5} //24 
	};

	GLint base_triangles[40][3] =
	{
		// крышка
		{0,1,7},
		{1,2,7},
		{2,3,7},
		{7,3,6},
		{6,3,5},
		{5,3,4},

		// фасад верх
		{0,8,9},
		{0,9,1},
		{1,9,10},
		{1,10,11},
		{1,11,12},
		{1,12,2},
		{2,12,3},
		{3,12,13},
		{3,13,4},
		{4,13,14},
		{4,14,5},

		// фасад перед
		{9,8,15},
		{9,15,16},

		// фасад перед / крыло
		{10,21,11},
		{21,10,22},

		// фасад стоона
		{11,22,12},
		{12,22,19},
		{12,19,13},

		// фасад спина
		{13,19,18},
		{13,18,14},

		// дно
		{15,17,16},
		{15,18,17},
		{17,18,19},
		{17,19,20},
		{22,20,21},
		{21,20,19},

		//под крылом
		{9,16,23},
		{23,16,17},
		{23,17,20},
		{24,23,20},
		{24,20,22},
		{24,22,10},
		{9,23,10},
		{10,23,24},
	};

	for (auto& triangle : base_triangles)
		b_engine::draw_triangle_sim_x(&base[triangle[0]][0], &base[triangle[1]][0], &base[triangle[2]][0]);

	// люки
	glPushMatrix();
		glTranslatef(0, 0, 2);
		draw_hatch();
		glTranslatef(1, 00.1, -4.2);
		glRotatef(-6, 1, 0, 0);
		draw_hatch();
	glPopMatrix();
}

void draw_muzzle()
{
	glPushMatrix();
		glTranslatef(0, 0.4, -3);
		// крепление
		glPushMatrix();
		glTranslatef(0, 0.0, 1.9);
			glRotatef(90, 1, 0, 0);
			glutSolidCube(0.35);
		glPopMatrix();
	
		// дуло
		glPushMatrix();
			glRotatef(90, 1, 0, 0);
			b_engine::draw_cylinder(16, 0.1, 0.1, 4);
		glPopMatrix();
		// глушитель
		glTranslatef(0, 0.0, -2);
		glRotatef(90, 1, 0, 0);
		b_engine::draw_cylinder(16, 0.12, 0.16, 0.35);
	glPopMatrix();
}

void draw_panzer()
{
	glTranslatef(0, 1, 0);
	draw_turret();
	draw_base();
	draw_muzzle();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	if (camera_distance > 0) glRotatef(180, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, camera_distance);
	glRotatef(camera_rotation[0], 1.0, 0.0, 0.0);
	glRotatef(camera_rotation[1], 0.0, 1.0, 0.0);

	b_engine::draw_marking();

	draw_panzer();

	glPopMatrix();

	glutSwapBuffers();
}

void reshape(const int w, const int h)
{
	glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, static_cast<GLfloat>(w) / static_cast<GLfloat>(h), 0.001, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard_special(const int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		speed_rotation[0] = speed_rotation_value;   break;
	case GLUT_KEY_DOWN:
		speed_rotation[0] = -speed_rotation_value;  break;
	case GLUT_KEY_LEFT:
		speed_rotation[1] = speed_rotation_value;   break;
	case GLUT_KEY_RIGHT:
		speed_rotation[1] = -speed_rotation_value;  break;

	case GLUT_KEY_PAGE_UP:
		distance = distance_value;  break;
	case GLUT_KEY_PAGE_DOWN:
		distance = -distance_value; break;

	default:
		printf("unassigned button");
	}
}

void keyboard_special_up(const int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
		speed_rotation[0] = 0.0;    break;
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT:
		speed_rotation[1] = 0.0;    break;

	case GLUT_KEY_PAGE_UP:
	case GLUT_KEY_PAGE_DOWN:
		distance = 0;   break;

	default:
		printf("unassigned button");
	}
}
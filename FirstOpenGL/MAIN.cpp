#include <windows.h> 
#include "b_engine.h"
#include <stdlib.h>
#include <stdio.h>

void init();
void display();
void reshape(int w, int h);
void keyboard(unsigned char, int, int);
void keyboard_special(int key, int x, int y);
void mouse(int button, int state, int x, int y);

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	
    glutInitWindowSize(1600, 900);
    glutInitWindowPosition(160, 60);
    glutCreateWindow("panzer");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboard_special);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);

    GLfloat mat_specular[] = { 1.0,1.0,1.0,1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    GLfloat light_ambient[] = { 0.1,0.1,0.1,1.0 };  // Интенсивность фонового света
    GLfloat light_diffuse[] = { 0.8,0.8,0.8,1.0 };  // Интенсивность "цвета света"
    GLfloat light_position[] = { 1.0,0.0,1.0,0.0 }; // Положение источника света w == 0, источник света считается направленным
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); 
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_diffuse); // Интенсивность зеркального света
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    GLfloat light1_ambient[] = { 0.1,0.1,0.1,1.0 };  // Интенсивность фонового света
    GLfloat light1_diffuse[] = { 0.3,0.3,0.5,1.0 };  // Интенсивность "цвета света"
    GLfloat light1_position[] = { -10.0,10.0,1.0,1.0 }; // Положение источника света w == 0, источник света считается направленным
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_diffuse); // Интенсивность зеркального света
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.05);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}


GLdouble spin;
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	
    glutSolidTorus(0.275, 0.85, 16, 32);

    glFlush();
}

void reshape(const int w, const int h)
{
    glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, static_cast<GLfloat>(w) / static_cast<GLfloat>(h), 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char, int, int)
{
	
}

void keyboard_special(const int key, int x, int y)
{
	switch (key)
	{
    case GLUT_KEY_F1:
        glPolygonMode(GL_FRONT, GL_FILL);
        display();
        break;
    case GLUT_KEY_F2:
        glPolygonMode(GL_FRONT, GL_LINE);
        display();
        break;
    default:
		printf("unassigned button");
	}
}

void mouse(int button, int state, int x, int y)
{
	
}
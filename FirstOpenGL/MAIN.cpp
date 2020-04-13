#include <Windows.h>
#include <GL/glut.h>
#include <iostream>
#include <xlocale>

#define X .525731112119133606
#define Z .850650808352039932 

void init(void)
{
    GLfloat mat_specular[] = { 1.0,1.0,1.0,1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { 1.0,1.0,1.0,0.0 };
    GLfloat white_light[] = { 1.0,1.0,1.0,1.0 };

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}

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

void normalize(float v[3])
{
    GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (d == 0.0)
    {
	    std::cout << "Длина вектора равна 0";
        return;
    }
    v[0] /= d;
    v[1] /= d;
    v[2] /= d;
}

void normcrossprod(float v1[3], float v2[3], float out[3])
{
    out[0] = v1[1] * v2[2] - v1[2] * v2[1];
    out[1] = v1[2] * v2[0] - v1[0] * v2[2];
    out[2] = v1[0] * v2[1] - v1[1] * v2[0];
    normalize(out);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    glLoadIdentity();

    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 20; i++)
    {
        glNormal3fv(&vdata[tindices[i][0]][0]);
        glVertex3fv(&vdata[tindices[i][0]][0]);
        glNormal3fv(&vdata[tindices[i][1]][0]);
        glVertex3fv(&vdata[tindices[i][1]][0]);
        glNormal3fv(&vdata[tindices[i][2]][0]);
        glVertex3fv(&vdata[tindices[i][2]][0]);
    }
    glEnd();
	
    //glBegin(GL_TRIANGLES);
    //for (int i = 0; i < 20; i++)
    //{
    //    glVertex3fv(&vdata[tindices[i][0]][0]);
    //    glVertex3fv(&vdata[tindices[i][1]][0]);
    //    glVertex3fv(&vdata[tindices[i][2]][0]);
    //	
    //    GLfloat d1[3], d2[3], norm[3];
    //    for (int j = 0; j < 3; j++)
    //    {
    //        d1[j] = vdata[tindices[i][0]][j] - vdata[tindices[i][1]][j];
    //        d2[j] = vdata[tindices[i][1]][j] - vdata[tindices[i][2]][j];
    //    }
    //    normcrossprod(d1, d2, norm);
    //    glNormal3fv(norm);
    //}
    //glEnd();

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

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1600, 900);
    glutInitWindowPosition(160, 60);
    glutCreateWindow("Transformed Cube");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
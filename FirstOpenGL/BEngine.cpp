#include "BEngine.h"

void b_engine::draw_triangle(float* v1, float* v2, float* v3)
{
    glBegin(GL_TRIANGLES);
    glNormal3fv(v1);
    glVertex3fv(v1);
    glNormal3fv(v2);
    glVertex3fv(v2);
    glNormal3fv(v3);
    glVertex3fv(v3);
    glEnd();
}

void b_engine::reshape(const int w, const int h) const
{
    glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, static_cast<GLfloat>(w) / static_cast<GLfloat>(h), z_near, z_far);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

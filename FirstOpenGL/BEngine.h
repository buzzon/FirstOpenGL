#pragma once

#include <windows.h> 
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h> 

 static class b_engine
{
 public:
     static GLdouble z_near = 1.0;
     static GLdouble z_far = 50.0;
 	
     void draw_triangle(float*, float*, float*);
     void reshape(int, int) const;
};


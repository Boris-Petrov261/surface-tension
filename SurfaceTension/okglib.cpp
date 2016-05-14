#include <cstdlib>
#include <math.h>
#include <GL/glfw.h>
#include "okglib.h"

#define M_PI 3.141592

float random(float a, float b)
{
    return (rand()/(float)RAND_MAX)*(b-a)+a;
}

float irandom(int a, int b)
{
    return rand()%(b-a+1)+a;
}

bool running()
{
    return( !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam( GLFW_OPENED) );
}

void init()
{
    int width, height;

    glfwInit();
    if( !glfwOpenWindow( 1280, 960, 0, 0, 0, 0, 8, 0, GLFW_WINDOW ) ) return;

    glfwGetWindowSize( &width, &height );
    height = height > 0 ? height : 1;

    glViewport( 0, 0, width, height );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 65.0f, (GLfloat)width/(GLfloat)height, 1.0f, 100.0f );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(0.0f, -7.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f );
}

void finit()
{
    glfwTerminate();
}
void drawOxyz()
{
    // Рисуване на лъчите
    glBegin( GL_LINES );
        // OX
        glVertex3f( -10.0, 0.0, 0.0 );
        glVertex3f( 10.0, 0.0, 0.0 );
        // OY
        glVertex3f( 0.0, 10.0, 0.0 );
        glVertex3f( 0.0, 0.0, 0.0 );
        // OZ
        glVertex3f( 0.0, 0.0, 10.0 );
        glVertex3f( 0.0, 0.0, 0.0 );
    glEnd();
}



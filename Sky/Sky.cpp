#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector> // la classe vector di STL
#include "Sky.h"
#include "../mesh/point3.h"
#include "../mesh/mesh.h"

void Sky::Init() {
    size=100;
    px = py = pz = 0;
}

// disegna a schermo
void Sky::Render() const {
    enum {
        SKY_LEFT = 0, SKY_BACK = 1, SKY_RIGHT = 2, SKY_FRONT = 3, SKY_TOP = 4, SKY_BOTTOM = 5
    };      //constants for the skybox faces, so we don't have to remember so much number
    glColor3f(1,1,1);
    glDisable(GL_LIGHTING); //turn off lighting, when making the skybox
    glDisable(GL_DEPTH_TEST);       //turn off depth texting
    glEnable(GL_TEXTURE_2D);        //and turn on texturing

    glBindTexture(GL_TEXTURE_2D, SKY_BACK);  //use the texture we want
    glBegin(GL_QUADS);      //and draw a face
    //back face
    glTexCoord2f(0, 0);      //use the correct texture coordinate
    glVertex3f(size / 2, size / 2, size / 2);       //and a vertex
    glTexCoord2f(1, 0);      //and repeat it...
    glVertex3f(-size / 2, size / 2, size / 2);
    glTexCoord2f(1, 1);
    glVertex3f(-size / 2, -size / 2, size / 2);
    glTexCoord2f(0, 1);
    glVertex3f(size / 2, -size / 2, size / 2);
    glEnd();



    glBindTexture(GL_TEXTURE_2D, SKY_LEFT);
    glBegin(GL_QUADS);
    //left face
    glTexCoord2f(0, 0);
    glVertex3f(-size / 2, size / 2, size / 2);
    glTexCoord2f(1, 0);
    glVertex3f(-size / 2, size / 2, -size / 2);
    glTexCoord2f(1, 1);
    glVertex3f(-size / 2, -size / 2, -size / 2);
    glTexCoord2f(0, 1);
    glVertex3f(-size / 2, -size / 2, size / 2);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, SKY_FRONT);
    glBegin(GL_QUADS);
    //front face
    glTexCoord2f(1, 0);
    glVertex3f(size / 2, size / 2, -size / 2);
    glTexCoord2f(0, 0);
    glVertex3f(-size / 2, size / 2, -size / 2);
    glTexCoord2f(0, 1);
    glVertex3f(-size / 2, -size / 2, -size / 2);
    glTexCoord2f(1, 1);
    glVertex3f(size / 2, -size / 2, -size / 2);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, SKY_RIGHT);
    glBegin(GL_QUADS);
    //right face
    glTexCoord2f(0, 0);
    glVertex3f(size / 2, size / 2, -size / 2);
    glTexCoord2f(1, 0);
    glVertex3f(size / 2, size / 2, size / 2);
    glTexCoord2f(1, 1);
    glVertex3f(size / 2, -size / 2, size / 2);
    glTexCoord2f(0, 1);
    glVertex3f(size / 2, -size / 2, -size / 2);
    glEnd();


    glBindTexture(GL_TEXTURE_2D, SKY_TOP);
    glBegin(GL_QUADS);                      //top face
    glTexCoord2f(1, 0);
    glVertex3f(size / 2, size / 2, size / 2);
    glTexCoord2f(0, 0);
    glVertex3f(-size / 2, size / 2, size / 2);
    glTexCoord2f(0, 1);
    glVertex3f(-size / 2, size / 2, -size / 2);
    glTexCoord2f(1, 1);
    glVertex3f(size / 2, size / 2, -size / 2);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, SKY_BOTTOM);
    glBegin(GL_QUADS);
    //bottom face
    glTexCoord2f(1, 1);
    glVertex3f(size / 2, -size / 2, size / 2);
    glTexCoord2f(0, 1);
    glVertex3f(-size / 2, -size / 2, size / 2);
    glTexCoord2f(0, 0);
    glVertex3f(-size / 2, -size / 2, -size / 2);
    glTexCoord2f(1, 0);
    glVertex3f(size / 2, -size / 2, -size / 2);
    glEnd();

    glEnable(GL_LIGHTING);  
    glEnable(GL_DEPTH_TEST);



}





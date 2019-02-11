#include <math.h>
#include <stdlib.h>
#include <time.h> /* time */
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include "cube.h"

using namespace std;
extern bool axesOnOff;
extern int points;
extern int typeSpecial,lvMagnete;
extern bool speed;

void drawAxis(int); // definita in main.cpp

void Cube::Render()
{
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    if(!joined) {
        glPushMatrix();
        glTranslatef(px, py, pz);
        quad(0, 3, 2, 1);
        quad(3,7,6,2);
        quad(0, 4, 7, 3);
        quad(1, 2, 6, 5);
        quad(7,4,5,6);
        quad(4,0,1,5);
        glPopMatrix();
    }
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    

}

void Cube::setCoord() {
    // cout << "Setting Coords..." << endl;
    GLfloat l2 = ((GLfloat)lato) / 2;
    for (int i = 0; i < 8; i++)
    {
        ver[i][0] *= l2;
        ver[i][1] *= l2;
        ver[i][2] *= l2;
    }
}
void Cube::quad(int a, int b, int c, int d) {
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, 7);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(ver[a]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(ver[b]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(ver[c]);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(ver[d]);
    glEnd(); //endpolygon
    glDisable(GL_TEXTURE_2D);


}

bool Cube::join(Ship s) {
//    return true;
    GLfloat epsilon = 2;
    if(joined) return false;

    if(fabsf(s.pz - pz) <= epsilon) {
        if(s.px >= px-1.5 && s.px <= px+1.5) {
            if(s.py >= py-1.5 && s.py <= py+1.5){
                joined = true;
                typeSpecial = rand() % 3;

                if(typeSpecial == 1) lvMagnete = 15;
                else if(typeSpecial == 2) speed = true;
                else points += 30;
                cerr << typeSpecial << endl;
                return true;
            }
        }
    }
    return  false;


}
#include <math.h>
#include <stdlib.h>
#include <time.h> /* time */
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include "../mesh/point3.h"
#include "../mesh/mesh.h"
#include "../Lake/Lake.h"

#include "Bridge.h"
using namespace std;
extern int points;
extern Lake lake;
void drawAxis(int); // definita in main.cpp
Mesh BridgeBody( (char *) "model/bridge.obj");
extern void SetupTexture(Point3 min, Point3 max, int type);
void Bridge::Init() {
    px = 350;
    py = 199;
    pz = 200;
}

void Bridge::Render() {
     float mat_ambient[4] ={ 0.19,0.07,0.02,1 };
    float mat_diffuse[4] ={0.7,0.27,0.08,1};
    float mat_specular[4] ={0.26,0.14,0.09,1 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    float shine = 128;
    glMaterialf(GL_FRONT, GL_SHININESS, shine );
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_COLOR_MATERIAL); //Abilita colore sulla mesh

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glPushMatrix();
        glTranslatef(px, py, pz);    
        glRotatef(90,0,1,0);
        glScaled(2.9,1,1);
        SetupTexture(BridgeBody.bbmin, BridgeBody.bbmax, 16);
        BridgeBody.RenderNxV();
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);

    glPopMatrix();

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_COLOR_MATERIAL); //Abilita colore sulla mesh

}


void Bridge::RenderReverse(){
    glPushMatrix();	    
    glTranslatef(px, lake.py-0.3,pz);
        glRotatef(90,0,1,0);
        glScaled(3.1,-0.005,1);
        SetupTexture(BridgeBody.bbmin, BridgeBody.bbmax, 16);
        BridgeBody.RenderNxV();
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);


    glPopMatrix();
    glColor3f(1, 1, 1);

}

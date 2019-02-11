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
#include "Coin.h"
using namespace std;
extern int points;
void drawAxis(int); // definita in main.cpp
Mesh bodyCoin( (char *) "model/coin.obj");
#define COIN_TEXTURE 6
#define ME_TEXTURE 7

extern int lvMagnete;
void SetupTexture(Point3 min, Point3 max, int type) {
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, type);
    glEnable(GL_TEXTURE_2D);

    // utilizzo le coordinate OGGETTO
    // cioe' le coordnate originali, PRIMA della moltiplicazione per la ModelView
    // in modo che la texture sia "attaccata" all'oggetto, e non "proiettata" su esso
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR); //(piano di riferimento in coord. mondo -> GL_OBJECT_LINEAR
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT ) ;
    // parametri per la funzione di generazione
    float sz = 1.0 / (max.Z() - min.Z());
    float ty = 1.0 / (max.Y() - min.Y());
    float s[4] = {0, 0, sz, -min.Z() * sz};
    float t[4] = {0, ty, 0, -min.Y() * ty};
    glTexGenfv(GL_S, GL_OBJECT_PLANE, s);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, t);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glPopMatrix();

}

void Coin::Render() {
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);

    if(!joined) {
    
        glPushMatrix();
        glColor3f(1,1,1);
        glTranslatef(px, py, pz);    
        angle += 8 ;
        glRotatef(angle,0,1,0);
        glScaled(0.4,0.4,0.8);
        SetupTexture(bodyCoin.bbmin, bodyCoin.bbmax, COIN_TEXTURE);
        bodyCoin.RenderNxV();
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);

        glPopMatrix();
        glColor3f(1,1,1);
    }
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
        

}


bool Coin::join(Ship s){
    GLfloat epsilon = 1.2;
    if(joined) return false;
    if(lvMagnete != 1) epsilon *=15;
    if(fabsf(s.pz - pz) <= epsilon) {
        if(s.px >= px-1-lvMagnete && s.px <= px+1+lvMagnete) {
            if(s.py >= py-1-lvMagnete && s.py <= py+1+lvMagnete){
                joined = true;
                points+=5;
                cerr << "\tNEW SCORE: "<< points;

                return true;
            }
        }
    }
    return false;
}

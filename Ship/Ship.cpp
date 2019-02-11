//
// Created by anto on 20/10/18.
//


#include <stdio.h>
#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector> // la classe vector di STL
#include "Ship.h"
#include "../Lake/Lake.h"


#include "../mesh/point3.h"
#include "../mesh/mesh.h"
#include "../Bridge/Bridge.h"

extern Lake lake;
extern Bridge b;

extern void drawAxis(int);
extern bool speed, betaVersion;
using namespace std;

Mesh ali((char *) "model/Ship/ali.obj");
Mesh vetri((char *) "model/Ship/vetri.obj");
Mesh body((char *) "model/Ship/body.obj");
Mesh cavi((char *) "model/Ship/cavi.obj");
Mesh elica((char *) "model/Ship/elica.obj");
Mesh pSxDx((char *) "model/Ship/paletteSXDX.obj");
Mesh pUpDw((char *) "model/Ship/paletteUPDOWN.obj");
Mesh rumo((char *) "model/Ship/ruote-motore.obj");
Mesh copri((char *) "model/Ship/coprimotore.obj");
extern bool useWireframe;
extern bool activeStencil;
extern bool Gouraud_Flat;

// da invocare quando e' stato premuto/rilasciato il tasto numero "keycode"
void Controller::EatKey(int keycode, int *keymap, bool pressed_or_released) {
    for (int i = 0; i < NKEYS; i++) {
        if (keycode == keymap[i]) key[i] = pressed_or_released;
    }
}


void Controller::Init() {
    for (int i = 0; i < NKEYS; i++) key[i] = false;
}

void Ship::Init() {
    facing = 0;  //orientamento


 
    inclinazioneRL = inclinazioneUD = 0;   // stato
    vx = vy = vz = 0;
    controller.Init();
    velInclinazioneRL = velInclinazioneUD = 1.1;
    velRitornoInclinazioneRL = velRitornoInclinazioneUD = 0.93;
    attritoZ = 0.97;
    attritoX = 0.4;
    attritoY = 0.8;

    grip = 0.1;
}



void Ship::DoStep() {
        if(speed) accMax = 0.01;
        else accMax = 0.005;
        float vxm, vym, vzm; // velocita' in spazio navicella
        // da vel frame mondo a vel frame navicella
        float cosf = cos(facing * M_PI / 180.0);
        float sinf = sin(facing * M_PI / 180.0);
        vxm = +cosf * vx - sinf * vz;
        vym = vy;
        vzm = +sinf * vx + cosf * vz;
        if (controller.key[Controller::LEFT]) inclinazioneRL += velInclinazioneRL;
        if (controller.key[Controller::RIGHT]) inclinazioneRL -= velInclinazioneRL;
        inclinazioneRL *= velRitornoInclinazioneRL; // ritorno a nave dritta
        if (controller.key[Controller::UP]) {
            inclinazioneUD += velInclinazioneUD;
            vym += accMax*2;
        }

        if (controller.key[Controller::DOWN]) {
            inclinazioneUD -= velInclinazioneUD;
            vym -= accMax*5;
        }

        vzm -= accMax;

        if (controller.key[Controller::ACC]) {
           vzm -= accMax - 0.001; 
        
            // vzm -= 0.003 - 0.001; 
        }

        inclinazioneUD *= velRitornoInclinazioneUD;


        // if(joinedPlatform) py = f.py + 1.02;
        vxm *= attritoX;
        vym *= attritoY;
        vzm *= attritoZ;

        facing = facing - (vzm * grip) * inclinazioneRL;
        // ritorno a vel coord mondo
        vx = +cosf * vxm + sinf * vzm;
        vy = vym;
        vz = -sinf * vxm + cosf * vzm;
        px += vx;
        pz += vz;
        py += vy;

        if(pz > 498) pz = 498;
        if(pz < 8) pz = 8;
        if(px > 499) px = 499;
        if(px < 9) px = 9;
        if( py < 200.60 ) py = 200.60;


        if(betaVersion && (py < 229.64 && py > 228) && (px >= 340 && px <= 356) 
            && (pz >= 8 && pz <= 449)) {
                py = 229.64;
                inclinazioneUD = 0;
                }

    }


int angle = 0;

// disegna a schermo
void Ship::Render() const {
    float mat_ambient[3] ={ 1.0f, 1.0f, 0.06625f };
    float mat_diffuse[3] ={ 0.18275f, 1.0f, 0.22525f};
    float mat_specular[3] ={1.0f, 1.0f, 0.346435f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 27);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL); //Abilita colore sulla mesh
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    if(activeStencil){
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        //non cambiare nessuna componente colore della maschera
	    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);  
        glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);    
        glStencilFunc(GL_ALWAYS, 1, 1);
        lake.RenderQuad(); // area su cui disegnare  (mask)
        glEnable(GL_DEPTH_TEST);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

        //(ref & mask) = (stencil & mask). => ref == stencilVal
        glStencilFunc(GL_EQUAL, 1, 1); // passa il test se stencilvalue == 1 (ref)
           //testStencil fail, Depth_test fail, depth_test pass
        glStencilOp(GL_ZERO, GL_REPLACE, GL_REPLACE);
    }
    RenderReverse();
    // per ottimizzare è stato inserito il riflesso del ponte nel render della ship
    // in modo da scrivere codice relativo allo stencil una sola volta
    if(betaVersion) b.RenderReverse(); //bridge
    if(activeStencil){
        glDisable(GL_STENCIL_TEST);
    }
    lake.Render();
    glPushMatrix();   
    glTranslatef(px, py -0.07,pz);
    glRotatef(facing, 0, 1, 0);
    if(Gouraud_Flat) RenderPartsGouraud();
    else RenderPartsFlat();
    glPopMatrix();
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 1);
    glDisable(GL_COLOR_MATERIAL);

}
// Normali per Vertice
void Ship::RenderPartsGouraud() const{
    //TODO: LOAD ON GRAPHICS 
    glPushMatrix();
        float a = 0.25;
        glScaled(a, a, -a);
        glRotatef(inclinazioneRL, 0, 0, 1);
        glRotatef(-inclinazioneUD, 1, 0, 0);
        glColor3f(0.4,0,0);
        body.RenderNxV();
        
        glPushMatrix();
            glColor3f(1,1,0.1);
            angle = angle+60;
            glRotatef(angle,0,0,1);

            elica.RenderNxV();
        glPopMatrix();
    
        glColor3f(1,1,1);
        copri.RenderNxV();
        pSxDx.RenderNxV();
        pUpDw.RenderNxV();
        ali.RenderNxV();
        glColor3f(0.2,0.2,0.2);
        rumo.RenderNxV();
        glColor3f(0.5,0.5,0.5);
        cavi.RenderNxV();
        //VETRI
        glColor4f(0.9,1,1,0.2);
      
        glPopMatrix();
}
// Normali per Faccia
void Ship::RenderPartsFlat() const{
    glPushMatrix();
        float a = 0.25;
        glScaled(a, a, -a);
        glRotatef(inclinazioneRL, 0, 0, 1);
        glRotatef(-inclinazioneUD, 1, 0, 0);
        glColor3f(0.4,0,0);
        body.RenderNxF();
        
        glPushMatrix();
            glColor3f(1,1,0.1);
            angle = angle+60;
            glRotatef(angle,0,0,1);

            elica.RenderNxF();
        glPopMatrix();
    
        glColor3f(1,1,1);
        copri.RenderNxF();
        pSxDx.RenderNxF();
        pUpDw.RenderNxF();
        ali.RenderNxF();
        glColor3f(0.2,0.2,0.2);
        rumo.RenderNxF();
        glColor3f(0.5,0.5,0.5);
        cavi.RenderNxF();
        //VETRI
        glColor4f(0.9,1,1,0.2);
      
        glPopMatrix();
}


void Ship::RenderReverse() const{

    glPushMatrix();	    
    glTranslatef(px, lake.py-0.4,pz);
    glRotatef(facing, 0, 1, 0);
    glScaled(1,-1,1);
    if (Gouraud_Flat) RenderPartsGouraud();
    else RenderPartsFlat();
    glPopMatrix();
    glColor3f(1, 1, 1);

}


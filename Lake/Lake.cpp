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
#include "Lake.h"
#include "../Ship/Ship.h"

using namespace std;
void drawAxis(int); // definita in main.cpp
extern int scrH, scrW;
extern Ship s;


void Lake::Render() {
    int dim = 350;
    px = 200;
    py = 200;
    pz = 185;
    //per la trasparenza
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.0, 1, 0.75, 0.3);
    glBegin(GL_QUADS);
    glVertex3f(px-dim, py,pz-dim/2);
    glVertex3f(px-dim, py,pz+dim/2);
    glVertex3f(px+dim,py, pz+dim/2);
    glVertex3f(px+dim, py, pz-dim/2);
    glEnd();
    glDisable(GL_BLEND);

}

//utilizzato per la maschera su cui disegnare la ship
void Lake::RenderQuad() {
    int dim = 350;
    px = 200;
    py = 200;
    pz = 185;
    glBegin(GL_QUADS);
    glVertex3f(px-dim, py,pz-dim/2);
    glVertex3f(px-dim, py,pz+dim/2);
    glVertex3f(px+dim,py, pz+dim/2);
    glVertex3f(px+dim, py, pz-dim/2);
    glEnd();

}

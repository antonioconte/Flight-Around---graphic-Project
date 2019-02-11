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
#include "Floor.h"
extern void SetupTexture(Point3 min, Point3 max, int type);
Mesh desert((char *) "model/desert.obj");
//Mesh desert((char *) "model/montagna.obj");

using namespace std;


void Floor::Render() const {
    float mat_ambient[3] = {0.19125f, 0.0735f, 0.0225f};
    float mat_diffuse[3] = {0.7038f, 0.27048f, 0.0828f};
    float mat_specular[3] = {0.0256777f, 0.0137622f, 0.0086014f};
    float shine = 12.0;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shine);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_COLOR_MATERIAL); // it allow to drive the glMaterial coulours using the more flexble glColor command

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glPushMatrix();

    glTranslatef(200,200,200);
    glScaled(1.3,0.8,1.3);
    SetupTexture(desert.bbmin, desert.bbmax, 12);
    desert.RenderNxV();


    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    glPopMatrix();
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

}


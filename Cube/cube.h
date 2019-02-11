#include "../Ship/Ship.h"

#ifndef GRAFICA_CUBE_H
#define GRAFICA_CUBE_H

class Cube{
    private:
        //Coords
        GLfloat ver[8][3] = {
            {-1.0,-1.0,1.0},
            {-1.0,1.0,1.0},
            {1.0,1.0,1.0},
            {1.0,-1.0,1.0},
            {-1.0,-1.0,-1.0},
            {-1.0,1.0,-1.0},
            {1.0,1.0,-1.0},
            {1.0,-1.0,-1.0}
        };

        void setCoord();
        void quad(int a,int b,int c,int d);
        int lato;
    
    public:
    GLfloat px, py, pz;

    bool joined = false;

        Cube(int l, GLfloat coords[]){
            px = coords[0];
            py = coords[1];
            pz = coords[2];
            lato = l;
            setCoord();
        }

        bool join(Ship s);
        void Render();
};

#endif
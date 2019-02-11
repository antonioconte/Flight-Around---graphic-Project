#include "../Ship/Ship.h"


#ifndef GRAFICA_COIN_H
#define GRAFICA_COIN_H

class Coin{
    private:
        //Coords
        int angle;

    public:
    GLfloat px, py, pz;

        Coin(GLfloat x, GLfloat y, GLfloat z){
            joined = false;
            px = x;
            py = y;
            pz = z;
            angle = 0;
        }
        Coin(Ship s){
            joined = false;
            px = s.px;
            py = s.py;
            pz = s.pz;
            angle = 0;
        }
        bool joined;
        bool join(Ship s);

        void Init();
        void Render();
};

#endif
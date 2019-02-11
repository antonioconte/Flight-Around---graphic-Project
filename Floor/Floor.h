#include "../Cube/cube.h"
#ifndef GRAFICA_FLOOR_H
#define GRAFICA_FLOOR_H


class Floor {
private:
    //Coords
    GLfloat px, py, pz;

public:
    Floor(){
        px = 50;
        py = 50;
        pz = 0;
    }

    void Render() const;
};


#endif //GRAFICA_FLOOR_H

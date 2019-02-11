#ifndef GRAFICA_BRIDGE_H
#define GRAFICA_BRIDGE_H

class Bridge{
    private:

    public:
        GLfloat px, py, pz;

        Bridge(){
            Init();
        }
        void RenderReverse();
        void Render();
        void Init();

};

#endif

#ifndef GRAFICA_SKY_H
#define GRAFICA_SKY_H

class Sky {
public:
    // Metodi
    void Init(); // inizializza variabili
    void Render() const; // disegna a schermo

    Sky() { Init();} // costruttore

    // STATO DELLO SPACE
    float size;
    int px, py, pz; //posizione

};

#endif

#ifndef GRAFICA_SHIP_H
#define GRAFICA_SHIP_H

class Controller {
public:
    enum {
        LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3, ACC=4, NKEYS = 5
    };
    bool key[NKEYS];

    void Init();

    void EatKey(int keycode, int *keymap, bool pressed_or_released);

    void Joy(int keymap, bool pressed_or_released);

    Controller() { Init(); } // costruttore
};


class Ship {

//    void RenderAllParts(bool usecolor) const;
//    // disegna tutte le parti della macchina
//    // invocato due volte: per la car e la sua ombra

public:
    // Metodi
    void Init(); // inizializza variabili
    void Render() const; // disegna a schermo
    void RenderReverse() const;
    void DoStep(); // computa un passo del motore fisico
    Ship() { Init(); } // costruttore
    void RenderPartsGouraud() const;
    void RenderPartsFlat() const;

    Controller controller;

    // STATO DEL VEICOLO
    float px, py, pz, facing; // posizione e orientamento
    float inclinazioneRL, inclinazioneUD; // stato interno
    float vx, vy, vz; // velocita' attuale
    // (di solito rimangono costanti)
    float velInclinazioneRL, velRitornoInclinazioneRL,
            velInclinazioneUD,velRitornoInclinazioneUD,
            accMax,
            grip,
            attritoX, attritoY, attritoZ; // attriti

};

#endif
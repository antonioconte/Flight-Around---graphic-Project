#include <iostream>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <GL/gl.h>
// #include <GL/glut.h>
#include <vector>
#include "GLText/GLText.h"
#include "Coin/Coin.h"
#include "Lake/Lake.h"
#include "Sky/Sky.h"
#include "Cube/cube.h"
#include "Bridge/Bridge.h"

#include "Floor/Floor.h"

using namespace std;

enum constTexture {
    SKY_LEFT = 0,
    SKY_BACK = 1,
    SKY_RIGHT = 2,
    SKY_FRONT = 3,
    SKY_TOP = 4,
    SKY_BOTTOM = 5,
    COIN_TEXTURE = 6,
    MYFACE = 7,
    MAGNETE = 8,
    SPEED = 9,
    FUEL = 10,
    MOUNTAIN = 11,
    FLOOR = 12,
    ICONPLANE = 13,
    FUELICON = 14,
    BONUSPOINT = 15,
    BRIDGE = 16
};
int points= 0;
int lvMagnete = 1; //lvMagnete = 20
bool bonusPoint = false;
bool speed = false;
int fuelLevel = 100;
int typeSpecial = -1;

int sec = 0;
clock_t startTime;
int centMill = 0;
int decMill = 0;
int unitSec = 0;
int decSec = 0;
int unitMin = 0;
int decMin = 0;
Ship s; Floor scene; Sky sky; Lake lake; vector<Coin> coins; vector<Cube> surprise;
Bridge b;
static int keymap[Controller::NKEYS] = {SDLK_a, SDLK_d, SDLK_w, SDLK_s, SDLK_SPACE};

//VAR GLOBALI
int nstep = 0; // numero di passi di FISICA fatti fin'ora
const int PHYS_SAMPLING_STEP = 9; // numero di millisec che un passo di fisica simula

// Frames Per Seconds
const int fpsSampling = 3000; // lunghezza intervallo di calcolo fps
float fps = 0; // valore di fps dell'intervallo precedente
int fpsNow = 0; // quanti fotogrammi ho disegnato fin'ora nell'intervallo attuale
Uint32 timeLastInterval = 0; // quando e' cominciato l'ultimo intervallo
SDL_Window *win;
SDL_GLContext mainContext;
Uint32 windowID;

TTF_Font *font20,*font30, *font40, *font50,*fontTitle;

int scrW = 1280, scrH = 720; //Dimensioni finestra
bool betaVersion = false;
bool done = false;
bool debugMode = false;
bool useWireframe = false;
bool typeGame = false;
bool activeStencil = true;
bool Gouraud_Flat = true;
bool pauseGame = false;
int lightType = 0;
int status = 0;
int voiceSelected = 0;
float viewAlpha = 0, viewBeta = -20; // angoli che definiscono la vista
float eyeDist = 2.0; // distanza dell'occhio dall'origine
int camera = 2;
void eventHandler( SDL_Event e);
void RenderTextScreenCentred(string message, SDL_Color color, int y, TTF_Font *font);
void renderInitMenu(SDL_Window *win);
void renderInfo(SDL_Window *win);
void rendering(SDL_Window *win);
void init();
void startGame();
void renderGame(SDL_Window *win);
void renderFinalScore(SDL_Window *win);
void idle(SDL_Window *win);
void RenderTextCoord(string message, SDL_Color color, int x, int y, TTF_Font *font);
void SetCoordToPixel();
void drawAxis(int);
bool LoadTexture(int textbind, string f);
void debugInfo();
void drawSpecial(int);
void viewInfo();
void viewMap();
void needFuel();
void setCamera();
int loadAllTexture(string name);
void loadTexturePNG2D(int textbind, char *filename);


int main(int argc, char *argv[]) {
    srand(time(NULL));
    // inizializzazione di SDL
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    font20 = TTF_OpenFont("Font/PoorStory-Regular.ttf", 20);
    font30 = TTF_OpenFont("Font/PoorStory-Regular.ttf", 30);
    font50 = TTF_OpenFont("Font/ARCADE.TTF", 50);
    font40 = TTF_OpenFont("Font/ARCADE.TTF", 40);
    fontTitle = TTF_OpenFont("Font/ARCADE.TTF", 200);
    fontTitle = TTF_OpenFont("Font/ARCADE.TTF", 180);
    
    win = SDL_CreateWindow(argv[0], 0, 0, scrW, scrH, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    mainContext = SDL_GL_CreateContext(win);
    init();
    loadAllTexture("Sunset");
  

    while (!done) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) eventHandler(e);
        else idle(win);
    }
    return 0;
}


int loadAllTexture(string name){
    string type = name;
    if (!LoadTexture(SKY_LEFT, "img/SkyBox/"+ type +"/left.jpg")) return -1;
    if (!LoadTexture(SKY_BACK,  "img/SkyBox/"+type+"/back.jpg")) return -1;
    if (!LoadTexture(SKY_RIGHT, "img/SkyBox/"+type+"/right.jpg")) return -1;
    if (!LoadTexture(SKY_FRONT, "img/SkyBox/"+type+"/front.jpg")) return -1;
    if (!LoadTexture(SKY_TOP,  "img/SkyBox/"+type+"/top.jpg")) return -1;
    if (!LoadTexture(SKY_BOTTOM, "img/SkyBox/"+type+"/bottom.jpg")) return -1;
    if (!LoadTexture(SKY_BOTTOM, "img/SkyBox/"+type+"/bottom.jpg")) return -1;
    if (!LoadTexture(MYFACE, "img/myface.jpg")) return -1;
    if (!LoadTexture(MOUNTAIN, "img/mount.png")) return -1;
    if (!LoadTexture(FLOOR, "img/desert3.jpg")) return -1;
    if (!LoadTexture(ICONPLANE, "img/iconplane.png")) return -1;
    if (!LoadTexture(FUELICON, "img/fuelIcon.png")) return -1;
    if (!LoadTexture(COIN_TEXTURE,"img/gold.jpg"))return -1;
    if (!LoadTexture(BRIDGE,"img/bridge.jpg"))return -1;

    loadTexturePNG2D(MAGNETE, (char *) "img/magnete.png");
    loadTexturePNG2D(SPEED, (char *) "img/speed.png");
    loadTexturePNG2D(FUEL, (char *) "img/fuel.png");

    loadTexturePNG2D(ICONPLANE, (char *) "img/iconplane.png");
    loadTexturePNG2D(BONUSPOINT, (char *) "img/bonus.png");

}

void eventHandler( SDL_Event e){
    switch (e.type) {
        case SDL_KEYDOWN: {

            if (e.key.keysym.sym == SDLK_r){
                camera = camera % 5  + 1 ;
            }
            if (e.key.keysym.sym == SDLK_p && voiceSelected==0) {
                    pauseGame = !pauseGame;
            }
            // if (e.key.keysym.sym == SDLK_q) done = true;
            if (e.key.keysym.sym == SDLK_ESCAPE && status != 0 ) status = 0;
            if (status == 0) {
                //MENU
                if (e.key.keysym.sym == SDLK_DOWN) {
                    voiceSelected++;
                    if (voiceSelected > 4) voiceSelected = 4;
                }
                if (e.key.keysym.sym == SDLK_UP) {
                    voiceSelected--;
                    if (voiceSelected < 0) voiceSelected = 0;
                }

               
                if (e.key.keysym.sym == SDLK_RETURN) {
                    if (voiceSelected == 0) {
                        pauseGame = false;
                        typeGame = false; //fuel version
                        status = 1;
                        startGame();
                    }
                    if (voiceSelected == 1) {
                        pauseGame = false;
                        typeGame = true; //time version
                        status = 1; 
                        startGame();
                    }
                    if (voiceSelected == 2) status = 2;
                    if (voiceSelected == 3) done = true;
                    if (voiceSelected == 4) {
                        pauseGame = false;
                        status = 1;
                        betaVersion = true;
                        startGame();

                    }
                }


            } else if (status == 1) {
                s.controller.EatKey(e.key.keysym.sym, keymap, true);
                if (e.key.keysym.sym == SDLK_F1) {
                    useWireframe = !useWireframe;
                }
               
                if (e.key.keysym.sym == SDLK_F2) {
                    activeStencil = !activeStencil;
                }
                if (e.key.keysym.sym == SDLK_F3) {
                    Gouraud_Flat = !Gouraud_Flat;
                }
                if (e.key.keysym.sym == SDLK_F4) {
                    lightType = 1 - lightType;
                }
                if (e.key.keysym.sym == SDLK_F5) {
                    betaVersion = !betaVersion;
                }
                if (e.key.keysym.sym == SDLK_F11) {
                    debugMode =  !debugMode;
                }
                //Game control
            } else if (status == 2) {
                //Info control
            }

            break;

        }
        case SDL_KEYUP:{
            s.controller.EatKey(e.key.keysym.sym, keymap, false);
            break;
        }
        case SDL_MOUSEMOTION:
            if (e.motion.state & SDL_BUTTON(1) /*& cameraType == CAMERA_MOUSE*/) {
                viewAlpha += e.motion.xrel;

                viewBeta += e.motion.yrel;
                if(viewBeta<-5) viewBeta = -5;
            }
            break;

        case SDL_MOUSEWHEEL:
            if (e.wheel.y > 0) {
                // (zoom in)
                eyeDist = eyeDist * 0.9;
                // if (eyeDist < 3) eyeDist = 3;
            };
            if (e.wheel.y < 0) {
                // (zoom out)
                eyeDist = eyeDist / 0.9;
                if (eyeDist > 7) eyeDist = 7;

            };
            break;

        case SDL_QUIT: {
            done = true;
            break;
        }
        case SDL_WINDOWEVENT: {
            // dobbiamo ridisegnare la finestra
            if (e.window.event == SDL_WINDOWEVENT_EXPOSED)
                rendering(win);
            else {
                windowID = SDL_GetWindowID(win);
                if (e.window.windowID == windowID) {
                    switch (e.window.event) {
                        case SDL_WINDOWEVENT_SIZE_CHANGED: {
                            scrW = e.window.data1;
                            scrH = e.window.data2;
                            glViewport(0, 0, scrW, scrH);
                            rendering(win);
                            //redraw(); // richiedi ridisegno
                            break;
                        }
                    }
                }
            }
            break;
        }
    }
}


bool LoadTexture(int textbind, string f) {
    GLenum texture_format;
    const char *filename= f.c_str();

    SDL_Surface *s = IMG_Load(filename);
    if (!s) return false;

    if (s->format->BytesPerPixel == 4) {     // contiene canale alpha
        if (s->format->Rmask == 0x000000ff) {
            texture_format = GL_RGBA;
        } else {
            texture_format = GL_BGRA;
        }
    } else if (s->format->BytesPerPixel == 3) {     // non contiene canale alpha
        if (s->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
        else
            texture_format = GL_BGR;
    } else {
        printf("[ERROR] the image is not truecolor\n");
        exit(1);
    }

    glBindTexture(GL_TEXTURE_2D, textbind);
    gluBuild2DMipmaps(
            GL_TEXTURE_2D,
            3,
            s->w, s->h,
            texture_format,
            GL_UNSIGNED_BYTE,
            s->pixels
    );
    glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            GL_LINEAR);
    glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
    //per lo skybox
    if (textbind==SKY_BOTTOM || textbind==SKY_TOP || textbind==SKY_FRONT || textbind==SKY_BACK || textbind==SKY_RIGHT || textbind==SKY_LEFT ) {
        SDL_FreeSurface(s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR);    //if the texture is smaller, than the image, we get the avarege of the pixels next to it
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    //same if the image bigger
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE); //we repeat the pixels in the edge of the texture, it will hide that 1px wide line at the edge of the cube, which you have seen in the video
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    }
    return true;
}

int timeToReset = 500;
void drawSpecial(int typeSpecial) {
    if(timeToReset == 0) {
        timeToReset = 8000;
        bonusPoint = false;
        lvMagnete = 1;
        speed = false;
    }
    timeToReset--;
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0, scrW, 0, scrH); // m_Width and m_Height is the resolution of window
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    int oX, oY, h, w;
    h = w = 50;
    oX = 20;
    oY = scrH - 75;
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, typeSpecial);
    //per sfondo trasparente
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2d(oX + w, oY);
        glTexCoord2f(1, 0);
        glVertex2d(oX + w, oY + h);
        glTexCoord2f(1, 1);
        glVertex2d(oX, oY + h);
        glTexCoord2f(0, 1);
        glVertex2d(oX, oY);
        glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glColor3f(1, 1, 1);

    glPopMatrix();
    glPopMatrix();
}



bool visible = true;

void needFuel() {
    // cerr << fuelLevel << endl;
    if(fpsNow % 30 == 0) fuelLevel -= 1;
    if(fuelLevel == 0) status = 3; //GAMEOVER
    SDL_Color a  = {1,1,1,1};
      int oX, oY, h, w;
        h = w = 40;
        oX = 20;
        oY = 20;
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, FUEL);
        if(fuelLevel > 45) glColor3f(0,1,0);
        else if(fuelLevel > 15 && fuelLevel <= 45) glColor3f(1,1,0);
        else glColor3f(1,0,0);
        if(fuelLevel < 10 && fpsNow %10 == 0) visible = !visible;
        if(visible){
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex2d(oX + w, oY);
            glTexCoord2f(1, 0);
            glVertex2d(oX + w, oY + h);
            glTexCoord2f(1, 1);
            glVertex2d(oX, oY + h);
            glTexCoord2f(0, 1);
            glVertex2d(oX, oY);
            glEnd();
        }
        glDisable(GL_TEXTURE_2D);

}


void init(){
    srand(time(NULL));

    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 2 );
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glFrontFace(GL_CW);
    glEnable(GL_POLYGON_OFFSET_FILL);

    glPolygonOffset(1, 1);

}


void loadTexturePNG2D(int textbind, char *filename) {

    SDL_Surface *surface;
    GLenum texture_format;
    GLint nOfColors;

    if ((surface = IMG_Load(filename))) {

        
        // get the number of channels in the SDL surface
        nOfColors = surface->format->BytesPerPixel;
        if (nOfColors == 4) // contains an alpha channel
        {
            if (surface->format->Rmask == 0x000000ff)
                texture_format = GL_RGBA;
            //else
            //texture_format = GL_BGRA;
        } else if (nOfColors == 3) // no alpha channel
        {
            if (surface->format->Rmask == 0x000000ff)
                texture_format = GL_RGB;
            //else
            //texture_format = GL_BGR;
        } else {
            printf("warning: the image is not truecolor..  this will probably break\n");
            // this error should not go unhandled
        }
        // Bind the texture object
        glBindTexture(GL_TEXTURE_2D, textbind);

        // Set the texture's stretching properties
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Edit the texture object's image data using the information SDL_Surface gives us
        glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                     texture_format, GL_UNSIGNED_BYTE, surface->pixels);
    } else {
        printf(SDL_GetError());
        SDL_Quit();
    }

    // Free the SDL_Surface only if it was successfully created
    if (surface) {
        SDL_FreeSurface(surface);
    }
}
void SetCoordToPixel()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-1, -1, 0);
    glScalef(2.0 / scrW, 2.0 / scrH, 1);
}

void setCamera(){
//    cerr << "Alpha: "<< viewAlpha <<endl;
//    cerr << "Beta: "<< viewBeta <<endl;
//    cerr << "Dist: "<< eyeDist <<endl;

    switch (camera){
        case 1:{
            //PRIMA PERSONA
            viewAlpha = 0, viewBeta = -14, eyeDist = 0.235507;
            break;
        }
        case 2: {
            viewAlpha = 0, viewBeta = 15, eyeDist = 1;
            break;
        }
        case 3: {
            viewAlpha = 0, viewBeta = 50, eyeDist = 5;
            break;
        }
        case 4: {
            //mouse
            break;
        }
        case 5: {
            viewAlpha = -138, viewBeta = 18, eyeDist = 1.77;

            break;
        }
    }
    glTranslatef(0, 1.4, -eyeDist);
    glRotatef(viewBeta, 1, 0, 0);
    glRotatef(-s.facing+viewAlpha, 0, 1, 0);
}


void renderGame(SDL_Window *win) {

    fpsNow++;
    glLineWidth(1);

    glViewport(0, 0, scrW, scrH);
    glClearColor(1, 1, 1, 1); //background color
    // settiamo la matrice di proiezione per la view
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, //fovy,
                   ((float) scrW) / scrH,  //aspect Y/X,
                   0.01,                   //distanza del NEAR CLIPPING PLANE in coordinate vista
                   1000                   //distanza del FAR CLIPPING PLANE in coordinate vista
    );
    glMatrixMode(GL_MODELVIEW); //per applicare le trasformazioni degli oggetti
    glLoadIdentity();
    //Pulisce il depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    setCamera();
    //render dello skybox in base al punto di origine della camera. 
    //cosi lo skybox rimane sempre fermo
    sky.Render();

    //mi sposto nello spazio della ship
    glTranslatef(-s.px, -s.py - 1.48, -s.pz);

    
    float tmpv[4] = {3000,500,1300,lightType}; // ultima comp: 0DIREZIONALE, 1PUNTIFORME  
    glLightfv(GL_LIGHT0, GL_POSITION, tmpv);
    scene.Render(); //desert render
    if(betaVersion) b.Render();  //bridge render
    
    for (unsigned i = 0; i < coins.size(); ++i)
        coins[i].Render();
    for (unsigned i = 0; i < surprise.size(); ++i)
        surprise[i].Render();
    // gestione conflitti 
    for (unsigned i = 0; i < coins.size(); ++i)
        coins[i].join(s);
    for (unsigned i = 0; i < surprise.size(); ++i)
        surprise[i].join(s);

    s.Render(); //ship render

    viewInfo();
    /* TYPEGAME:
     bool: false -> fuelVersion
     bool: true -> timeVersion */
    if(unitMin == 2 && typeGame) status = 3; //PER IL GIOCO A TEMPO
    glFinish();
    SDL_GL_SwapWindow(win);
}



void viewInfo(){

    stringstream scoreSt;
    stringstream time;
    scoreSt << "Points: " << points;
    SDL_Color color = {255, 255, 255, 1};
    if(pauseGame)RenderTextScreenCentred("PAUSA", color,scrH -100, font50);
    if(camera == 4) RenderTextScreenCentred("MUOVI IL MOUSE", color,22, font30);
    if (typeGame && !pauseGame){
        sec = clock() - startTime;
        centMill = (sec / 10000) % 10;
        decMill = (sec / 100000) % 10;
        unitSec = (sec / 1000000) % 10;
        decSec = (sec / 10000000) % 6;
        unitMin = (sec / 60000000) % 10;
        decMin = (sec / 600000000) % 6;
        time << "Time  " << decMin << unitMin << ":" << decSec << unitSec << ":" << decMill << centMill;
        RenderTextCoord(time.str(), color, scrW - 240, scrH - 100, font30);
    }

    RenderTextCoord(scoreSt.str(), color, scrW - 240, scrH - 50, font30);
    if(debugMode){
      debugInfo();
    }



    if(lvMagnete != 1) drawSpecial(MAGNETE);
    else if(speed) drawSpecial(SPEED);
    else if(bonusPoint) drawSpecial(BONUSPOINT);
    if(!typeGame){
         SDL_Color colorB = {0,0,0, 1};
        RenderTextCoord(to_string(fuelLevel) + "%",colorB,70,22, font30);

    }   

    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0, scrW, 0, scrH); // m_Width and m_Height is the resolution of window
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    if (!typeGame && !pauseGame) needFuel();
    viewMap();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glColor3f(1, 1, 1);
    glPopMatrix();
    glPopMatrix();

}

void debugInfo(){
    SDL_Color color = {255, 255, 255, 1};
    //info coord ship
    stringstream spx;
    spx << "[ X: "<<s.px << ", Y: " << s.py << ", Z: " << s.pz << "]";
    RenderTextCoord(spx.str(), color, 10, scrH - 22, font20);
    // info coord cam
    stringstream camCoord;
    camCoord << "[ Alpha: "<<viewAlpha << ", Beta: " << viewBeta << ", EyeDist: " << eyeDist << "]";
    RenderTextCoord(camCoord.str(), color, 10, scrH - 44, font20);

    //status wireframe
    stringstream statusWireframe; 
    statusWireframe << useWireframe ;
    RenderTextCoord("Wireframe: " + statusWireframe.str(), color, 10, scrH - 66, font20);
    
    //status pencil
    stringstream statusStencil; 
    statusStencil << activeStencil ;
    RenderTextCoord("Stencil: " + statusStencil.str(), color, 10, scrH - 88, font20);
    
    //Type Shading
    stringstream shading; 
    if(Gouraud_Flat) shading << "GOURAUD";
    else shading << "FLAT";
    RenderTextCoord("SHADING: " + shading.str(), color, 10, scrH - 110, font20);

     //Type Light
    stringstream typeL; 
    if(lightType == 1) typeL << "Puntiforme";
    else typeL << "Direzionale";
    RenderTextCoord("TipoLuce: "+ typeL.str(), color, 10, scrH - 132, font20);

      //FPS
    stringstream fpsString; 
    fpsString << fps;
    RenderTextCoord("FPS: "+ fpsString.str(), color, 10, scrH - 154, font20);

}

void viewMap(){

    float w = scrW/5;
    float h = scrW/5;
    float verticesMap[4][2] = {
            {scrW, 0},
            {scrW-w, 0 },
            {scrW-w, h},
            {scrW, h},
    };
    float dimPlane2d = 8.0;
    float planeVertex[4][2] = {
            {dimPlane2d, -dimPlane2d},
            {-dimPlane2d, -dimPlane2d},
            {-dimPlane2d, dimPlane2d},
            {dimPlane2d, dimPlane2d}
    };


    glPushMatrix();
    // setta le matrici di trasformazione in modo che le coordinate 
    // in spazio oggetto siano le coord del pixel sullo schemo
    SetCoordToPixel();
    //background mappa
    glBegin(GL_QUADS);
    glColor4f(1,0.2,0.2,0.5);
    glVertex2fv(verticesMap[2]);
    glVertex2fv(verticesMap[3]);
    glVertex2fv(verticesMap[0]);
    glVertex2fv(verticesMap[1]);
    glEnd();


    //2D plane
    glPushMatrix();
    glTranslatef(scrW-w + s.pz*w/500,s.px*h/500,0);
    glRotatef(s.facing + 135, 0, 0, 1); //+135 per allineare la texture in base al facing
    
    glBindTexture(GL_TEXTURE_2D, ICONPLANE);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glColor3f(1, 1, 1);
    glTexCoord2f(0, 0);
    glVertex2fv(planeVertex[2]);
    glTexCoord2f(1, 0);
    glVertex2fv(planeVertex[3]);
    glTexCoord2f(1, 1);
    glVertex2fv(planeVertex[0]);
    glTexCoord2f(0, 1);
    glVertex2fv(planeVertex[1]);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();





    float hSpace = s.py;//altezza y aereo
    for (unsigned i = 0; i < coins.size(); ++i){
        if(!coins[i].joined){
            glPushMatrix();
            glTranslatef(scrW-w + coins[i].pz*w/500,coins[i].px*h/500,0);
            float diffCoinSpace = fabsf(hSpace - coins[i].py);
            if( diffCoinSpace < 10) {
                glPointSize(10);
                glColor3f(1,1,0);
            }
            else if(diffCoinSpace >= 10 && diffCoinSpace <= 100){
                glPointSize(7);
                glColor3f(1,1,0.3);
            }
            else {
                glPointSize(4);
                glColor3f(1,1,0.9);
            }
            glEnable(GL_POINT_SMOOTH);
            glBegin(GL_POINTS);
            glVertex2d(0,0);
            glEnd();
            glDisable(GL_POINT_SMOOTH);

            glPopMatrix();
        }
    }


    for (unsigned i = 0; i < surprise.size(); ++i){
        if(!surprise[i].joined){
            glPushMatrix();
            glTranslatef(scrW-w + surprise[i].pz*w/500,surprise[i].px*h/500,0);

            glPointSize(10);
            glColor3f(0,1,0);
            glEnable(GL_POINT_SMOOTH);
            glBegin(GL_POINTS);
            glVertex2d(0,0);
            glEnd();
            glDisable(GL_POINT_SMOOTH);

            glPopMatrix();
        }
    }


    glPopMatrix();
}

// disegna gli assi nel sist. di riferimento
void drawAxis(int dim){
    const float K = 0.10;
    glColor3f(0, 0, 1);
    glBegin(GL_LINES);
    //X
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(dim, 0, 0);
    //Y
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, +dim, 0);
    //Z
    glColor3f(0, 0, 100);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, +dim);
    glEnd();

    glBegin(GL_TRIANGLES);

    glColor3f(1, 0, 0);
    glVertex3f(+dim, 0, 0);
    glVertex3f(+dim - K, +K, 0);
    glVertex3f(+dim - K, -K, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, +dim, 0);
    glVertex3f(K, +dim - K, 0);
    glVertex3f(-K, +dim - K, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, +dim);
    glVertex3f(0, +K, +dim - K);
    glVertex3f(0, -K, +dim - K);
    glEnd();

    glColor3f(1, 1, 1);
}

int dimFontTitle = 50;
void renderInitMenu(SDL_Window *win){
    fpsNow++;
    glLineWidth(3); // linee larghe
    // settiamo il viewport
    glViewport(0, 0, scrW, scrH);
    glClearColor(0, 0, 0, 1);  //background color
    glMatrixMode(GL_PROJECTION);     // settiamo la matrice di proiezione
    glLoadIdentity();
    gluPerspective(70, //fovy,
                   ((float) scrW) / scrH,//aspect Y/X,
                   0.2,//distanza del NEAR CLIPPING PLANE in coordinate vista
                   10000  //distanza del FAR CLIPPING PLANE in coordinate vista
    );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // riempe tutto lo screen buffer di pixel color sfondo
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sky.Render();
    //disattivo eventuali stati
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    SDL_Color colorW = {255, 255, 255, 0};
    SDL_Color colorR = {255, 0, 0, 0};
    SDL_Color color;
 
    RenderTextScreenCentred("Flight Around", colorW, scrH - 250, fontTitle);
    string s = " ";
    TTF_Font *fontToUse;

    if(voiceSelected == 0 ) {
        s = "> Ottieni piu' punti possibili: occhio a non finire la benzina!";
        color = colorR;
        fontToUse = font50;
    }
    else {
        color = colorW;
        fontToUse = font40;
    }
    RenderTextScreenCentred("NEW GAME (fuel version)", color, scrH - 360, fontToUse);


    //SFIDA A TEMPO
    if(voiceSelected == 1 ) {
        s = "> Ottieni piu' punti possibili: entro il minuto";
        color = colorR;
        fontToUse = font50;
    }
    else {
        color = colorW;
        fontToUse = font40;
    }
    RenderTextScreenCentred("NEW GAME (time version)", color, scrH - 420, fontToUse);

    if(voiceSelected == 2 ){
        s = "> Come si gioca? Te lo spiego!";
        color = colorR;
        fontToUse = font50;
    }
    else {
        color = colorW;
        fontToUse = font40;
    }
    RenderTextScreenCentred("INFO", color, scrH - 480, fontToUse);

    if(voiceSelected == 3 ){
        s = "> Vuoi gia' uscire? :)";
        color = colorR;        fontToUse = font50;

    }
    else{
        color = colorW;
        fontToUse = font40;
    }
    RenderTextScreenCentred("QUIT", color, scrH - 540, fontToUse);

    if(voiceSelected == 4 ){
        s = "> Prova la versione 'Work in Progress'";
        color = colorR;        fontToUse = font50;
    }
    else{
        color = colorW;
        fontToUse = font40;
    }
    RenderTextScreenCentred("Beta Version", color, scrH - 600, fontToUse);


    RenderTextScreenCentred(s, colorW, scrH/15, font30);


    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    glFinish();
    SDL_GL_SwapWindow(win);
}
void renderFinalScore(SDL_Window *win){

    glLineWidth(3); // linee larghe
    // settiamo il viewport
    glViewport(0, 0, scrW, scrH);
    glClearColor(0, 0, 0, 1);  //background color
    //projection matrix definisce le proprietà della camera
    glMatrixMode(GL_PROJECTION);    
    glLoadIdentity();
    gluPerspective(70, //fovy,
                   ((float) scrW) / scrH,//aspect Y/X,
                   0.2,//distanza del NEAR CLIPPING PLANE in coordinate vista
                   10000  //distanza del FAR CLIPPING PLANE in coordinate vista
    );
    //modelview matrix definisce come un oggetto deve essere trasformato
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // riempe tutto lo screen buffer di pixel color sfondo
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sky.Render();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    SDL_Color colorW = {255, 255, 255, 0};
    stringstream score;
    score << points;
    RenderTextScreenCentred("FINAL SCORE", colorW, scrH/2, fontTitle);

    RenderTextScreenCentred(score.str(), colorW, scrH/2 - 150, fontTitle);

    RenderTextScreenCentred("premi ESC per tornare al menu' principale", colorW, 50, font20);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_LIGHTING);
    glFinish();
    SDL_GL_SwapWindow(win);
}


void renderInfo(SDL_Window *win){

    glLineWidth(3); // linee larghe
    // settiamo il viewport
    glViewport(0, 0, scrW, scrH);
    glClearColor(0, 0, 0, 1);  //background color
    glMatrixMode(GL_PROJECTION);     // settiamo la matrice di proiezione
    glLoadIdentity();
    gluPerspective(70, //fovy,
                   ((float) scrW) / scrH,//aspect Y/X,
                   0.2,//distanza del NEAR CLIPPING PLANE in coordinate vista
                   10000  //distanza del FAR CLIPPING PLANE in coordinate vista
    );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // riempe tutto lo screen buffer di pixel color sfondo
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sky.Render();

    //disabilito eventuali stati
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    SDL_Color colorW = {255, 255, 255};

    RenderTextScreenCentred("> Obiettivo: prendere piu monete possibili",colorW , scrH-60, font30);
    RenderTextScreenCentred("> Bonus: cubi (SPEED, MAGNETE, +30 Punti)",colorW , scrH-120, font30);

    RenderTextScreenCentred("Come si gioca?", colorW, scrH - 300, font40);
    string buf;
    ifstream in("instr.txt");
    int offsetY = 360;
    while(getline(in,buf)) {
        RenderTextCoord(buf,colorW ,60, scrH-offsetY, font30);
        offsetY += 40;
    }

    in.close();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glFinish();
    SDL_GL_SwapWindow(win);
}

/* Esegue il Rendering della scena */
void rendering(SDL_Window *win) {
    if(status == 0) renderInitMenu(win);
    else if(status == 1) renderGame(win);
    else if(status == 2) renderInfo(win);
    else if(status == 3) renderFinalScore(win);
    else if(status == 4) renderInitMenu(win);

}

void RenderTextScreenCentred(string message, SDL_Color color, int y, TTF_Font *font) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    gluOrtho2D(0, scrW, 0, scrH); // m_Width and m_Height is the resolution of window
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    SDL_Surface *sFont = TTF_RenderText_Blended(font, message.c_str(), color);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFont->w, sFont->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, sFont->pixels);

    int x = (scrW - sFont->w) / 2;
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0);
        glVertex2f(x, y + sFont->h);
        glTexCoord2f(1, 0);
        glVertex2f(x + sFont->w, y + sFont->h);
        glTexCoord2f(1, 1);
        glVertex2f(x + sFont->w, y);
        glTexCoord2f(0, 1);
        glVertex2f(x, y);
    }
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glDeleteTextures(1, &texture);
    //TTF_CloseFont(font);
    SDL_FreeSurface(sFont);
    glColor3f(1, 1, 1);
}

// bool joinPlatform(Ship ship, Fuel fuel){
//     if(ship.py - fuel.py > 1.2) return false;
//     if(ship.py - fuel.py < 0.5) return false;
//     int scale = 3;
//     if(!(ship.px >= fuel.px - 12*scale)) return false;
//     if(!(ship.px <= fuel.px + 12*scale)) return false;
//     if(!(ship.pz >= fuel.pz - 12*scale)) return false;
//     if(!(ship.pz <= fuel.pz + 12*scale)) return false;

//     fuelLevel = 100;
//     return true;


// }

void idle(SDL_Window *win){
    
    // nessun evento: siamo IDLE
    Uint32 timeNow = SDL_GetTicks(); // che ore sono?
    if (timeLastInterval + fpsSampling < timeNow) {
        fps = 1000.0 * ((float) fpsNow) / (timeNow - timeLastInterval);
        fpsNow = 0;
        timeLastInterval = timeNow;
    }
    bool doneSomething = false;
    int guardia = 0; // sicurezza da loop infinito
    // finche' il tempo simulato e' rimasto indietro rispetto
    // al tempo reale...
    while (nstep * PHYS_SAMPLING_STEP < timeNow) {
        if (!pauseGame) s.DoStep();
        nstep++;
        doneSomething = true;
        timeNow = SDL_GetTicks();
        if (guardia++ > 1000) {
            done = true;
            break;
        } // siamo troppo lenti!
    }
    
    if (doneSomething)
        rendering(win);
    else {
        //NADA
    }
}

void startGame(){
    fuelLevel = 100;
    lvMagnete = 1;
    speed = false;
    bonusPoint = false;
    camera = 2;

    // cerr << "\nNEW GAME";
    // cerr << "LOADING COINS..";
    int NUM_COINS = 20;
    s.px = 350.2;
    s.py = 229.64;
    s.pz = 300;
    if(!coins.empty()) coins.erase(coins.begin(),coins.begin()+NUM_COINS);

    for(int i = 0; i < NUM_COINS; i++){
        float x = rand() % 491 + 9;
        float y = rand() % 70 + 250 ;
        float z = rand() % 490 + 8 ;

        coins.push_back(Coin(x,y,z));
    }


    int NUM_SURPISE = 6;
    if(!surprise.empty()) surprise.erase(surprise.begin(),surprise.begin()+NUM_SURPISE);

    for(int i = 0; i < NUM_SURPISE - 3; i++){
        float x = rand() % 250;
        float y = rand() % 70 + 250 ;
        float z = rand() % 490 + 8 ;
        GLfloat coord[3] = {x,y,z};
        surprise.push_back(Cube(3,coord));
    }


    for(int i = 3; i < NUM_SURPISE; i++){
        float x = rand() % 241 + 250;
        float y = rand() % 70 + 250 ;
        float z = rand() % 490 + 8 ;
        GLfloat coord[3] = {x,y,z};
        surprise.push_back(Cube(3,coord));
    }
    startTime = clock();

    // cerr << "\nSTART";

}

void RenderTextCoord(string message, SDL_Color color, int x, int y, TTF_Font *font) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    gluOrtho2D(0, scrW, 0, scrH); // m_Width and m_Height is the resolution of window
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    SDL_Surface *sFont = TTF_RenderText_Blended(font, message.c_str(), color);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFont->w, sFont->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, sFont->pixels);

    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0);
        glVertex2f(x, y + sFont->h);
        glTexCoord2f(1, 0);
        glVertex2f(x + sFont->w, y + sFont->h);
        glTexCoord2f(1, 1);
        glVertex2f(x + sFont->w, y);
        glTexCoord2f(0, 1);
        glVertex2f(x, y);
    }
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glDeleteTextures(1, &texture);
    //TTF_CloseFont(font);
    SDL_FreeSurface(sFont);
    glColor3f(1, 1, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

}


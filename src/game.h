#ifndef GAME_H
#define GAME_H

#include "tiles.h"
#include "tower.h"
#include "enemy.h"
#include "headsUpDisplay.h"

#define SCREEN_WIDTH TILESIZE*MAP_COLS
#define SCREEN_HEIGHT TILESIZE*MAP_ROWS
#define FPS 60

/*typedef struct keyHanlder {
    ;
} KeyH;*/

typedef struct gameModel {
    SDL_Window *win;
    SDL_Renderer *renderer;
    SDL_Event *event;
    TM *tileManager;
    TOWERS *towers;
    EM *enemies;
    HUD *hud;
    int state;
    unsigned int renderTime;
    bool debug;
    int money;
    int hearts;
} GameModel;

int gameLoop(GameModel *gm);
void update(GameModel *gm);
void render(GameModel *gm);

int loadGame(GameModel *gm);
void cleanup(GameModel *gm);

#endif
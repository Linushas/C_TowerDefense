#ifndef GAME_H
#define GAME_H

#include "tiles.h"
#include "tower.h"
#include "enemy.h"
#include "headsUpDisplay.h"

#define SCREEN_WIDTH TILESIZE*MAP_COLS
#define SCREEN_HEIGHT TILESIZE*MAP_ROWS
#define FPS 60

typedef struct gameModel {
    SDL_Window *win;
    SDL_Renderer *renderer;
    TM *tileManager;
    TOWERS *towers;
    EM *ememies;
    HUD *hud;
    int state;
    unsigned int renderTime;
    bool debug;
    int money;
} GameModel;

int gameLoop(SDL_Window* win);
void update(TM *tileManager, TOWERS *towers, EM *enemies, HUD *hud);
void render(TM *tileManager, TOWERS *towers, EM *enemies, HUD *hud);

int loadGame(SDL_Window* win, SDL_Renderer* renderer, TM *tileManager, TOWERS *towers, EM *enemies, HUD *hud);
void cleanup(SDL_Window* win, SDL_Renderer* renderer, TM *tileManager, TOWERS *towers, EM *enemies, HUD *hud);

#endif
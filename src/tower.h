#ifndef TOWER_H
#define TOWER_H

#include "tiles.h"
#include "enemy.h"

enum{CANON, SNIPER, FLAMETHROWER};

typedef struct tower {
    SDL_Texture *texture;
    int x, y;
    int type;
    int level;
    double angle;
} Tower;

typedef struct towers {
    Tower types[10];
    Tower inGame[50];
    int activeTowers;
    SDL_Renderer* renderer;
} TOWERS;

TOWERS initializeTowers(SDL_Renderer* renderer);
void newTower(TOWERS *towers, int x_pos, int y_pos);
int loadTowers(TOWERS *towers);
void updateTowers(TOWERS *towers, EM *enemies);
void drawTowers(TOWERS *towers);
bool isTower(TOWERS *towers, int x, int y);
void cleanupTowers(TOWERS *towers);

#endif
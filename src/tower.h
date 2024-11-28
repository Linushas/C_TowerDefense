#ifndef TOWER_H
#define TOWER_H

#include "tiles.h"

enum{CANON, SNIPER, FLAMETHROWER};

typedef struct tower {
    SDL_Texture *texture;
    int x, y;
    int type;
    int level;
} Tower;

typedef struct towers {
    Tower types[10];
    Tower inGame[50];
    int activeTowers;
    SDL_Renderer* renderer;
    void (*draw)(struct towers*);
} TOWERS;

TOWERS initializeTowers(SDL_Renderer* renderer);
void newTower(TOWERS *towers, int x_pos, int y_pos);
int loadTowers(TOWERS *towers);
void updateTowers(TOWERS *towers);
void drawTowers(TOWERS *towers);
void cleanupTowers(TOWERS *towers);

#endif
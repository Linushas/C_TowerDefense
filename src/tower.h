#ifndef TOWER_H
#define TOWER_H

#include "tiles.h"
#include "enemy.h"

typedef struct projectile {
    double x, y;
    double angle;
    double speed;
    int enemiesHit;
} Projectile;

typedef struct tower {
    SDL_Texture *texture[10];
    SDL_Texture *projTexture;
    int spriteState;
    int x, y;
    int type;
    int level;
    int price;
    double angle;
    Projectile proj[10];
    int projIndex;
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
void shoot(Tower *tower);
void updateTowers(TOWERS *towers, EM *enemies);
void drawProjectiles(TOWERS *towers);
void drawTowers(TOWERS *towers);
bool isTower(TOWERS *towers, int x, int y);
void cleanupTowers(TOWERS *towers);

#endif
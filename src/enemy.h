#ifndef ENEMY_H
#define ENEMY_H

#include "tiles.h"

enum{LEFT, DOWN, RIGHT, UP};

typedef struct coord {
    int x, y;
} Coord;

typedef struct enemy {
    SDL_Texture *texture;
    int x, y;
    int type;
    int level;
    int speed;
    int direction;
    int tilesTraveled;
} Enemy;

typedef struct enemyManager {
    Enemy types[10];
    Enemy inGame[50];
    int activeEnemies;
    SDL_Renderer* renderer;
} EM;

EM initializeEnemies(SDL_Renderer* renderer);
void newEnemy(EM *enemies, TM *tileManager, int x_pos, int y_pos);
int loadEnemies(EM *enemies);
void updateEnemies(EM *enemies, TM *tileManager);
void drawEnemies(EM *enemies);
void cleanupEnemies(EM *enemies);

#endif
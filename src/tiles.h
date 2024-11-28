#ifndef TILES_H
#define TILES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>

#define TILESIZE 48
#define MAP_COLS 16
#define MAP_ROWS 10

typedef struct tile {
    SDL_Texture *texture;
    bool isEnemyPath;
} Tile;

typedef struct tileManager {
    Tile tiles[10];
    int tileMap[MAP_ROWS][MAP_COLS];
    SDL_Renderer* renderer;
    void (*draw)(struct tileManager*);
} TM;

TM initializeTiles(SDL_Renderer* renderer);
int loadTiles(TM *tileManager);
int loadMap(char *path, int map[MAP_ROWS][MAP_COLS]);
void cleanupTiles(TM *tileManager);
void drawTiles(TM *tileManager);

#endif
#include <stdio.h>
#include "tower.h"

TOWERS initializeTowers(SDL_Renderer* renderer) {
    TOWERS towers;
    towers.renderer = renderer;
    towers.activeTowers = 0;

    loadTowers(&towers);

    return towers;
}

void newTower(TOWERS *towers, int x_pos, int y_pos) {
    bool isAvailable = true;
    for(int i = 0; i < towers->activeTowers; i++) {
        if(towers->inGame[i].x == x_pos && towers->inGame[i].y == y_pos) {
            isAvailable = false; 
            break;
        }
    }

    if(isAvailable) {
        towers->inGame[towers->activeTowers].angle = 30;
        towers->inGame[towers->activeTowers] = towers->types[0];
        towers->inGame[towers->activeTowers].type = 0;
        towers->inGame[towers->activeTowers].x = x_pos;
        towers->inGame[towers->activeTowers].y = y_pos;
        towers->inGame[towers->activeTowers].level = 1;
        (towers->activeTowers)++;
    }
}

int loadTowers(TOWERS *towers) {
    char *path1 = "res/images/tower1.png";

    towers->types[0].texture = IMG_LoadTexture(towers->renderer, path1);
    if (!towers->types[0].texture) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return false;
    }
    printf("%s loaded\n", path1);
}

void updateTowers(TOWERS *towers) {

}

void drawTowers(TOWERS *towers) {
    SDL_Texture *texture;

    for(int i = 0; i < towers->activeTowers; i++) {
        texture = towers->inGame[i].texture;
        SDL_Rect texture_rect = {towers->inGame[i].x*TILESIZE, towers->inGame[i].y*TILESIZE, TILESIZE, TILESIZE};
        //SDL_RenderCopy(towers->renderer, texture, NULL, &texture_rect); 
        SDL_RenderCopyEx(towers->renderer, towers->types[0].texture, NULL, &texture_rect, towers->inGame[i].angle, NULL, SDL_FLIP_NONE);
    }
}

bool isTower(TOWERS *towers, int x, int y) {
    for(int i = 0; i < towers->activeTowers; i++) {
        if(towers->inGame[i].x == x && towers->inGame[i].y == y)
            return true;
    }
    return false;
}

void cleanupTowers(TOWERS *towers) {
    SDL_DestroyTexture(towers->types[0].texture);

    for(int i = 0; i < towers->activeTowers; i++)
        SDL_DestroyTexture(towers->inGame[i].texture);
}
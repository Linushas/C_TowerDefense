#include <math.h>
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
        towers->inGame[towers->activeTowers].angle = 30.0;
        towers->inGame[towers->activeTowers] = towers->types[0];
        towers->inGame[towers->activeTowers].type = 0;
        towers->inGame[towers->activeTowers].x = x_pos;
        towers->inGame[towers->activeTowers].y = y_pos;
        towers->inGame[towers->activeTowers].level = 1;
        towers->inGame[towers->activeTowers].projIndex = 0;
        towers->inGame[towers->activeTowers].spriteState = 0;
        (towers->activeTowers)++;
    }
}

int loadTowers(TOWERS *towers) {
    char *path1 = "res/images/tower1.png";
    char *path2 = "res/images/projectile1.png";
    char *path3 = "res/images/tower1shoot.png";

    towers->types[0].texture[0] = IMG_LoadTexture(towers->renderer, path1);
    if (!towers->types[0].texture[0]) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return false;
    }
    printf("%s loaded\n", path1);

    towers->types[0].texture[1] = IMG_LoadTexture(towers->renderer, path3);
    if (!towers->types[0].texture[1]) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return false;
    }
    printf("%s loaded\n", path3);

    towers->types[0].projTexture = IMG_LoadTexture(towers->renderer, path2);
    if (!towers->types[0].projTexture) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return false;
    }
    printf("%s loaded\n", path2);
}

void shoot(Tower *tower) {
    if(tower->projIndex >= 10) tower->projIndex = 0;
    tower->proj[tower->projIndex].speed = 120;
    tower->proj[tower->projIndex].x = (double)tower->x * TILESIZE;
    tower->proj[tower->projIndex].y = (double)tower->y * TILESIZE;
    tower->proj[tower->projIndex].angle = tower->angle;
    (tower->projIndex)++;
}

void updateTowers(TOWERS *towers, EM *enemies) {
    static Enemy targetEnemy;
    int i = 0;
    do {
        targetEnemy = enemies->inGame[i];
        i++;
    } while(targetEnemy.isDead);

    for(int i = 0; i < towers->activeTowers; i++){
        double dx = targetEnemy.x - towers->inGame[i].x * TILESIZE;
        double dy = targetEnemy.y - towers->inGame[i].y * TILESIZE;
        double angle = atan2(dy, dx) * 180 / M_PI;
        if (angle < 0) angle += 360; 
        towers->inGame[i].angle = angle;
    }

    static int ticks = 0;
    if(ticks > 30) {
        ticks = 0;
        for(int i = 0; i < towers->activeTowers; i++) {
            shoot(&towers->inGame[i]);
        }
    }
    for(int i = 0; i < towers->activeTowers; i++) {
        if(ticks > 20) towers->inGame[i].spriteState = 1;
        else towers->inGame[i].spriteState = 0;    
    }
    ticks++;

    for(int i = 0; i < towers->activeTowers; i++) {
        for(int p = 0; p < towers->inGame[i].projIndex; p++) {
            int speed = towers->inGame[i].proj[p].speed;
            double angle = (towers->inGame[i].proj[p].angle) / (180 / M_PI);
            towers->inGame[i].proj[p].x += speed * cos(angle) * 0.1;
            towers->inGame[i].proj[p].y += speed * sin(angle) * 0.1;
        }
    }
}

void drawProjectiles(TOWERS *towers) {
    SDL_Texture *texture;

    for(int i = 0; i < towers->activeTowers; i++) {
        for(int p = 0; p < towers->inGame[i].projIndex; p++) {
            texture = towers->inGame[i].projTexture;
            SDL_Rect texture_rect = {(int)towers->inGame[i].proj[p].x, (int)towers->inGame[i].proj[p].y, TILESIZE, TILESIZE};
            SDL_RenderCopyEx(towers->renderer, texture, NULL, &texture_rect, towers->inGame[i].proj[p].angle, NULL, SDL_FLIP_NONE);
        }
    }
}

void drawTowers(TOWERS *towers) {
    SDL_Texture *texture;

    for(int i = 0; i < towers->activeTowers; i++) {
        texture = towers->inGame[i].texture[towers->inGame[i].spriteState];

        SDL_Rect texture_rect = {towers->inGame[i].x*TILESIZE, towers->inGame[i].y*TILESIZE, TILESIZE, TILESIZE};
        SDL_RenderCopyEx(towers->renderer, texture, NULL, &texture_rect, towers->inGame[i].angle, NULL, SDL_FLIP_NONE);
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
    SDL_DestroyTexture(towers->types[0].texture[0]);

    for(int i = 0; i < towers->activeTowers; i++)
        SDL_DestroyTexture(towers->inGame[i].texture[0]);
}
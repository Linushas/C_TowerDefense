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
        towers->inGame[towers->activeTowers].projSpeed = 120;
        towers->inGame[towers->activeTowers].timer = 0;
        towers->inGame[towers->activeTowers].reloadDelay = 30;
        towers->inGame[towers->activeTowers].spriteState = 0;
        (towers->activeTowers)++;
    }
}

int loadTowers(TOWERS *towers) {
    char *path1 = "res/images/tower1.png";
    char *path2 = "res/images/projectile1.png";
    char *path3 = "res/images/tower1shoot.png";
    char *path4 = "res/images/tower1lv2.png";

    towers->types[0].texture[0] = IMG_LoadTexture(towers->renderer, path1);
    if (!towers->types[0].texture[0]) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return false;
    }
    printf("%s loaded\n", path1);

    towers->types[0].texture[2] = IMG_LoadTexture(towers->renderer, path3);
    if (!towers->types[0].texture[2]) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return false;
    }
    printf("%s loaded\n", path3);

    towers->types[0].texture[1] = IMG_LoadTexture(towers->renderer, path4);
    if (!towers->types[0].texture[1]) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return false;
    }
    printf("%s loaded\n", path4);

    towers->types[0].projTexture = IMG_LoadTexture(towers->renderer, path2);
    if (!towers->types[0].projTexture) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return false;
    }
    printf("%s loaded\n", path2);
}

void shoot(Tower *tower) {
    if(tower->projIndex >= 10) tower->projIndex = 0;
    tower->proj[tower->projIndex].x = (double)tower->x * TILESIZE;
    tower->proj[tower->projIndex].y = (double)tower->y * TILESIZE;
    tower->proj[tower->projIndex].angle = tower->angle;
    tower->proj[tower->projIndex].enemiesHit = 0;
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

    for(int i = 0; i < towers->activeTowers; i++) {
        if(towers->inGame[i].timer > towers->inGame[i].reloadDelay) {
            towers->inGame[i].timer = 0;
            shoot(&towers->inGame[i]);
        }  
        (towers->inGame[i].timer)++;
    }

    for(int i = 0; i < towers->activeTowers; i++) {
        for(int p = 0; p < towers->inGame[i].projIndex; p++) {
            int speed = towers->inGame[i].projSpeed;
            double angle = (towers->inGame[i].proj[p].angle) / (180 / M_PI);
            towers->inGame[i].proj[p].x += speed * cos(angle) * 0.1;
            towers->inGame[i].proj[p].y += speed * sin(angle) * 0.1;
        }
    }
}

void upgradeTower(Tower *tower) {
    (tower->level)++;
    switch(tower->level) {
        case 1: 
            tower->projSpeed = 120;
            tower->reloadDelay = 30;
            break;
        case 2: 
            tower->projSpeed = 120;
            tower->reloadDelay = 15;
            break;
        case 2: 
            tower->projSpeed = 120;
            tower->reloadDelay = 25;
            break;
    }
}

void drawProjectiles(TOWERS *towers) {
    SDL_Texture *texture;

    for(int i = 0; i < towers->activeTowers; i++) {
        for(int p = 0; p < towers->inGame[i].projIndex; p++) {
            if(towers->inGame[i].proj[p].enemiesHit == 0) {
                texture = towers->inGame[i].projTexture;
                SDL_Rect texture_rect = {(int)towers->inGame[i].proj[p].x, (int)towers->inGame[i].proj[p].y, TILESIZE, TILESIZE};
                SDL_RenderCopyEx(towers->renderer, texture, NULL, &texture_rect, towers->inGame[i].proj[p].angle, NULL, SDL_FLIP_NONE);
            }
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
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
        towers->inGame[towers->activeTowers] = towers->types[0];
        towers->inGame[towers->activeTowers].angle = 30.0;
        towers->inGame[towers->activeTowers].type = 0;
        towers->inGame[towers->activeTowers].x = x_pos;
        towers->inGame[towers->activeTowers].y = y_pos;
        towers->inGame[towers->activeTowers].level = 1;
        towers->inGame[towers->activeTowers].projIndex = 0;
        towers->inGame[towers->activeTowers].projSpeed = 120;
        towers->inGame[towers->activeTowers].timer = 0;
        towers->inGame[towers->activeTowers].reloadDelay = 60;
        towers->inGame[towers->activeTowers].spriteState = 0;
        (towers->activeTowers)++;
    }
}

int loadTowers(TOWERS *towers) {
    char *path = "res/images/tower01.png";
    char *pathProjectile = "res/images/projectile1.png";

    SDL_Texture *sheet = IMG_LoadTexture(towers->renderer, path);
    if (!sheet) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return false;
    }
    printf("%s loaded\n", path);

    int rows = 5, cols = 2;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int spriteIndex = i * cols + j;
            if (spriteIndex >= 50) {
                break;
            }

            SDL_Rect cropRect = {j * 32, i * 32, 32, 32};
            towers->types[0].texture[spriteIndex] = SDL_CreateTexture(
                towers->renderer,
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET,
                TILESIZE,
                TILESIZE
            );
            if (!towers->types[0].texture[spriteIndex]) {
                printf("Failed to create tile texture: %s\n", SDL_GetError());
                SDL_DestroyTexture(sheet);
                return false;
            }
            
            SDL_SetRenderTarget(towers->renderer, towers->types[0].texture[spriteIndex]);
            SDL_SetTextureBlendMode(towers->types[0].texture[spriteIndex], SDL_BLENDMODE_BLEND);

            if (SDL_RenderCopy(towers->renderer, sheet, &cropRect, NULL) < 0) {
                printf("Failed to render copy tile: %s\n", SDL_GetError());
                SDL_DestroyTexture(sheet);
                return false;
            }

            SDL_SetRenderTarget(towers->renderer, NULL);

            if (!towers->types[0].texture[spriteIndex]) {
                printf("Texture creation failed at sprite %d: %s\n", spriteIndex, SDL_GetError());
            }

        }
    }

    SDL_DestroyTexture(sheet);

    towers->types[0].projTexture = IMG_LoadTexture(towers->renderer, pathProjectile);
    if (!sheet) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return false;
    }
    printf("%s loaded\n", pathProjectile);
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
        if(towers->inGame[i].timer > towers->inGame[i].reloadDelay * 2/3) {
            towers->inGame[i].spriteState = towers->inGame[i].level*2 - 1;
        }
        else {
            towers->inGame[i].spriteState = towers->inGame[i].level*2 - 2;
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
            tower->reloadDelay = 60;
            break;
        case 2: 
            tower->projSpeed = 120;
            tower->reloadDelay = 30;
            break;
        case 3: 
            tower->projSpeed = 120;
            tower->reloadDelay = 30;
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

int isTower(TOWERS *towers, int x, int y) {
    for(int i = 0; i < towers->activeTowers; i++) {
        if(towers->inGame[i].x == x && towers->inGame[i].y == y)
            return i;
    }
    return -1;
}

void cleanupTowers(TOWERS *towers) {
    SDL_DestroyTexture(towers->types[0].texture[0]);

    for(int i = 0; i < towers->activeTowers; i++)
        SDL_DestroyTexture(towers->inGame[i].texture[0]);
}
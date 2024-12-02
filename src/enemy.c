#include <stdio.h>
#include "enemy.h"

EM initializeEnemies(SDL_Renderer* renderer) {
    EM enemies;
    enemies.renderer = renderer;
    enemies.activeEnemies = 0;
    
    loadEnemies(&enemies);

    return enemies;
}

void newEnemy(EM *enemies, TM *tileManager, int x_pos, int y_pos) {
    int tileX = x_pos / TILESIZE;
    int tileY = y_pos / TILESIZE;

    if (tileManager->tileMap[tileY][tileX] != 1) {
        printf("Invalid spawn position (%d, %d). Must be on a path tile.\n", tileX, tileY);
        return;
    }

    enemies->inGame[enemies->activeEnemies].texture = enemies->types[0].texture;
    enemies->inGame[enemies->activeEnemies].type = 0;
    enemies->inGame[enemies->activeEnemies].isDead = false;
    enemies->inGame[enemies->activeEnemies].speed = 3;
    enemies->inGame[enemies->activeEnemies].level = 0;
    enemies->inGame[enemies->activeEnemies].direction = RIGHT;
    enemies->inGame[enemies->activeEnemies].x = x_pos;
    enemies->inGame[enemies->activeEnemies].y = y_pos;
    enemies->inGame[enemies->activeEnemies].y = y_pos;
    enemies->inGame[enemies->activeEnemies].tilesTraveled = 0;
    enemies->inGame[enemies->activeEnemies].hp = 4;

    (enemies->activeEnemies)++;
}

int loadEnemies(EM *enemies) {
    char *path1 = "res/images/enemy1.png";
    char *path2 = "res/images/enemy1pop.png";

    enemies->types[0].texture = IMG_LoadTexture(enemies->renderer, path1);
    if (!enemies->types[0].texture) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return false;
    }
    printf("%s loaded\n", path1);

    enemies->lowHPTexture = IMG_LoadTexture(enemies->renderer, path2);
    if (!enemies->lowHPTexture) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return false;
    }
    printf("%s loaded\n", path2);
}

void updateEnemies(EM *enemies, TM *tileManager) {
    for (int i = 0; i < enemies->activeEnemies; i++) {
        Enemy *enemy = &enemies->inGame[i];

        int centerX = (enemy->x + TILESIZE / 2) / TILESIZE;
        int centerY = (enemy->y + TILESIZE / 2) / TILESIZE;

        int currentTileCenterX = centerX * TILESIZE + TILESIZE / 2;
        int currentTileCenterY = centerY * TILESIZE + TILESIZE / 2;

        bool changeDirection = false;

        bool alignedX = abs(enemy->x + TILESIZE / 2 - currentTileCenterX) < enemy->speed;
        bool alignedY = abs(enemy->y + TILESIZE / 2 - currentTileCenterY) < enemy->speed;

        if (alignedX && alignedY) {

            switch (enemy->direction) {
                case UP:
                    if (centerY > 0 && tileManager->tileMap[centerY - 1][centerX] != 1)
                        changeDirection = true;
                    break;
                case DOWN:
                    if (centerY < MAP_ROWS - 1 && tileManager->tileMap[centerY + 1][centerX] != 1)
                        changeDirection = true;
                    break;
                case LEFT:
                    if (centerX > 0 && tileManager->tileMap[centerY][centerX - 1] != 1)
                        changeDirection = true;
                    break;
                case RIGHT:
                    if (centerX < MAP_COLS - 1 && tileManager->tileMap[centerY][centerX + 1] != 1)
                        changeDirection = true;
                    break;
            }

            if (changeDirection) {
                if (enemy->direction == RIGHT || enemy->direction == LEFT) {
                    if (centerY > 0 && tileManager->tileMap[centerY - 1][centerX] == 1) {
                        enemy->direction = UP;
                    } else if (centerY < MAP_ROWS - 1 && tileManager->tileMap[centerY + 1][centerX] == 1) {
                        enemy->direction = DOWN;
                    }
                } else if (enemy->direction == UP || enemy->direction == DOWN) {
                    if (centerX > 0 && tileManager->tileMap[centerY][centerX - 1] == 1) {
                        enemy->direction = LEFT;
                    } else if (centerX < MAP_COLS - 1 && tileManager->tileMap[centerY][centerX + 1] == 1) {
                        enemy->direction = RIGHT;
                    }
                }
            }
        }

        switch (enemy->direction) {
            case UP:
                enemy->y -= enemy->speed;
                break;
            case DOWN:
                enemy->y += enemy->speed;
                break;
            case LEFT:
                enemy->x -= enemy->speed;
                break;
            case RIGHT:
                enemy->x += enemy->speed;
                break;
        }
    }

    static int ticks = 0;
    if(ticks > 100) {
        ticks = 0;
        if(enemies->activeEnemies < 40)
            newEnemy(enemies, tileManager, 0*TILESIZE, 3*TILESIZE);
    }
    ticks++;
}

Enemy *isEnemy(EM *enemies) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    for(int i = 0; i < enemies->activeEnemies; i++) {
        Enemy *e = &enemies->inGame[i];
        if (!e->isDead && mouseX >= e->x && mouseX <= e->x + TILESIZE &&
            mouseY >= e->y && mouseY <= e->y + TILESIZE) {
            return e;
        }
    }
    return NULL;
}

void drawEnemies(EM *enemies) {
    SDL_Texture *texture;

    for(int i = 0; i < enemies->activeEnemies; i++) {
        if(!enemies->inGame[i].isDead) {
            texture = enemies->inGame[i].texture;
            SDL_Rect texture_rect = {enemies->inGame[i].x, enemies->inGame[i].y, TILESIZE, TILESIZE};
            SDL_RenderCopy(enemies->renderer, texture, NULL, &texture_rect); 

            if(enemies->inGame[i].hp <= 2) {
                texture = enemies->lowHPTexture;
                SDL_Rect texture_rect = {enemies->inGame[i].x, enemies->inGame[i].y, TILESIZE, TILESIZE};
                SDL_RenderCopy(enemies->renderer, texture, NULL, &texture_rect); 
            }
        }
    }
}

void cleanupEnemies(EM *enemies) {
    SDL_DestroyTexture(enemies->types[0].texture);

    for(int i = 0; i < enemies->activeEnemies; i++)
        SDL_DestroyTexture(enemies->inGame[i].texture);
}
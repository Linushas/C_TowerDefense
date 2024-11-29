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
    enemies->inGame[enemies->activeEnemies].speed = 3;
    enemies->inGame[enemies->activeEnemies].level = 0;
    enemies->inGame[enemies->activeEnemies].direction = RIGHT;
    enemies->inGame[enemies->activeEnemies].x = x_pos;
    enemies->inGame[enemies->activeEnemies].y = y_pos;
    enemies->inGame[enemies->activeEnemies].y = y_pos;
    enemies->inGame[enemies->activeEnemies].tilesTraveled = 0;

    (enemies->activeEnemies)++;
}

int loadEnemies(EM *enemies) {
    char *path1 = "res/images/enemy1.png";

    enemies->types[0].texture = IMG_LoadTexture(enemies->renderer, path1);
    if (!enemies->types[0].texture) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return false;
    }
    printf("%s loaded\n", path1);
}

/*
bool isForward(int direction, int pathX[], int pathY[], int tilesTraveled) {
    int x = pathX[tilesTraveled]
}*/

void updateEnemies(EM *enemies, TM *tileManager) {
    int pathX[34] = {0,1,2,2,2,3,4,4,4,4,5,6,7,8,9,10,11,12,13,13,13,12,11,10,10,10,10,10,10,11,12,13,14,15};
    int pathY[34] = {3,3,3,2,1,1,1,2,3,4,4,4,4,4,4,4, 4, 4, 4, 3, 2, 2, 2, 2, 3, 4, 5 ,6, 7, 7, 7, 7, 7, 7};
     
    for (int i = 0; i < enemies->activeEnemies; i++) {
        Enemy *enemy = &enemies->inGame[i];

        int tileX = enemy->x / TILESIZE;
        int tileY = enemy->y / TILESIZE;
        bool changeDirection = false;

        switch (enemy->direction) {
            case UP:
                if(tileManager->tileMap[tileY - 1][tileX] != 1)
                    changeDirection = true;
                break;
            case DOWN:
                if(tileManager->tileMap[tileY + 1][tileX] != 1)
                    changeDirection = true;
                break;
            case LEFT:
                if(tileManager->tileMap[tileY][tileX - 1] != 1)
                    changeDirection = true;
                break;
            case RIGHT:
                if(tileManager->tileMap[tileY][tileX + 1] != 1)
                    changeDirection = true;
                break;
        }

        if(changeDirection) {
            if (tileY > 0 && tileManager->tileMap[tileY - 1][tileX] == 1 
            && (tileX != pathX[enemy->tilesTraveled] && tileY-1 != pathY[enemy->tilesTraveled])) {
                enemy->direction = UP;
            } 
            else if (tileY < MAP_ROWS - 1 && tileManager->tileMap[tileY + 1][tileX] == 1 
            && (tileX != pathX[enemy->tilesTraveled] && tileY+1 != pathY[enemy->tilesTraveled])) {
                enemy->direction = DOWN;
            } 
            else if (tileX > 0 && tileManager->tileMap[tileY][tileX - 1] == 1 
            && (tileX-1 != pathX[enemy->tilesTraveled] && tileY != pathY[enemy->tilesTraveled])) {
                enemy->direction = LEFT;
            } 
            else if (tileX < MAP_COLS - 1 && tileManager->tileMap[tileY][tileX + 1] == 1 
            && (tileX+1 != pathX[enemy->tilesTraveled] && tileY != pathY[enemy->tilesTraveled])) {
                enemy->direction = RIGHT;
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
}



void drawEnemies(EM *enemies) {
    SDL_Texture *texture;

    for(int i = 0; i < enemies->activeEnemies; i++) {
        texture = enemies->inGame[i].texture;
        SDL_Rect texture_rect = {enemies->inGame[i].x, enemies->inGame[i].y, TILESIZE, TILESIZE};
        SDL_RenderCopy(enemies->renderer, texture, NULL, &texture_rect); 
    }
}

void cleanupEnemies(EM *enemies) {
    SDL_DestroyTexture(enemies->types[0].texture);

    for(int i = 0; i < enemies->activeEnemies; i++)
        SDL_DestroyTexture(enemies->inGame[i].texture);
}
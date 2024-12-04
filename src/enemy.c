#include <stdio.h>
#include "enemy.h"

EM initializeEnemies(SDL_Renderer* renderer) {
    EM enemies;
    enemies.renderer = renderer;
    enemies.activeEnemies = 0;
    enemies.currentWave = 1;
    
    loadEnemies(&enemies);

    return enemies;
}

void newEnemy(EM *enemies, TM *tileManager, int type, int x_pos, int y_pos, int speed) {
    int tileX = x_pos / TILESIZE;
    int tileY = y_pos / TILESIZE;

    if (tileManager->tiles[tileManager->tileMap[tileY][tileX]].isEnemyPath == false) {
        printf("Invalid spawn position (%d, %d). Must be on a path tile.\n", tileX, tileY);
        return;
    }

    switch(type) {
        case 0:
            enemies->inGame[enemies->activeEnemies].type = 0;
            enemies->inGame[enemies->activeEnemies].maxHP = 4;
            enemies->inGame[enemies->activeEnemies].hp = 4;
            break;
        case 1:
            enemies->inGame[enemies->activeEnemies].type = 1;
            enemies->inGame[enemies->activeEnemies].maxHP = 8;
            enemies->inGame[enemies->activeEnemies].hp = 8;
            break;
        default: 
            enemies->inGame[enemies->activeEnemies].type = 1;
            enemies->inGame[enemies->activeEnemies].maxHP = 4;
            enemies->inGame[enemies->activeEnemies].hp = 4;
    }

    enemies->inGame[enemies->activeEnemies].x = x_pos;
    enemies->inGame[enemies->activeEnemies].y = y_pos;
    enemies->inGame[enemies->activeEnemies].tilesTraveled = 0;
    enemies->inGame[enemies->activeEnemies].direction = RIGHT;
    enemies->inGame[enemies->activeEnemies].lastHorizontalDirection = RIGHT;
    enemies->inGame[enemies->activeEnemies].speed = speed;
    enemies->inGame[enemies->activeEnemies].isDead = false;
    enemies->inGame[enemies->activeEnemies].currentTexture = 0;

    (enemies->activeEnemies)++;
}

int loadEnemies(EM *enemies) {
    char *path = "res/images/snails.png";

    SDL_Texture *sheet = IMG_LoadTexture(enemies->renderer, path);
    if (!sheet) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return false;
    }
    printf("%s loaded\n", path);

    int rows = 4, cols = 16;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int spriteIndex = i * cols + j;
            if (spriteIndex >= 50) {
                break;
            }

            SDL_Rect cropRect = {j * 32, i * 32, 32, 32};
            enemies->textures[spriteIndex] = SDL_CreateTexture(
                enemies->renderer,
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET,
                TILESIZE,
                TILESIZE
            );
            if (!enemies->textures[spriteIndex]) {
                printf("Failed to create sprite texture: %s\n", SDL_GetError());
                SDL_DestroyTexture(sheet);
                return false;
            }
            
            SDL_SetRenderTarget(enemies->renderer, enemies->textures[spriteIndex]);
            SDL_SetTextureBlendMode(enemies->textures[spriteIndex], SDL_BLENDMODE_BLEND);

            if (SDL_RenderCopy(enemies->renderer, sheet, &cropRect, NULL) < 0) {
                printf("Failed to render copy sprite: %s\n", SDL_GetError());
                SDL_DestroyTexture(sheet);
                return false;
            }

            SDL_SetRenderTarget(enemies->renderer, NULL);

            if (!enemies->textures[spriteIndex]) {
                printf("Texture creation failed at sprite %d: %s\n", spriteIndex, SDL_GetError());
            }

        }
    }

    SDL_DestroyTexture(sheet);
}

void updateEnemies(EM *enemies, TM *tileManager) {
    static int ticks = 0;
    
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
                    if (centerY > 0 && tileManager->tiles[tileManager->tileMap[centerY - 1][centerX]].isEnemyPath == false)
                        changeDirection = true;
                    break;
                case DOWN:
                    if (centerY < MAP_ROWS - 1 && tileManager->tiles[tileManager->tileMap[centerY + 1][centerX]].isEnemyPath == false)
                        changeDirection = true;
                    break;
                case LEFT:
                    if (centerX > 0 && tileManager->tiles[tileManager->tileMap[centerY][centerX - 1]].isEnemyPath == false)
                        changeDirection = true;
                    break;
                case RIGHT:
                    if (centerX < MAP_COLS - 1 && tileManager->tiles[tileManager->tileMap[centerY][centerX + 1]].isEnemyPath == false)
                        changeDirection = true;
                    break;
            }

            if (changeDirection) {
                if (enemy->direction == RIGHT || enemy->direction == LEFT) {
                    if (centerY > 0 && tileManager->tiles[tileManager->tileMap[centerY - 1][centerX]].isEnemyPath) {
                        enemy->direction = UP;
                    } else if (centerY < MAP_ROWS - 1 && tileManager->tiles[tileManager->tileMap[centerY + 1][centerX]].isEnemyPath) {
                        enemy->direction = DOWN;
                    }
                } else if (enemy->direction == UP || enemy->direction == DOWN) {
                    if (centerX > 0 && tileManager->tiles[tileManager->tileMap[centerY][centerX - 1]].isEnemyPath) {
                        enemy->direction = LEFT;
                    } else if (centerX < MAP_COLS - 1 && tileManager->tiles[tileManager->tileMap[centerY][centerX + 1]].isEnemyPath) {
                        enemy->direction = RIGHT;
                    }
                }
            }
        }

        int hp = 0;
        if( (float)((float)enemy->hp / (float)enemy-> maxHP) < 0.90 )
            hp = 6;
        if( (float)((float)enemy->hp / (float)enemy-> maxHP) < 0.50 )
            hp = 12;

        switch (enemy->direction) {
            case UP:
                if(enemy->lastHorizontalDirection == RIGHT)
                    enemy->currentTexture = (enemy->type*16) + 3 + hp;
                else enemy->currentTexture = (enemy->type*16) + 2 + hp;

                enemy->y -= enemy->speed;
                break;
            case DOWN:
                if(ticks >= 30 && hp != 12) {
                    if(enemy->lastHorizontalDirection == RIGHT)
                        enemy->currentTexture = (enemy->type*16) + 5 + hp;
                    else enemy->currentTexture = (enemy->type*16) + 4 + hp;
                }
                else {
                    if(enemy->lastHorizontalDirection == RIGHT)
                        enemy->currentTexture = (enemy->type*16) + 1 + hp;
                    else enemy->currentTexture = (enemy->type*16) + 0 + hp;
                }
                
                enemy->y += enemy->speed;
                break;
            case LEFT:
                enemy->lastHorizontalDirection = LEFT;

                if(ticks >= 30 && hp != 12) 
                    enemy->currentTexture = (enemy->type*16) + 4 + hp;
                else enemy->currentTexture = (enemy->type*16) + 0 + hp;

                enemy->x -= enemy->speed;
                break;
            case RIGHT:
                enemy->lastHorizontalDirection = RIGHT;

                if(ticks >= 30 && hp != 12)
                    enemy->currentTexture = (enemy->type*16) + 5 + hp;
                else enemy->currentTexture = (enemy->type*16) + 1 + hp;

                enemy->x += enemy->speed;
                break;
        }

        
    }
    if(ticks >= 36) ticks = 0;
    ticks++;
    spawnEnemies(enemies, tileManager);
}

void spawnEnemies(EM *enemies, TM *tileManager) {
    static int ticks = 0;
    static bool wait = true;

    static bool nextRound = false;
    bool isAllDead = true;
    for(int i = 0; i < enemies->activeEnemies; i++) {
        if(enemies->inGame[i].isDead == false) {
            isAllDead = false;
            break;
        }
    }

    if((wait && ticks < 6*60) || (wait && !isAllDead)) {
        ticks++;
        return;
    }
    if(nextRound) {
        ticks = 0;
        enemies->activeEnemies = 0;
        (enemies->currentWave)++;
        nextRound = false;
    }
    wait = false;

    switch(enemies->currentWave) {
        case 1: 
            if(ticks > 140) {
                if(enemies->activeEnemies < 10) {
                    ticks = 0;
                    newEnemy(enemies, tileManager, 0, 0*TILESIZE, 4*TILESIZE, 2);
                    wait = false;
                }
                else {
                    nextRound = true;
                    wait = true;
                }
            }
            break;
        case 2: 
            if(ticks > 100) {
                if(enemies->activeEnemies < 4) {
                    ticks = 0;
                    newEnemy(enemies, tileManager, 0, 0*TILESIZE, 4*TILESIZE, 4);
                    wait = false;
                }
                else {
                    nextRound = true;
                    wait = true;
                }
            }
            break;
        case 3: 
            if(ticks > 100) {
                if(enemies->activeEnemies < 20) {
                    ticks = 0;
                    newEnemy(enemies, tileManager, 1, 0*TILESIZE, 4*TILESIZE, 3);
                    wait = false; 
                }
                else {
                    nextRound = true;
                    wait = true;
                }
            }
            break;
        case 4: 
            if(ticks > 80) {
                if(enemies->activeEnemies < 10) {
                    ticks = 0;
                    newEnemy(enemies, tileManager, 1, 0*TILESIZE, 4*TILESIZE, 3);
                    wait = false;
                }
                else {
                    nextRound = true;
                    wait = true;
                }
            }
            break;
        case 5: 
            if(ticks > 120) {
                if(enemies->activeEnemies < 20) {
                    ticks = 0;
                    newEnemy(enemies, tileManager, 1, 0*TILESIZE, 4*TILESIZE, 4);
                    wait = false;
                }
                else {
                    nextRound = true;
                    wait = true;
                }
            }
            break;
        case 6: 
            if(ticks > 10) {
                if(enemies->activeEnemies < 40) {
                    ticks = 0;
                    newEnemy(enemies, tileManager, 1, 0*TILESIZE, 4*TILESIZE, 5);
                    wait = false;
                }
                else {
                    nextRound = true;
                    wait = true;
                }
            }
            break;
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

    for (int i = enemies->activeEnemies - 1; i >= 0; i--) {
        if(!enemies->inGame[i].isDead) {
            texture = enemies->textures[enemies->inGame[i].currentTexture];
            SDL_Rect texture_rect = {enemies->inGame[i].x, enemies->inGame[i].y, TILESIZE, TILESIZE};
            SDL_RenderCopy(enemies->renderer, texture, NULL, &texture_rect); 
        }
    }
}

void cleanupEnemies(EM *enemies) {
    for(int i = 0; i < 64; i++)
        SDL_DestroyTexture(enemies->textures[i]);
}
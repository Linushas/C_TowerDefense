#include <stdio.h>
#include <math.h>
#include "game.h"

int gameLoop(SDL_Window* win) {
    SDL_Renderer* renderer;
    TM tileManager;
    TOWERS towers;
    EM enemies;
    HUD hud;
    SDL_Event event;
    int running = loadGame(win, renderer, &tileManager, &towers, &enemies, &hud);
    hud.money = 400;

    // MAIN GAME LOOP
    while(running) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT: 
                    running = false; 
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(hud.state == UPGRADE_STATE || hud.state == NEW_TOWER_STATE) {
                        updateHUD(&hud, &towers, &tileManager);
                    }
                    else {
                        towers.selectedTowerIndex = isTower(&towers, tileManager.selectedCol, tileManager.selectedRow);
                        if(towers.selectedTowerIndex != -1) {
                            hud.state = UPGRADE_STATE;
                        }
                        else if(tileManager.tiles[tileManager.selectedTileID].isEnemyPath == false) {
                            hud.state = NEW_TOWER_STATE;
                        }
                    }
                    
                    break;
            }

        }
        
        update(&tileManager, &towers, &enemies, &hud);
        render(&tileManager, &towers, &enemies, &hud);

        SDL_Delay(1000/FPS);
    }

    cleanup(win, renderer, &tileManager, &towers, &enemies, &hud);
    return true;
}

void update(TM *tileManager, TOWERS *towers, EM *enemies, HUD *hud) {
    int mouseX, mouseY;
    Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);
    if(hud->state == NEW_TOWER_STATE || hud->state == UPGRADE_STATE) {
        hud->mouseX = mouseX;
        hud->mouseY = mouseY;
    }
    else {
        tileManager->selectedCol = mouseX / TILESIZE;
        tileManager->selectedRow = mouseY / TILESIZE;
        tileManager->selectedTileID = tileManager->tileMap[tileManager->selectedRow][tileManager->selectedCol];
    }

    updateTowers(towers, enemies);
    updateEnemies(enemies, tileManager);

    for(int enemy = 0; enemy < enemies->activeEnemies; enemy++) {
        for(int i = 0; i < towers->activeTowers; i++) {
            for(int p = 0; p < towers->inGame[i].projIndex; p++){
                if(enemies->inGame[enemy].x >= (int)towers->inGame[i].proj[p].x
                && enemies->inGame[enemy].x <= (int)towers->inGame[i].proj[p].x + TILESIZE
                && enemies->inGame[enemy].y >= (int)towers->inGame[i].proj[p].y
                && enemies->inGame[enemy].y <= (int)towers->inGame[i].proj[p].y + TILESIZE 
                && enemies->inGame[enemy].isDead == false) {
                    if(towers->inGame[i].proj[p].enemiesHit == 0) {
                        enemies->inGame[enemy].hp -= towers->inGame[i].damage;
                    }
                    (towers->inGame[i].proj[p].enemiesHit)++;
                    if(enemies->inGame[enemy].hp <= 0) {
                        enemies->inGame[enemy].isDead = true;
                        hud->money += 20;
                    }
                }
            }
            
        }
    }
}

void render(TM *tileManager, TOWERS *towers, EM *enemies, HUD *hud) {
    SDL_SetRenderDrawColor(tileManager->renderer, 0, 0, 0, 0);
    SDL_RenderClear(tileManager->renderer);

    drawTiles(tileManager);
    drawProjectiles(towers);
    drawTowers(towers);
    drawEnemies(enemies);
    drawHUD(hud, towers);

    SDL_RenderPresent(tileManager->renderer);
}

int loadGame(SDL_Window* win, SDL_Renderer* renderer, TM *tileManager, TOWERS *towers, EM *enemies, HUD *hud) {
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (!renderer) {
        printf("Could not create renderer: %s\n", SDL_GetError());
        return false;
    }
    printf("Renderer created\n");

    *tileManager = initializeTiles(renderer);
    if(tileManager == NULL){
        printf("could not load tiles\n");
        return false;
    } 
    printf("Tiles initialized\n");

    *towers = initializeTowers(renderer);
    if(towers == NULL){
        printf("could not load towers\n");
        return false;
    } 
    printf("Towers initialized\n");

    *enemies = initializeEnemies(renderer);
    if(enemies == NULL){
        printf("could not load enemies\n");
        return false;
    } 

    *hud = initializeHUD(renderer);
    if(hud == NULL){
        printf("could not load HUD\n");
        return false;
    } 
    printf("HUD initialized\n");

    return true;
}

void cleanup(SDL_Window* win, SDL_Renderer* renderer, TM *tileManager, TOWERS *towers, EM *enemies, HUD *hud){
    printf("cleaning up...\n");
    cleanupTiles(tileManager);
    cleanupTowers(towers);
    cleanupEnemies(enemies);
    cleanupHUD(hud);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    SDL_DestroyWindow(win);
    SDL_Quit();
    printf("Done!\n");
}
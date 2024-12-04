#include <stdio.h>
#include <math.h>
#include "game.h"

int gameLoop(GameModel *gm) { 
    SDL_Renderer *renderer;
    SDL_Event event;
    TM tileManager;
    TOWERS towers;
    EM enemies;
    HUD hud;
    gm->renderer = renderer;
    gm->event = &event;
    gm->tileManager = &tileManager;
    gm->towers = &towers;
    gm->enemies = &enemies;
    gm->hud = &hud;

    int running = loadGame(gm);
    hud.money = 440;
    hud.debug = false;
    unsigned int time = SDL_GetTicks();

    // MAIN GAME LOOP
    while(running) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT: 
                    running = false; 
                    break;
                case SDL_KEYDOWN: 
                    if(event.key.keysym.sym == SDLK_o) 
                        hud.debug = !hud.debug;
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
        
        update(gm);
        render(gm);

        if(hud.debug) {
            unsigned int now = SDL_GetTicks();
            hud.renderTime = now - time;
            time = now;
        }
        
        SDL_Delay(1000/FPS);
    }

    cleanup(gm);
    return true;
}

void update(GameModel *gm) {
    int mouseX, mouseY;
    Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);
    if(gm->hud->state == NEW_TOWER_STATE || gm->hud->state == UPGRADE_STATE) {
        gm->hud->mouseX = mouseX;
        gm->hud->mouseY = mouseY;
    }
    else {
        gm->tileManager->selectedCol = mouseX / TILESIZE;
        gm->tileManager->selectedRow = mouseY / TILESIZE;
        gm->tileManager->selectedTileID = gm->tileManager->tileMap[gm->tileManager->selectedRow][gm->tileManager->selectedCol];
    }

    updateTowers(gm->towers, gm->enemies);
    updateEnemies(gm->enemies, gm->tileManager);

    for(int enemy = 0; enemy < gm->enemies->activeEnemies; enemy++) {
        for(int i = 0; i < gm->towers->activeTowers; i++) {
            for(int p = 0; p < gm->towers->inGame[i].projIndex; p++){
                if(gm->enemies->inGame[enemy].x >= (int)gm->towers->inGame[i].proj[p].x
                && gm->enemies->inGame[enemy].x <= (int)gm->towers->inGame[i].proj[p].x + TILESIZE
                && gm->enemies->inGame[enemy].y >= (int)gm->towers->inGame[i].proj[p].y
                && gm->enemies->inGame[enemy].y <= (int)gm->towers->inGame[i].proj[p].y + TILESIZE 
                && gm->enemies->inGame[enemy].isDead == false) {
                    if(gm->towers->inGame[i].proj[p].enemiesHit == 0) {
                        gm->enemies->inGame[enemy].hp -= gm->towers->inGame[i].damage;
                    }
                    (gm->towers->inGame[i].proj[p].enemiesHit)++;
                    if(gm->enemies->inGame[enemy].hp <= 0) {
                        gm->enemies->inGame[enemy].isDead = true;
                        gm->hud->money += 20;
                    }
                }
            }
            
        }
    }
}

void render(GameModel *gm) {
    SDL_SetRenderDrawColor(gm->renderer, 0, 0, 0, 0);
SDL_RenderClear(gm->renderer);

    drawTiles(gm->tileManager);
    drawProjectiles(gm->towers);
    drawTowers(gm->towers);
    drawEnemies(gm->enemies);
    drawHUD(gm->hud, gm->towers, gm->enemies);

    SDL_RenderPresent(gm->renderer);
}

int loadGame(GameModel *gm) {
    gm->renderer = SDL_CreateRenderer(gm->win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(gm->renderer, SDL_BLENDMODE_BLEND);
    if (!gm->renderer) {
        printf("Could not create renderer: %s\n", SDL_GetError());
        return false;
    }
    printf("Renderer created\n");

    *gm->tileManager = initializeTiles(gm->renderer);
    if(gm->tileManager == NULL){
        printf("could not load tiles\n");
        return false;
    } 
    printf("Tiles initialized\n");

    *gm->towers = initializeTowers(gm->renderer);
    if(gm->towers == NULL){
        printf("could not load towers\n");
        return false;
    } 
    printf("Towers initialized\n");

    *gm->enemies = initializeEnemies(gm->renderer);
    if(gm->enemies == NULL){
        printf("could not load enemies\n");
        return false;
    } 

    *gm->hud = initializeHUD(gm->renderer);
    if(gm->hud == NULL){
        printf("could not load HUD\n");
        return false;
    } 
    printf("HUD initialized\n");

    return true;
}

void cleanup(GameModel *gm){
    printf("cleaning up...\n");
    cleanupTiles(gm->tileManager);
    cleanupTowers(gm->towers);
    cleanupEnemies(gm->enemies);
    cleanupHUD(gm->hud);
    SDL_DestroyRenderer(gm->renderer);
    IMG_Quit();
    SDL_DestroyWindow(gm->win);
    SDL_Quit();
    printf("Done!\n");
}
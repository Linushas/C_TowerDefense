#include <stdio.h>
#include <math.h>
#include "game.h"

int gameLoop(GameModel *gm) { 
    // use malloc?
    SDL_Renderer *renderer; gm->renderer = renderer;
    SDL_Event event; gm->event = &event;
    TM tileManager; gm->tileManager = &tileManager;
    TOWERS towers; gm->towers = &towers;
    EM enemies; gm->enemies = &enemies;
    HUD hud; gm->hud = &hud;
    
    int running = loadGame(gm);
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
                        updateHUD(gm);
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
    updateEnemies(gm);

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

    if(gm->hearts <= 0) {
        printf("GAME OVER! YOU LOST\n");
        cleanup(gm);
    } 
}

void render(GameModel *gm) {
    SDL_SetRenderDrawColor(gm->renderer, 0, 0, 0, 0);
SDL_RenderClear(gm->renderer);

    drawTiles(gm->tileManager);
    drawProjectiles(gm->towers);
    drawTowers(gm->towers);
    drawEnemies(gm->enemies);
    drawHUD(gm);

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

    gm->hud->money = 440;
    gm->hud->debug = false;
    gm->hearts = 3;

    return true;
}

void cleanup(GameModel *gm){
    printf("cleaning up...\n");

    cleanupTiles(gm->tileManager);
    printf("tiles destroyed\n");

    cleanupTowers(gm->towers);
    printf("towers destroyed\n");

    cleanupEnemies(gm->enemies);
    printf("enemies destroyed\n");

    cleanupHUD(gm->hud);
    printf("hud destroyed\n");

    SDL_DestroyRenderer(gm->renderer);
    printf("renderer destroyed\n");

    IMG_Quit();
    SDL_DestroyWindow(gm->win);
    printf("window destroyed\n");
    SDL_Quit();
    printf("Done!\n");
}
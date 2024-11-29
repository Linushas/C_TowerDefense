#include <stdio.h>
#include <math.h>
#include "game.h"

int gameLoop(SDL_Window* win) {
    SDL_Renderer* renderer;
    TM tileManager;
    TOWERS towers;
    HUD hud;
    SDL_Event* event;
    int running = loadGame(win, renderer, &tileManager, &towers, &hud);
    hud.money = 1000;

    // MAIN GAME LOOP
    while(running) {
        while(SDL_PollEvent(event)) {

            switch(event->type) {
                case SDL_QUIT: 
                    running = false; 
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(hud.state == NEW_TOWER_STATE) {
                        updateHUD(&hud, &towers, &tileManager);
                    }
                    else {
                        if(isTower(&towers, tileManager.selectedCol, tileManager.selectedRow)){
                            hud.state = UPGRADE_STATE;
                        }
                        else {
                            if(tileManager.tiles[tileManager.selectedTileID].isEnemyPath == false)
                                hud.state = NEW_TOWER_STATE;
                            break;
                        }
                        
                    }    
            }

        }

        update(&tileManager, &towers, &hud);
        render(&tileManager, &towers, &hud);

        SDL_Delay(1000/FPS);
    }

    cleanup(win, renderer, &tileManager, &towers, &hud);
    return true;
}

void update(TM *tileManager, TOWERS *towers, HUD *hud) {
    int mouseX, mouseY;
    Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);
    if(hud->state == NEW_TOWER_STATE) {
        hud->mouseX = mouseX;
        hud->mouseY = mouseY;
    }
    else {
        tileManager->selectedCol = mouseX / TILESIZE;
        tileManager->selectedRow = mouseY / TILESIZE;
        tileManager->selectedTileID = tileManager->tileMap[tileManager->selectedRow][tileManager->selectedCol];
    }

    for(int i = 0; i < towers->activeTowers; i++){
        float dx = mouseX - towers->inGame[i].x * TILESIZE;
        float dy = mouseY - towers->inGame[i].y * TILESIZE;
        float angle = atan2(dy, dx) * 180 / M_PI;
        if (angle < 0) angle += 360; 
        towers->inGame[i].angle = angle + 90.0;
    }
    
}

void render(TM *tileManager, TOWERS *towers, HUD *hud) {
    SDL_RenderClear(tileManager->renderer);

    drawTiles(tileManager);
    drawTowers(towers);
    drawHUD(hud, towers);

    SDL_RenderPresent(tileManager->renderer);
}

int loadGame(SDL_Window* win, SDL_Renderer* renderer, TM *tileManager, TOWERS *towers, HUD *hud) {
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (!renderer) {
        printf("Could not create renderer: %s\n", SDL_GetError());
        return false;
    }

    *tileManager = initializeTiles(renderer);
    if(tileManager == NULL){
        printf("could not load tiles\n");
        return false;
    } 

    *towers = initializeTowers(renderer);
    if(towers == NULL){
        printf("could not load towers\n");
        return false;
    } 

    *hud = initializeHUD(renderer);
    if(hud == NULL){
        printf("could not load HUD\n");
        return false;
    } 

    return true;
}

void cleanup(SDL_Window* win, SDL_Renderer* renderer, TM *tileManager, TOWERS *towers, HUD *hud){
    cleanupTiles(tileManager);
    cleanupTowers(towers);
    cleanupHUD(hud);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    SDL_DestroyWindow(win);
    SDL_Quit();
}
#include <stdio.h>
#include "game.h"

int gameLoop(SDL_Window* win) {
    SDL_Renderer* renderer;
    TM tileManager;
    TOWERS towers;
    HUD hud;
    SDL_Event* event;
    int running = loadGame(win, renderer, &tileManager, &towers, &hud);

    // MAIN GAME LOOP
    while(running) {
        while(SDL_PollEvent(event)) {
            switch(event->type) {
                case SDL_QUIT: 
                    running = false; 
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(tileManager.tiles[tileManager.selectedTileID].isEnemyPath == false)
                        newTower(&towers, tileManager.selectedCol, tileManager.selectedRow);
                    break;
            }
        }

        update(&towers);
        render(&tileManager, &towers);

        SDL_Delay(1000/FPS);
    }

    cleanup(win, renderer, &tileManager, &towers, &hud);
    return true;
}

void update(TOWERS *towers) {

}

void render(TM *tileManager, TOWERS *towers) {
    SDL_SetRenderDrawColor(tileManager->renderer, 0, 0, 0, 255);
    SDL_RenderClear(tileManager->renderer);

    tileManager->draw(tileManager);
    towers->draw(towers);

    SDL_RenderPresent(tileManager->renderer);
}

int loadGame(SDL_Window* win, SDL_Renderer* renderer, TM *tileManager, TOWERS *towers, HUD *hud) {
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
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
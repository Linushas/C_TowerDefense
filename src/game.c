#include <stdio.h>
#include "game.h"

int gameLoop(SDL_Window* win) {
    SDL_Renderer* renderer;
    TM tileManager;
    HUD hud;
    SDL_Event event;
    int running = loadGame(win, renderer, &tileManager, &hud);

    // MAIN GAME LOOP
    while(running) {
        if(eventHandler(&event) == QUIT_WINDOW) running = false;

        render(&tileManager);

        SDL_Delay(1000/FPS);
    }

    cleanup(win, renderer, &tileManager);
    return true;
}

int eventHandler(SDL_Event *event) {
    while(SDL_PollEvent(event)) {
        if(event->type == SDL_QUIT) {
            return QUIT_WINDOW;
        }
    }
    return NOTHING;
}

void update() {

}

void render(TM *tileManager) {
    SDL_SetRenderDrawColor(tileManager->renderer, 0, 0, 0, 255);
    SDL_RenderClear(tileManager->renderer);

    tileManager->draw(tileManager);

    SDL_RenderPresent(tileManager->renderer);
}

int loadGame(SDL_Window* win, SDL_Renderer* renderer, TM *tileManager, HUD *hud) {
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

    *hud = initializeHUD(renderer);
    if(hud == NULL){
        printf("could not load HUD\n");
        return false;
    } 

    return true;
}

void cleanup(SDL_Window* win, SDL_Renderer* renderer, TM *tileManager){
    cleanupTiles(tileManager);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    SDL_DestroyWindow(win);
    SDL_Quit();
}
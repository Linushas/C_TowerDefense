#include <stdio.h>
#include "game.h"

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("Linus Tower Defense Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    if(!gameLoop(win))
        printf("game loop error : returned false\n");

    return 0;
}
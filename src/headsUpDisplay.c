#include <stdio.h>
#include "headsUpDisplay.h"
#include "game.h"

HUD initializeHUD(SDL_Renderer* renderer){
    HUD hud;
    hud.renderer = renderer;

    return hud;
}

void updateHUD(HUD *hud, TOWERS *towers, TM *tileManager, int *money){
    if(hud->state == NEW_TOWER_STATE){
        if(hud->mouseX > SCREEN_WIDTH-180 && hud->mouseX < SCREEN_WIDTH-180+TILESIZE &&
            hud->mouseY > 50 && hud->mouseY < 50+TILESIZE && *money >= 200) {
                newTower(towers, tileManager->selectedCol, tileManager->selectedRow);
                hud->state = NO_HUD;
                *money -= 200;
                printf("Money: %d\n", *money);
        }
        if(hud->mouseX < SCREEN_WIDTH-180) {
                hud->state = NO_HUD;
        }
    }
    
}

void drawHUD(HUD *hud, TOWERS *towers){
    SDL_Texture *texture;

    if(hud->state == NEW_TOWER_STATE) {
        SDL_Rect rect = {SCREEN_WIDTH-200, 40, 200, SCREEN_HEIGHT-80};
        SDL_SetRenderDrawColor(hud->renderer, 0, 0, 0, 200);
        SDL_RenderFillRect(hud->renderer, &rect);
        
        texture = towers->types[0].texture;
        SDL_Rect texture_rect = {SCREEN_WIDTH-180, 50, TILESIZE, TILESIZE};
        SDL_RenderCopy(hud->renderer, texture, NULL, &texture_rect); 
    }
}

void cleanupHUD(HUD *hud){

}
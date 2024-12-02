#include <stdio.h>
#include "headsUpDisplay.h"
#include "game.h"

HUD initializeHUD(SDL_Renderer* renderer){
    HUD hud;
    hud.renderer = renderer;

    return hud;
}

void updateHUD(HUD *hud, TOWERS *towers, TM *tileManager){
    if(hud->state == NEW_TOWER_STATE){
        if(hud->mouseX > SCREEN_WIDTH-180 && hud->mouseX < SCREEN_WIDTH-180+TILESIZE &&
            hud->mouseY > 50 && hud->mouseY < 50+TILESIZE && hud->money >= 200) {
                newTower(towers, tileManager->selectedCol, tileManager->selectedRow);
                hud->state = NO_HUD;
                hud->money -= 200;
        }
        if(hud->mouseX < SCREEN_WIDTH-180) {
            hud->state = NO_HUD;
        }
    }
    else if(hud->state == UPGRADE_STATE){
        if(hud->mouseX > SCREEN_WIDTH-180 && hud->mouseX < (SCREEN_WIDTH-180)+(TILESIZE*2) &&
            hud->mouseY > 100 && hud->mouseY < 100+40) {
                upgradeTower(&towers->inGame[towers->selectedTowerIndex]);
                hud->state = NO_HUD;
        }
        SDL_Rect button = {SCREEN_WIDTH-180, 100, TILESIZE*2, 40};
        if(hud->mouseX < SCREEN_WIDTH-180) {
            hud->state = NO_HUD;
        }
    }
    
}

void drawHUD(HUD *hud, TOWERS *towers){
    SDL_Texture *texture;
    TTF_Font *sans = TTF_OpenFont("res/fonts/Roboto/Roboto-Black.ttf", 20);
    SDL_Color fontColor1 = {255, 255, 255, 255};
    SDL_Color fontColor2 = {255, 0, 0, 255};

    if(hud->state == NEW_TOWER_STATE) {
        SDL_Rect rect = {SCREEN_WIDTH-200, 40, 200, SCREEN_HEIGHT-80};
        SDL_SetRenderDrawColor(hud->renderer, 0, 0, 0, 200);
        SDL_RenderFillRect(hud->renderer, &rect);
        
        texture = towers->types[0].texture[0];
        SDL_Rect texture_rect = {SCREEN_WIDTH-180, 50, TILESIZE, TILESIZE};
        SDL_RenderCopy(hud->renderer, texture, NULL, &texture_rect); 

        if(hud->money < 200)
            renderText(hud, "$200", SCREEN_WIDTH-180, 70, sans, fontColor2);
        else renderText(hud, "$200", SCREEN_WIDTH-180, 70, sans, fontColor1);
    }
    else if(hud->state == UPGRADE_STATE) {
        SDL_Rect rect = {SCREEN_WIDTH-200, 40, 200, SCREEN_HEIGHT-80};
        SDL_SetRenderDrawColor(hud->renderer, 0, 0, 0, 200);
        SDL_RenderFillRect(hud->renderer, &rect);

        char lvl[60];
        sprintf(lvl, "Level %d", towers->inGame[towers->selectedTowerIndex].level);
        renderText(hud, lvl, SCREEN_WIDTH-180, 70, sans, fontColor1);

        SDL_Rect button = {SCREEN_WIDTH-180, 100, TILESIZE*2, 40};
        SDL_SetRenderDrawColor(hud->renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(hud->renderer, &button);
    }

    char string[60];
    sprintf(string, "$%d", hud->money);
    renderText(hud, string, 20, 20, sans, fontColor1);
}

void renderText(HUD* hud, char *str, int x, int y, TTF_Font *font, SDL_Color fontColor) {
    SDL_Surface *fontSurf = TTF_RenderText_Blended(font, str, fontColor);
    SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(hud->renderer, fontSurf);

    SDL_Rect font_rect = {x, y, fontSurf->w, fontSurf->h};
    SDL_FreeSurface(fontSurf);
    SDL_RenderCopy(hud->renderer, fontTexture, NULL, &font_rect);
}

void cleanupHUD(HUD *hud){

}
#include <stdio.h>
#include "headsUpDisplay.h"
#include "game.h"

HUD initializeHUD(SDL_Renderer* renderer){
    HUD hud;
    hud.renderer = renderer;
    hud.font = TTF_OpenFont("res/fonts/Roboto/Roboto-Black.ttf", 20);

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
            hud->mouseY > 100 && hud->mouseY < 100+40 && hud->money >= 60) {
                if(towers->inGame[towers->selectedTowerIndex].level < 5) {
                    upgradeTower(&towers->inGame[towers->selectedTowerIndex]);
                    hud->state = NO_HUD;
                    hud->money -= 60;
                }
        }
        SDL_Rect button = {SCREEN_WIDTH-180, 100, TILESIZE*2, 40};
        if(hud->mouseX < SCREEN_WIDTH-180) {
            hud->state = NO_HUD;
        }
    }
    
}

void drawHUD(HUD *hud, TOWERS *towers, EM *enemies){
    SDL_Texture *texture;
    SDL_Color fontColor1 = {255, 255, 255, 255};
    SDL_Color fontColor2 = {255, 0, 0, 255};
    SDL_Color fontColor3 = {0, 0, 0, 255};

    if(hud->state == NEW_TOWER_STATE) {
        SDL_Rect rect = {SCREEN_WIDTH-200, 40, 200, SCREEN_HEIGHT-80};
        SDL_SetRenderDrawColor(hud->renderer, 0, 0, 0, 200);
        SDL_RenderFillRect(hud->renderer, &rect);
        
        texture = towers->types[0].texture[0];
        SDL_Rect texture_rect = {SCREEN_WIDTH-180, 50, TILESIZE, TILESIZE};
        SDL_RenderCopy(hud->renderer, texture, NULL, &texture_rect); 

        if(hud->money < 200)
            renderText(hud, "$200", SCREEN_WIDTH-180, 70, fontColor2);
        else renderText(hud, "$200", SCREEN_WIDTH-180, 70, fontColor1);
    }
    else if(hud->state == UPGRADE_STATE) {
        SDL_Rect rect = {SCREEN_WIDTH-200, 40, 200, SCREEN_HEIGHT-80};
        SDL_SetRenderDrawColor(hud->renderer, 0, 0, 0, 200);
        SDL_RenderFillRect(hud->renderer, &rect);

        char lvl[60];
        if(towers->inGame[towers->selectedTowerIndex].level == 5) {
            renderText(hud, "Level: MAX", SCREEN_WIDTH-180, 70, fontColor1);
        }
        else {
            sprintf(lvl, "Level: %d", towers->inGame[towers->selectedTowerIndex].level);
            renderText(hud, lvl, SCREEN_WIDTH-180, 70, fontColor1);
            createButton(hud, "Upgrade $60", SCREEN_WIDTH-180, 100, TILESIZE*2, 40, fontColor3, fontColor1);
        }
    }

    char string[60];
    sprintf(string, "$%d", hud->money);
    renderText(hud, string, 20, 20, fontColor1);

    sprintf(string, "Wave: %d", enemies->currentWave);
    renderText(hud, string, 20, 50, fontColor1);
}

void renderText(HUD* hud, char *str, int x, int y, SDL_Color textColor) {
    SDL_Surface *fontSurf = TTF_RenderText_Blended(hud->font, str, textColor);
    SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(hud->renderer, fontSurf);

    SDL_Rect font_rect = {x, y, fontSurf->w, fontSurf->h};
    SDL_FreeSurface(fontSurf);
    SDL_RenderCopy(hud->renderer, fontTexture, NULL, &font_rect);
}


Button createButton(HUD* hud, char *text, int x, int y, int width, int height, SDL_Color bgColor, SDL_Color textColor) {
    Button b;
    b.rect.x = x;
    b.rect.y = y;
    b.rect.w = width;
    b.rect.h = height;
    b.bgColor = bgColor;
    b.textColor = textColor;
    b.text = text;
    
    SDL_SetRenderDrawColor(hud->renderer, b.bgColor.r, b.bgColor.g, b.bgColor.b, b.bgColor.a);
    SDL_RenderFillRect(hud->renderer, &b.rect);
    renderText(hud, b.text, b.rect.x + 10, b.rect.y + 5, b.textColor);

    return b;
}

void cleanupHUD(HUD *hud){

}
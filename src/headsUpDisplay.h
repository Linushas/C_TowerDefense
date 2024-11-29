#ifndef HEADSUPDISPLAY_H
#define HEADSUPDISPLAY_H

#include "tiles.h"
#include "tower.h"
#include <SDL2/SDL_ttf.h>

enum{NO_HUD, NEW_TOWER_STATE, UPGRADE_STATE};

typedef struct hud {
    int state;
    int mouseX, mouseY;
    int money;
    SDL_Renderer* renderer;
} HUD;

HUD initializeHUD(SDL_Renderer* renderer);
void cleanupHUD(HUD *hud);
void updateHUD(HUD *hud, TOWERS *towers, TM *tileManager);
void drawHUD(HUD *hud, TOWERS *towers);
void renderText(HUD* hud, char *str, int x, int y, TTF_Font *font, SDL_Color fontColor);

#endif
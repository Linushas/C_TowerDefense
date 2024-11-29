#ifndef HEADSUPDISPLAY_H
#define HEADSUPDISPLAY_H

#include "tiles.h"
#include "tower.h"

enum{NO_HUD, NEW_TOWER_STATE, UPGRADE_STATE};

typedef struct hud {
    int state;
    int mouseX, mouseY;
    SDL_Renderer* renderer;
} HUD;

HUD initializeHUD(SDL_Renderer* renderer);
void cleanupHUD(HUD *hud);
void updateHUD(HUD *hud, TOWERS *towers, TM *tileManager, int *money);
void drawHUD(HUD *hud, TOWERS *towers);

#endif
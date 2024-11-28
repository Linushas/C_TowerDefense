#ifndef HEADSUPDISPLAY_H
#define HEADSUPDISPLAY_H

#include "tiles.h"

typedef struct hud {
    SDL_Renderer* renderer;
    void (*draw)(struct hud*);
    void (*update)(struct hud*);
} HUD;

HUD initializeHUD(SDL_Renderer* renderer);
void cleanupHUD(HUD *hud);
void updateHUD(HUD *hud);
void drawHUD(HUD *hud);

#endif
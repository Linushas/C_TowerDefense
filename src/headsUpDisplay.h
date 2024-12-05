#ifndef HEADSUPDISPLAY_H
#define HEADSUPDISPLAY_H

#include "tiles.h"
#include "tower.h"
#include <SDL2/SDL_ttf.h>

enum{NO_HUD, NEW_TOWER_STATE, UPGRADE_STATE};

typedef struct hud {
    int state;
    unsigned int renderTime;
    bool debug;
    int mouseX, mouseY;
    int money;
    SDL_Renderer* renderer;
    TTF_Font *font;
} HUD;

typedef struct button {
    SDL_Rect rect;
    char *text;
    SDL_Color bgColor;
    SDL_Color textColor;
} Button;

typedef struct gameModel GameModel;

HUD initializeHUD(SDL_Renderer* renderer);
void cleanupHUD(HUD *hud);
void updateHUD(GameModel *gm);
void drawHUD(GameModel *gm);

void renderText(HUD* hud, char *str, int x, int y, SDL_Color textColor);
Button createButton(HUD* hud, char *text, int x, int y, int width, int height, SDL_Color bgColor, SDL_Color textColor);

#endif
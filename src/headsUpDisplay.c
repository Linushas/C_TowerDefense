#include "headsUpDisplay.h"

HUD initializeHUD(SDL_Renderer* renderer){
    HUD hud;
    hud.renderer = renderer;
    hud.draw = drawHUD;
    hud.update = updateHUD;

    return hud;
}

void updateHUD(HUD *hud){

}

void drawHUD(HUD *hud){

}

void cleanupHUD(HUD *hud){

}
#ifndef GAME_H
#define GAME_H

#include "tiles.h"
#include "headsUpDisplay.h"

#define SCREEN_WIDTH TILESIZE*MAP_COLS
#define SCREEN_HEIGHT TILESIZE*MAP_ROWS
#define FPS 60

#define QUIT_WINDOW -1
enum events{NOTHING};

int gameLoop(SDL_Window* win);
int eventHandler(SDL_Event *event);
void update();
void render(TM *tileManager);

int loadGame(SDL_Window* win, SDL_Renderer* renderer, TM *tileManager, HUD *hud);
void cleanup(SDL_Window* win, SDL_Renderer* renderer, TM *tileManager);

#endif
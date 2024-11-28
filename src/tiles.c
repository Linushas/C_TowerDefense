#include "tiles.h"
#include <stdio.h>

TM initializeTiles(SDL_Renderer* renderer) {
    TM tileManager;
    tileManager.renderer = renderer;
    tileManager.draw = drawTiles;

    loadTiles(&tileManager);
    loadMap("res/maps/map01.txt", tileManager.tileMap);

    return tileManager;
}

int loadTiles(TM *tileManager) {
    char *path1 = "res/images/grass.png";
    char *path2 = "res/images/sand.png";

    tileManager->tiles[0].texture = IMG_LoadTexture(tileManager->renderer, path1);
    if (!tileManager->tiles[0].texture) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return false;
    }
    printf("%s loaded\n", path1);
    tileManager->tiles[0].isEnemyPath = false;

    tileManager->tiles[1].texture = IMG_LoadTexture(tileManager->renderer, path2);
    if (!tileManager->tiles[1].texture) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return false;
    }
    printf("%s loaded\n", path2);
    tileManager->tiles[0].isEnemyPath = true;
}

int loadMap(char *path, int map[MAP_ROWS][MAP_COLS]) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        printf("Error: Could not open file %s\n", path);
        return 0;
    }

    char ch;
    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS; j++) {
            ch = fgetc(f);
            if (ch == EOF || ch == '\n') {
                printf("Error: Unexpected end of file or invalid format\n");
                fclose(f);
                return 0;
            }
            map[i][j] = ch - '0';
        }

        if (fgetc(f) != '\n' && !feof(f)) {
            printf("Error: Line too long in file\n");
            fclose(f);
            return 0;
        }
    }

    fclose(f);
    printf("Succesfully loaded map: %s\n", path);
    return 1;
}

void drawTiles(TM *tileManager) {
    SDL_Texture *texture;
    for(int y = 0; y < MAP_ROWS; y++){
        for(int x = 0; x < MAP_COLS; x++){
            texture = tileManager->tiles[tileManager->tileMap[y][x]].texture;
            SDL_Rect texture_rect = {x*TILESIZE, y*TILESIZE, TILESIZE, TILESIZE};
            SDL_RenderCopy(tileManager->renderer, texture, NULL, &texture_rect); 
        }
    }
}

void cleanupTiles(TM *tileManager) {
    SDL_DestroyTexture(tileManager->tiles[0].texture);
}
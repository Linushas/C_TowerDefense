#include "tiles.h"
#include <stdio.h>

TM initializeTiles(SDL_Renderer* renderer) {
    TM tileManager;
    tileManager.renderer = renderer;

    if (!loadTiles(&tileManager)) {
        printf("Error loading tiles.\n");
    }

    if (loadMap("res/maps/map1.csv", tileManager.tileMap)) {
        for (int i = 0; i < MAP_ROWS; i++) {
            for (int j = 0; j < MAP_COLS; j++) {
                if(tileManager.tileMap[i][j] < 10)
                    printf("%d  ", tileManager.tileMap[i][j]);
                else printf("%d ", tileManager.tileMap[i][j]);
            }
            printf("\n");
        }
    }

    return tileManager;
}

int loadTiles(TM *tileManager) {
    char *path = "res/images/tileSheet.png";
    SDL_Texture *sheet = IMG_LoadTexture(tileManager->renderer, path);
    if (!sheet) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return false;
    }
    printf("%s loaded\n", path);

    int rows = 5, cols = 10;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int tileIndex = i * cols + j;
            if (tileIndex >= 50) {
                break;
            }

            SDL_Rect cropRect = {j * 32, i * 32, 32, 32};
            tileManager->tiles[tileIndex].texture = SDL_CreateTexture(
                tileManager->renderer,
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET,
                TILESIZE,
                TILESIZE
            );
            if (!tileManager->tiles[tileIndex].texture) {
                printf("Failed to create tile texture: %s\n", SDL_GetError());
                SDL_DestroyTexture(sheet);
                return false;
            }
            
            SDL_SetRenderTarget(tileManager->renderer, tileManager->tiles[tileIndex].texture);
            SDL_SetTextureBlendMode(tileManager->tiles[tileIndex].texture, SDL_BLENDMODE_BLEND);

            if (SDL_RenderCopy(tileManager->renderer, sheet, &cropRect, NULL) < 0) {
                printf("Failed to render copy tile: %s\n", SDL_GetError());
                SDL_DestroyTexture(sheet);
                return false;
            }

            tileManager->tiles[tileIndex].isEnemyPath = false;

            SDL_SetRenderTarget(tileManager->renderer, NULL);
        }
    }

    SDL_DestroyTexture(sheet);

    tileManager->tiles[10].isEnemyPath = true;
    tileManager->tiles[11].isEnemyPath = true;
    tileManager->tiles[12].isEnemyPath = true;
    tileManager->tiles[13].isEnemyPath = true;
    tileManager->tiles[15].isEnemyPath = true;

    return true;
}

int loadMap(char *path, int map[MAP_ROWS][MAP_COLS]) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        printf("Error: Could not open file %s\n", path);
        return 0;
    }

    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS; j++) {
            int value;
            if (fscanf(f, "%d", &value) != 1) {
                printf("Error: Invalid format or unexpected end of file\n");
                fclose(f);
                return 0;
            }
            map[i][j] = value;

            if (j < MAP_COLS - 1) {
                char ch = fgetc(f);
                if (ch != ',') {
                    printf("Error: Missing comma or unexpected character\n");
                    fclose(f);
                    return 0;
                }
            }
        }

        char ch = fgetc(f);
        if (ch != '\n' && ch != EOF) {
            printf("Error: Line too long or invalid format\n");
            fclose(f);
            return 0;
        }
    }

    fclose(f);
    printf("Successfully loaded map: %s\n", path);
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

    // draws selected tile if not tile is enemy path
    if(tileManager->tiles[tileManager->selectedTileID].isEnemyPath == false) {
        texture = tileManager->tiles[49].texture;
        SDL_Rect texture_rect = {tileManager->selectedCol*TILESIZE, tileManager->selectedRow*TILESIZE, TILESIZE, TILESIZE};
        SDL_RenderCopy(tileManager->renderer, texture, NULL, &texture_rect); 
    }
}

void cleanupTiles(TM *tileManager) {
    SDL_DestroyTexture(tileManager->tiles[0].texture);
}
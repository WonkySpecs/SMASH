#include "tiles.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// these make me a bit ill
#define NUM_LAVA_TILES 4
#define NUM_GROUND_TILES 8

const char *tileFilenames[NUM_TILE_TYPES];
Vector2 tileOffsets[NUM_TILE_TYPES];
Vector2 tilePercentSize;

TileType lavaTiles[NUM_LAVA_TILES] = {LAVA_0, LAVA_1, LAVA_2, LAVA_3};
TileType groundTiles[NUM_GROUND_TILES] =
    {TOMB_0_NEW, TOMB_1_NEW, TOMB_2_NEW, TOMB_3_NEW,
     TOMB_0_OLD, TOMB_1_OLD, TOMB_2_OLD, TOMB_3_OLD, };

void loadTileFilenames() {
    char *bases[4] = { "lava_%d", "catacombs_%d", "tomb_%d_new", "tomb_%d_old" };
    int num[4] = {4, 10, 4, 4};
    int c = 0;
    char fname[64];
    char path[256];
    for (int b = 0; b < 4; b++) {
        for (int i = 0; i < num[b]; i++) {
            snprintf(fname, sizeof(fname), bases[b], i);
            snprintf(path, sizeof(path), "assets/%s.png", fname);
            tileFilenames[c] = (char *)malloc(sizeof(path));
            strcpy(tileFilenames[c], &path);
            c++;
        }
    }
}

Texture2D loadTileSheet() {
    int w = ceil(sqrt(NUM_TILE_TYPES));
    Image test = LoadImage(tileFilenames[0]);
    Image im = GenImageColor(w * test.width, w * test.height, PINK);
    UnloadImage(test);
    tilePercentSize = (Vector2){1.0 / (float)w, 1.0 / (float)w};

    for(int i = 0; i < NUM_TILE_TYPES; i++) {
        int x = i % w;
        int y = i / w;
        Image tile = LoadImage(tileFilenames[i]);

        ImageDraw(&im, tile,
                  (Rectangle){0, 0, tile.width, tile.height},
                  (Rectangle){x * tile.width, y * tile.height,
                              tile.width, tile.height},
                  WHITE);
        tileOffsets[i] = (Vector2){(float)x * tilePercentSize.x,
                                   (float)y * tilePercentSize.y};
    }
    Texture2D ret = LoadTextureFromImage(im);
    UnloadImage(im);
    return ret;
}

Texture2D loadTiles() {
    loadTileFilenames();
    return loadTileSheet();
}

TileType randomTile(TileCategory cat) {
    int num = cat == LAVA ? NUM_LAVA_TILES :
                            NUM_GROUND_TILES;
    TileType *types = cat == LAVA ? lavaTiles : groundTiles;
    return types[GetRandomValue(0, num - 1)];
}


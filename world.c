#include "world.h"
#include <stdlib.h>
#include <stdio.h>

bool isLava(int x, int y) {
    int c = x * x + y * y;
    return c > 250 && c < 500;
}

Map initMap() {
    Map map;
    Texture2D lavaTex = LoadTexture("assets/lava_0.png");
    Texture2D stoneTextures[8];
    for (int i = 0; i < 8; i++) {
        char *texName = (char *)malloc(50 * sizeof(char));
        sprintf(texName,
                "assets/tomb_%d_%s.png",
                i % 4,
                i > 4 ? "old" : "new");
        stoneTextures[i] = LoadTexture(texName);
        free(texName);
    }

    for(int x = 0; x < MAP_WIDTH; x++) {
        for(int y = 0; y < MAP_HEIGHT; y++) {
            Texture tex = isLava(x, y) ? lavaTex : stoneTextures[GetRandomValue(0, 7)];
            map.tiles[x][y] = (Tile){tex};
        }
    }
    return map;
}

void drawMap(Map map, Camera2D camera) {
    for(int x = 0; x < MAP_WIDTH; x++) {
        for(int y = 0; y < MAP_HEIGHT; y++) {
            Texture tex = map.tiles[x][y].texture;
            Vector2 screenPos = GetWorldToScreen2D(
                (Vector2){x * tex.width, y * tex.height},
                camera);
            DrawTextureV(tex, screenPos,  WHITE);
        }
    }
}

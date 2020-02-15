#include "world.h"

bool isLava(int x, int y) {
    int c = x * x + y * y;
    return c > 250 && c < 500;
}

Map initMap() {
    Map map;
    for(int x = 0; x < MAP_WIDTH; x++) {
        for(int y = 0; y < MAP_HEIGHT; y++) {
            if (isLava(x, y)) {
                map.tiles[x][y] = (Tile){LoadTexture("assets/lava_0.png")};
            } else {
                char *texName = (char *)malloc(50 * sizeof(char));
                sprintf(texName,
                        "assets/tomb_%d_%s.png",
                        x % 4,
                        y % 2 == 0 ? "old" : "new");
                map.tiles[x][y] = (Tile){LoadTexture(texName)};
                free(texName);
            }
        }
    }
    return map;
}

void drawMap(Map map) {
    for(int x = 0; x < MAP_WIDTH; x++) {
        for(int y = 0; y < MAP_HEIGHT; y++) {
            Texture tex = map.tiles[x][y].texture;
            DrawTexture(tex, x * tex.width, y * tex.height, WHITE);
        }
    }
}

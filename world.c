#include "world.h"
#include <stdlib.h>
#include "tiles.h"

bool isLava(int x, int y) {
    int c = x * x + y * y;
    return c > 250 && c < 500;
}

Map initMap() {
    Map map = {loadTiles()};

    for(int x = 0; x < MAP_WIDTH; x++) {
        for(int y = 0; y < MAP_HEIGHT; y++) {
            TileCategory cat = isLava(x, y) ? LAVA : GROUND;
            map.tiles[x][y] = tileOffsets[randomTile(cat)];
        }
    }
    const int numObstacles = 4;
    const int w = MAP_WIDTH * TILE_SIZE;
    const int h = MAP_HEIGHT * TILE_SIZE;
    map.obstacles = (Rectangle *)malloc(numObstacles * sizeof(Rectangle));
    map.obstacles[0] = (Rectangle) {0, 0, w, 10};
    map.obstacles[1] = (Rectangle) {0, 0, 10, h};
    map.obstacles[2] = (Rectangle) {0, h, MAP_WIDTH * TILE_SIZE + 9, 10};
    map.obstacles[3] = (Rectangle) {w, 0, 10, h};
    map.numObstacles = numObstacles;
    return map;
}

void drawMap(Map map, Camera2D camera) {
    for(int x = 0; x < MAP_WIDTH; x++) {
        for(int y = 0; y < MAP_HEIGHT; y++) {
            Vector2 screenPos = GetWorldToScreen2D(
                (Vector2){x * TILE_SIZE, y * TILE_SIZE},
                camera);
            Rectangle dest = (Rectangle) {
                screenPos.x, screenPos.y,
                map.tileTex.width * tilePercentSize.x,
                map.tileTex.height * tilePercentSize.y
            };
            DrawTextureQuad(map.tileTex,
                            tilePercentSize,
                            map.tiles[x][y],
                            dest,
                            WHITE);
        }
    }
}

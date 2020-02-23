#ifndef __TILES
#define __TILES

#include "raylib.h"

typedef enum TileType {
    LAVA_0, LAVA_1, LAVA_2, LAVA_3,
    CATACOMBS_0, CATACOMBS_1, CATACOMBS_2, CATACOMBS_3, CATACOMBS_4,
    CATACOMBS_5, CATACOMBS_6, CATACOMBS_7, CATACOMBS_8, CATACOMBS_9,
    TOMB_0_NEW, TOMB_1_NEW, TOMB_2_NEW, TOMB_3_NEW,
    TOMB_0_OLD, TOMB_1_OLD, TOMB_2_OLD, TOMB_3_OLD,
    NUM_TILE_TYPES,
} TileType;

typedef enum TileCategory {
    LAVA, GROUND, WALL
} TileCategory;

Vector2 tilePercentSize;
Vector2 tileOffsets[NUM_TILE_TYPES];

TileType randomTile(TileCategory cat);
Texture2D loadTiles(void);
#endif

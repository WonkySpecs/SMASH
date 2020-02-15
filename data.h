#ifndef __DATA
#define __DATA
#include "raylib.h"
#include <math.h>

#define BASEFIELDS \
    Texture2D texture; \
    Vector2 pos; \
    Vector2 vel; \
    float rot;

#define MAP_WIDTH 50
#define MAP_HEIGHT 40

typedef struct Entity {
    BASEFIELDS;
} Entity;

typedef struct Hand {
    BASEFIELDS;
    float speed;
    bool flying;
    Vector2 targetPos;
} Hand;

typedef struct Demon {
    BASEFIELDS;
    Vector2 targetPos;
    Hand rHand;
    Hand lHand;
    float height;
    float zVel;
} Demon;

typedef struct Tile {
    Texture2D texture;
} Tile;

typedef struct Map {
    Tile tiles[MAP_WIDTH][MAP_HEIGHT];
} Map;
#endif

#ifndef __WORLD
#define __WORLD

#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define MAP_WIDTH 50
#define MAP_HEIGHT 40

typedef struct Tile {
    Texture2D texture;
} Tile;

typedef struct Map {
    Tile tiles[MAP_WIDTH][MAP_HEIGHT];
} Map;

Map initMap();
void drawMap(Map map);
#endif

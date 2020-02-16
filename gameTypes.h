#ifndef __GAMETYPES
#define __GAMETYPES

#include "raylib.h"
#include "constants.h"

#define BASEFIELDS \
    Texture2D texture; \
    Vector2 pos; \
    Vector2 vel; \
    float rot;

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
    float trauma; // Not convinced this should be here
} Demon;

typedef enum EnemyType {
    ENEMY_IMP,
} EnemyType;


typedef struct Enemy {
    BASEFIELDS;
    EnemyType type;
} Enemy;

typedef struct EnemyShooter {
    BASEFIELDS;
    EnemyType type;
    float fireDelay;
    float sinceFired;
} EnemyShooter;

typedef struct Tile {
    Texture2D texture;
} Tile;

typedef struct Map {
    Tile tiles[MAP_WIDTH][MAP_HEIGHT];
} Map;

typedef struct World {
    Map *map;
    Demon *demon;
    Enemy *enemies[MAX_ENEMIES];
} World;

#endif
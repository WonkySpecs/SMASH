#ifndef __DATA
#define __DATA
#include "raylib.h"

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
#endif

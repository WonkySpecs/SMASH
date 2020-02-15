#include "raylib.h"
#include <math.h>

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
} Demon;

void DrawEntityScaled(Entity *e, float scale) {
    Texture tex = e->texture;
    float w = tex.width;
    float h = tex.height;
    DrawTexturePro(tex,
                   (Rectangle){0, 0, w, h},
                   (Rectangle){e->pos.x, e->pos.y, w * scale, h * scale},
                   (Vector2){(w / 2) * scale, (h / 2) * scale},
                   e->rot,
                   WHITE);
}

void DrawEntity(Entity *e) {
    DrawEntityScaled(e, 1.0);
}

Vector2 Vector2ToLength(Vector2 vec, float len) {
    return Vector2Scale(vec, len / Vector2Length(vec));
}

Vector2 Vector2Rotate(Vector2 vec, float rads) {
    float s = sin(rads);
    float c = cos(rads);
    return (Vector2){vec.x * c - vec.y * s, vec.x * s + vec.y * c};
}

void MoveTowardsPoint(Entity *e, Vector2 target, float maxSpeed) {
    Vector2 toTarget = Vector2Subtract(target, e->pos);
    float speed = fmin(maxSpeed, Vector2Length(toTarget));
    e->pos = Vector2Add(e->pos,
                        Vector2ToLength(toTarget, speed));
}

#ifndef __MATHUTILS
#define __MATHUTILS
#include "raylib.h"
#include "raymath.h"

typedef struct Circle {
    Vector2 c;
    float r;
} Circle;

Vector2 Vector2ToLength(Vector2 vec, float len);
Vector2 VecLenAngle(float len, float rads);
Vector2 Vector2Rotate(Vector2 vec, float rads);
float Vector2Length2(Vector2 vec);
void moveTowardsPoint(Vector2 *vec, Vector2 target, float maxSpeed);
float randFloat();
float randFloatBetween(float minInclusive, float maxInclusive);
Vector2 rectClosestPoint(Rectangle rect, Vector2 point);
bool rectCircleColliding(Rectangle rect, Circle circle);
bool circlesColliding(Circle c1, Circle c2);
Vector2 moveWithoutHitting(Circle mover, Vector2 vel, Rectangle obstacle);
Vector2 rectPos(Rectangle rect);
#endif

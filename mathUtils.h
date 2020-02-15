#ifndef __MATHUTILS
#define __MATHUTILS
#include "raylib.h"
#include "raymath.h"
#include <math.h>

Vector2 Vector2ToLength(Vector2 vec, float len);
Vector2 Vector2Rotate(Vector2 vec, float rads);
void MoveTowardsPoint(Vector2 *vec, Vector2 target, float maxSpeed);
#endif

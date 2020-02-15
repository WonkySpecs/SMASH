#include "mathUtils.h"

Vector2 Vector2ToLength(Vector2 vec, float len) {
    return Vector2Scale(vec, len / Vector2Length(vec));
}

Vector2 Vector2Rotate(Vector2 vec, float rads) {
    float s = sin(rads);
    float c = cos(rads);
    return (Vector2){vec.x * c - vec.y * s, vec.x * s + vec.y * c};
}

void MoveTowardsPoint(Vector2 *vec, Vector2 target, float maxSpeed) {
    Vector2 toTarget = Vector2Subtract(target, *vec);
    float speed = fmin(maxSpeed, Vector2Length(toTarget));
    *vec = Vector2Add(*vec,
                      Vector2ToLength(toTarget, speed));
}

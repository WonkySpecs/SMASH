#include "mathUtils.h"
#include <math.h>
#include <stdlib.h>

Vector2 VecLenAngle(float len, float rads) {
    return Vector2Rotate((Vector2){len, 0}, rads);
}

Vector2 Vector2ToLength(Vector2 vec, float len) {
    return Vector2Scale(vec, len / Vector2Length(vec));
}

Vector2 Vector2Rotate(Vector2 vec, float rads) {
    float s = sin(rads);
    float c = cos(rads);
    return (Vector2){vec.x * c - vec.y * s, vec.x * s + vec.y * c};
}

float Vector2Length2(Vector2 vec) {
    return vec.x * vec.x + vec.y * vec.y;
}

void moveTowardsPoint(Vector2 *vec, Vector2 target, float maxSpeed) {
    Vector2 toTarget = Vector2Subtract(target, *vec);
    float speed = fmin(maxSpeed, Vector2Length(toTarget));
    *vec = Vector2Add(*vec,
                      Vector2ToLength(toTarget, speed));
}

float randFloat() {
    return (double)rand() / (double)RAND_MAX;
}

float randFloatBetween(float minInclusive, float maxInclusive) {
    return randFloat() * (maxInclusive - minInclusive) + minInclusive;
}

Vector2 rectClosestPoint(Rectangle rect, Vector2 point) {
    return (Vector2) {
        fmax(rect.x, fmin(rect.x + rect.width, point.x)),
        fmax(rect.y, fmin(rect.y + rect.height, point.y)),
    };
}

bool rectCircleColliding(Rectangle rect, Circle circle) {
    Vector2 closest = rectClosestPoint(rect, circle.c);
    return Vector2Length(Vector2Subtract(closest, circle.c)) <= circle.r;
}

bool circlesColliding(Circle c1, Circle c2) {
    return Vector2Length(Vector2Subtract(c1.c, c2.c)) < c1.r + c2.r;
}

Vector2 moveWithoutHitting(Circle mover, Vector2 vel, Rectangle obstacle) {
    Vector2 targetPos = Vector2Add(mover.c, vel);
    Vector2 closestPoint = rectClosestPoint(obstacle, targetPos);
    float closestLen = Vector2Length(Vector2Subtract(targetPos, closestPoint));
    if (closestLen >= mover.r) {
        return vel;
    }

    Vector2 correction = Vector2Scale(Vector2Subtract(mover.c, closestPoint),
                                      (mover.r - closestLen) / closestLen);
    return Vector2Add(vel, correction);
}

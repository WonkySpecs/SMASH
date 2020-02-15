#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <math.h>

#include "utils.h"
#include "constants.h"

#define DARKRED (Color){80, 10, 50, 255}

const int W_WIDTH = 1200;
const int W_HEIGHT = 960;
const int TARGET_FPS = 60;

void handleInputs(Demon *demon) {
    float accel = 3.5;
    if (IsKeyDown(KEY_A)) demon->vel.x -= accel;
    if (IsKeyDown(KEY_D)) demon->vel.x += accel;
    if (IsKeyDown(KEY_W)) demon->vel.y -= accel;
    if (IsKeyDown(KEY_S)) demon->vel.y += accel;
    if (IsKeyDown(KEY_SPACE) && demon->height == 0) demon->zVel = 8;
    demon->targetPos = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON) && !demon->lHand.flying) {
        demon->lHand.flying = true;
        demon->lHand.targetPos = GetMousePosition();
        demon->lHand.speed = 3;
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && !demon->rHand.flying) {
        demon->rHand.flying = true;
        demon->rHand.targetPos = GetMousePosition();
        demon->rHand.speed = 3;
    }
}

void moveHand(Hand *hand, Vector2 basePos, Vector2 neutralOffset, float rot) {
    Vector2 toTarget;

    if (!hand->flying) {
        Vector2 handOffset = Vector2Rotate(neutralOffset, DEG2RAD * rot);
        Vector2 targetPos = Vector2Add(basePos, handOffset);
        MoveTowardsPoint((Entity *)hand, targetPos, hand->speed);
        float waving = sin(GetTime() * 4);
        hand->rot = rot - INITIAL_ROT_OFFSET + waving * 2;
    } else {
        hand->speed += hand->speed / 10;
        MoveTowardsPoint((Entity *)hand,
                         hand->targetPos,
                         hand->speed);
        Vector2 toTarget = Vector2Subtract(hand->targetPos,
                                           hand->pos);
        if (Vector2Length(toTarget) < 2) {
            hand->flying = false;
            hand->speed = NEUTRAL_HAND_SPEED;
        }
    }

}

void updateHands(Demon *demon, float delta) {
    float waving = sin(GetTime() * 4);
    Vector2 rHandOffset = (Vector2){-X_OFFSET + waving * 3,
                                    Y_OFFSET + waving * 2};
    Vector2 lHandOffset = (Vector2){X_OFFSET - waving * 3,
                                    Y_OFFSET + waving * 2};
    moveHand(&demon->rHand, demon->pos, rHandOffset, demon->rot);
    moveHand(&demon->lHand, demon->pos, lHandOffset, demon->rot);
}

void update(Demon *demon, float delta) {
    demon->vel = Vector2Scale(demon->vel, 0.75);
    demon->pos.x += demon->vel.x;
    demon->pos.y += demon->vel.y;
    if (Vector2Length(demon->vel) > 5) {
        demon->vel = Vector2ToLength(demon->vel, 5);
    }
    demon->rot = Vector2Angle(demon->targetPos, demon->pos) + 90;
    updateHands(demon, delta);

    demon->height += demon->zVel;
    demon->zVel -= 0.5;
    if (demon-> height <= 0) {
        demon->height = 0;
        demon->zVel = 0;
    }
}

int main() {
    InitWindow(W_WIDTH, W_HEIGHT, "Test");
    SetTargetFPS(TARGET_FPS);
    float EXPECTED_FRAME_TIME = 1 / TARGET_FPS;
    Vector2 demonStartPos = (Vector2){W_WIDTH / 2, W_HEIGHT / 2};
    Image hand = LoadImage("assets/demon_hand.png");
    Hand rHand = {
        LoadTextureFromImage(hand),
        demonStartPos,
        Vector2Zero(),
        0,
        NEUTRAL_HAND_SPEED,
        false,
    };
    ImageFlipHorizontal(&hand);
    Hand lHand = {
        LoadTextureFromImage(hand),
        demonStartPos,
        Vector2Zero(),
        0,
        NEUTRAL_HAND_SPEED,
        false,
    };
    UnloadImage(hand);

    Demon demon = {
        LoadTexture("assets/demon_head.png"),
        demonStartPos,
        Vector2Zero(),
        0.0,
        Vector2Zero(),
        rHand, lHand,
        0.0, 0.0,
    };
    int w = demon.texture.width;
    int h = demon.texture.height;

    while (!WindowShouldClose()) {
        ClearBackground(WHITE);
        handleInputs(&demon);
        update(&demon, GetFrameTime() / EXPECTED_FRAME_TIME);

        BeginDrawing();
            Rectangle body = (Rectangle) {demon.pos.x, demon.pos.y, 2 * w, h};
            DrawEntity((Entity *)(&demon.rHand));
            DrawEntity((Entity *)(&demon.lHand));
            DrawEntityScaled((Entity *)&demon, 1 + (demon.height) / 30);
        EndDrawing();
    }

    UnloadTexture(demon.texture);
    UnloadTexture(demon.rHand.texture);
    UnloadTexture(demon.lHand.texture);
    CloseWindow();
}

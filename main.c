#include <stdio.h>
#include <math.h>

#include "raylib.h"
#include "raymath.h"

#include "data.h"
#include "drawing.h"
#include "mathUtils.h"
#include "world.h"
#include "constants.h"

#define DARKRED (Color){80, 10, 50, 255}

const int TARGET_FPS = 60;

void handleInputs(Demon *demon, Camera2D camera) {
    float accel = 3.5;
    if (demon->height == 0) {
        if (IsKeyDown(KEY_A)) demon->vel.x -= accel;
        if (IsKeyDown(KEY_D)) demon->vel.x += accel;
        if (IsKeyDown(KEY_W)) demon->vel.y -= accel;
        if (IsKeyDown(KEY_S)) demon->vel.y += accel;
        if (IsKeyDown(KEY_SPACE)) demon->zVel = 8;
    }
    demon->targetPos = GetScreenToWorld2D(GetMousePosition(), camera);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !demon->lHand.flying) {
        demon->lHand.flying = true;
        demon->lHand.targetPos = GetScreenToWorld2D(GetMousePosition(), camera);
        demon->lHand.speed = 3;
        demon->lHand.rot = Vector2Angle(demon->lHand.pos,
                                        demon->lHand.targetPos) - 90;
    }
    // Third condition works around a problem (bug?) where all buttons are 
    // 'pressed' when lift click is held.
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)
        && !demon->rHand.flying
        && !IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        demon->rHand.flying = true;
        demon->rHand.targetPos = GetScreenToWorld2D(GetMousePosition(), camera);
        demon->rHand.speed = 3;
        demon->rHand.rot = Vector2Angle(demon->rHand.pos,
                                        demon->rHand.targetPos) - 90;
    }
}

void moveHand(Hand *hand, Vector2 basePos, Vector2 neutralOffset, float rot) {
    Vector2 toTarget;

    if (!hand->flying) {
        Vector2 handOffset = Vector2Rotate(neutralOffset, DEG2RAD * rot);
        Vector2 targetPos = Vector2Add(basePos, handOffset);
        MoveTowardsPoint(&hand->pos, targetPos, hand->speed);
        float waving = sin(GetTime() * 4);
        hand->rot = rot - INITIAL_ROT_OFFSET + waving * 2;
    } else {
        hand->speed += hand->speed / 10;
        MoveTowardsPoint(&hand->pos,
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
    if (demon->height == 0) demon->vel = Vector2Scale(demon->vel, 0.75);
    if (Vector2Length(demon->vel) > 5) {
        demon->vel = Vector2ToLength(demon->vel, 5);
    }
    demon->pos = Vector2Add(demon->pos, demon->vel);
    demon->rot = Vector2Angle(demon->targetPos, demon->pos) + 90;
    updateHands(demon, delta);

    bool wasInAir = demon->height > 0;
    demon->height += demon->zVel;
    bool inAir = demon->height > 0;

    if (wasInAir && !inAir) {
        demon->trauma += 0.5;
    }

    if (inAir) {
        demon->zVel -= 0.5;
    } else {
        demon->height = 0;
        demon->zVel = 0;
    }
}

Demon initDemon() {
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

    return (Demon) {
        LoadTexture("assets/demon_head.png"),
        demonStartPos,
        Vector2Zero(),
        0.0,
        Vector2Zero(),
        rHand, lHand,
        0.0, 0.0, 0.0
    };
}

int main() {
    InitWindow(W_WIDTH, W_HEIGHT, "Test");
    SetTargetFPS(TARGET_FPS);

    Demon demon = initDemon();
    Map map = initMap();

    Camera2D camera;
    camera.target = demon.pos;
    camera.offset = (Vector2){W_WIDTH / 2, W_HEIGHT / 2};
    camera.rotation = 0;
    camera.zoom = 1;

    float EXPECTED_FRAME_TIME = 1 / TARGET_FPS;
    int w = demon.texture.width;
    int h = demon.texture.height;

    while (!WindowShouldClose()) {
        if (demon.trauma > 0) demon.trauma -= 0.01;
        ClearBackground(WHITE);
        handleInputs(&demon, camera);
        update(&demon, GetFrameTime() / EXPECTED_FRAME_TIME);
        updateCamera(&camera, demon);

        BeginDrawing();
        BeginMode2D(camera);
            drawMap(map);
            DrawEntity((Entity *)(&demon.rHand));
            DrawEntity((Entity *)(&demon.lHand));
            DrawEntityScaled((Entity *)&demon, 1 + (demon.height) / 80);
        EndMode2D();
        EndDrawing();
        camera.rotation = 0;
    }

    UnloadTexture(demon.texture);
    UnloadTexture(demon.rHand.texture);
    UnloadTexture(demon.lHand.texture);
    CloseWindow();
}

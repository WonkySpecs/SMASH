#include <stdio.h>
#include <math.h>

#include "raylib.h"

#include "entities.h"
#include "drawing.h"
#include "world.h"
#include "constants.h"
#include "mathUtils.h"

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

int main() {
    InitWindow(W_WIDTH, W_HEIGHT, "Test");
    SetTargetFPS(TARGET_FPS);

    Demon demon = initDemon();
    Map map = initMap();
    World world = { &map, &demon, };
    EnemyShooter enemy = {
        LoadTexture("assets/beast.png"),
        (Vector2) { 200, 200 }, (Vector2) { 1, 1 }, 0
    };
    world.enemies[0] = &enemy;
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
        updateDemon(&demon, GetFrameTime() / EXPECTED_FRAME_TIME);
        updateCamera(&camera, demon);
        updateEnemies(&world);

        BeginDrawing();
        BeginMode2D(camera);
            drawMap(map);
            drawEnemies(&world);
            drawEntity((Entity *)(&demon.rHand));
            drawEntity((Entity *)(&demon.lHand));
            drawEntityScaled((Entity *)&demon, 1 + (demon.height) / 80);
        EndMode2D();
        EndDrawing();
        camera.rotation = 0;
    }

    UnloadTexture(demon.texture);
    UnloadTexture(demon.rHand.texture);
    UnloadTexture(demon.lHand.texture);
    CloseWindow();
}

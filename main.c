#include <stdio.h>
#include <math.h>

#include "raylib.h"

#include "entities.h"
#include "drawing.h"
#include "world.h"
#include "constants.h"
#include "mathUtils.h"
#include "particles.h"

const int TARGET_FPS = 120;
const int NEUTRAL_FPS = 60;
bool DEBUG = false;

void setHandFlying(Hand *hand, Vector2 target) {
        hand->flying = true;
        hand->targetPos = target;
        hand->speed = 3;
        hand->rot = Vector2Angle(hand->pos,
                                 hand->targetPos) - 90;
}

void handleInputs(Demon *demon, Camera2D camera, float delta) {
    float accel = 3.5 * delta;
    if (demon->height == 0) {
        if (IsKeyDown(KEY_A)) demon->vel.x -= accel;
        if (IsKeyDown(KEY_D)) demon->vel.x += accel;
        if (IsKeyDown(KEY_W)) demon->vel.y -= accel;
        if (IsKeyDown(KEY_S)) demon->vel.y += accel;
        if (IsKeyDown(KEY_SPACE)) demon->zVel = 8;
        if (IsKeyPressed(KEY_GRAVE)) DEBUG = !DEBUG;
        demon->breathingFire = IsKeyDown(KEY_E);
    }
    demon->targetPos = GetScreenToWorld2D(GetMousePosition(), camera);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !demon->lHand.flying) {
        setHandFlying(&demon->lHand, GetScreenToWorld2D(GetMousePosition(), camera));
    }
    // Third condition works around a problem (bug?) where all buttons are 
    // 'pressed' when lift click is held.
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)
        && !demon->rHand.flying
        && !IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        setHandFlying(&demon->rHand, GetScreenToWorld2D(GetMousePosition(), camera));
    }
}

int main() {
    InitWindow(W_WIDTH, W_HEIGHT, "Test");
    SetTargetFPS(TARGET_FPS);

    Demon demon = initDemon();
    Map map = initMap();
    ParticleLayer pl = (ParticleLayer) {
        newParticleEmitter(demon.pos, LoadTexture("assets/lava_0.png")), 0, 0, 0
    };
    World world = { &map, &demon, .particles = &pl};
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

    float EXPECTED_FRAME_TIME = 1 / (float)NEUTRAL_FPS;
    int w = demon.texture.width;
    int h = demon.texture.height;

    RenderTexture2D particleTex = LoadRenderTexture(W_WIDTH, W_HEIGHT);

    while (!WindowShouldClose()) {
        float delta = GetFrameTime() / EXPECTED_FRAME_TIME;
        if (demon.trauma > 0) demon.trauma -= 0.01;
        ClearBackground(BLACK);
        handleInputs(&demon, camera, delta);
        updateDemon(&world, delta);
        updateCamera(&camera, demon);
        updateEnemies(&world);
        updateParticleEffects(&world, delta);

        BeginDrawing();
        BeginMode2D(camera);
            BeginTextureMode(particleTex);
                ClearBackground(BLANK);
                drawParticleLayer(*world.particles, camera);
            EndTextureMode();
            drawMap(map, camera);
            drawEnemies(&world, camera);
            drawEntity((Entity *)(&demon.rHand), camera);
            drawEntity((Entity *)(&demon.lHand), camera);
            DrawTextureRec(particleTex.texture,
                           (Rectangle){ 0, 0,
                                        particleTex.texture.width, 
                                        -particleTex.texture.height },
                           (Vector2){ 0, 0 }, WHITE);
            drawEntityScaled((Entity *)&demon, 1 + (demon.height) / 80, camera);

        if (DEBUG) {
            DrawFPS(10, 10);
            for (int i = 0; i < world.map->numObstacles; i++) {
                Rectangle obst = world.map->obstacles[i];
                Vector2 pos = GetWorldToScreen2D((Vector2){obst.x, obst.y}, camera);
                DrawRectangle(pos.x, pos.y, obst.width, obst.height, PINK);
            }
            DrawCircleV(GetWorldToScreen2D(demon.pos, camera), 20, GREEN);
        }
        EndMode2D();
        EndDrawing();
    }

    UnloadTexture(demon.texture);
    UnloadTexture(demon.rHand.texture);
    UnloadTexture(demon.lHand.texture);
    CloseWindow();
}

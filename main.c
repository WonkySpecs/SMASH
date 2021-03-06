#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "raylib.h"

#include "entities.h"
#include "drawing.h"
#include "world.h"
#include "constants.h"
#include "mathUtils.h"
#include "particles.h"
#include "levels.h"

const int TARGET_FPS = 120;
const int NEUTRAL_FPS = 60;
bool DEBUG = false;

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

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && demon->lHand.state == NEUTRAL) {
        setHandFlying(&demon->lHand, GetScreenToWorld2D(GetMousePosition(), camera));
    }
    // Third condition works around a problem (bug?) where all buttons are 
    // 'pressed' when lift click is held.
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)
        && demon->rHand.state == NEUTRAL
        && !IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        setHandFlying(&demon->rHand, GetScreenToWorld2D(GetMousePosition(), camera));
    }
}

const int INIT_ENEMIES_ALLOCATED = 100;
const int INIT_PROJ_ALLOCATED = 100;

void initWorld(World *world) {
    Demon *demon = calloc(1, sizeof(Demon));
    initDemon(demon);
    Map *map = calloc(1, sizeof(Map));
    initMap(map);
    world->enemies = calloc(INIT_ENEMIES_ALLOCATED, sizeof(Enemy));
    world->enemiesAllocated = INIT_ENEMIES_ALLOCATED;
    Projectile *projectiles = calloc(INIT_PROJ_ALLOCATED, sizeof(Projectile));
    ParticleLayer *pl = calloc(1, sizeof(ParticleLayer));
    initParticleLayer(pl);

    world->map = map;
    world->demon = demon;
    world->projectiles = projectiles;
    world->projAllocated = INIT_PROJ_ALLOCATED;
    world->particles = pl;
}

int main() {
    InitWindow(W_WIDTH, W_HEIGHT, "Demon SMASH");
    SetTargetFPS(TARGET_FPS);

    World world = {0};
    initWorld(&world);

    levelUpdateFn levelUpdate = loadLevelUpdate(INFINITE_IMPS);

    Camera2D camera = initCamera(world.demon->pos);
    const float EXPECTED_FRAME_TIME = 1 / (float)NEUTRAL_FPS;

    RenderTexture2D particleTex = LoadRenderTexture(W_WIDTH, W_HEIGHT);

    while (!WindowShouldClose()) {
        float delta = GetFrameTime() / EXPECTED_FRAME_TIME;

        handleInputs(world.demon, camera, delta);

        updateDemon(&world, delta);
        updateCamera(&camera, *world.demon);
        updateEnemies(&world, delta);
        updateProjectiles(&world, delta);
        updateParticleEffects(&world, delta);
        levelUpdate(&world, delta);

        ClearBackground(BLACK);

        BeginDrawing();
        BeginMode2D(camera);
            BeginTextureMode(particleTex);
                ClearBackground(BLANK);
                drawParticleLayer(*world.particles, camera);
            EndTextureMode();
            drawMap(*world.map, camera);
            drawEnemiesAndProj(&world, camera);
            drawEntity((Entity *)(&world.demon->rHand), camera);
            drawEntity((Entity *)(&world.demon->lHand), camera);
            DrawTextureRec(particleTex.texture,
                           (Rectangle){ 0, 0,
                                        particleTex.texture.width, 
                                        -particleTex.texture.height },
                           (Vector2){ 0, 0 }, WHITE);
            drawEntityScaled((Entity *)world.demon,
                             1 + (world.demon->height) / 80,
                             camera);

            if (DEBUG) {
                DrawFPS(10, 10);
                for (int i = 0; i < world.map->numObstacles; i++) {
                    Rectangle obst = world.map->obstacles[i];
                    Vector2 pos = GetWorldToScreen2D(rectPos(obst),
                                                     camera);
                    DrawRectangle(pos.x, pos.y, obst.width, obst.height, PINK);
                }
                drawEntityHitbox((Entity*)world.demon, camera, GREEN);
                drawEntityHitbox((Entity*)&world.demon->rHand, camera, BLUE);
                drawEntityHitbox((Entity*)&world.demon->lHand, camera, BLUE);
                for (int i = 0; i < world.enemiesAllocated; i++) {
                    drawEntityHitbox((Entity*)&world.enemies[i], camera, RED);
                }
            }
        EndMode2D();
        EndDrawing();
    }

    UnloadTexture(world.demon->texture);
    UnloadTexture(world.demon->rHand.texture);
    UnloadTexture(world.demon->lHand.texture);
    CloseWindow();
}

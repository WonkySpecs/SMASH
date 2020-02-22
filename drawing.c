#include "raymath.h"
#include "drawing.h"
#include "constants.h"
#include "mathUtils.h"

Vector2 lastCameraTarget;

Camera2D initCamera(Vector2 target) {
    Camera2D camera;
    camera.target = target;
    camera.offset = (Vector2){W_WIDTH / 2, W_HEIGHT / 2};
    camera.rotation = 0;
    camera.zoom = 1;

    lastCameraTarget = target;
    return camera;
}

void drawEntityScaled(Entity *e, float scale, Camera2D camera) {
    Texture tex = e->texture;
    float w = tex.width;
    float h = tex.height;
    Vector2 screenPos = GetWorldToScreen2D(e->pos, camera);
    DrawTexturePro(tex,
                   (Rectangle){0, 0, w, h},
                   (Rectangle){screenPos.x, screenPos.y, w * scale, h * scale},
                   (Vector2){(w / 2) * scale, (h / 2) * scale},
                   e->rot,
                   WHITE);
}

void drawEntity(Entity *e, Camera2D camera) {
    drawEntityScaled(e, 1.0, camera);
}

void updateCamera(Camera2D *camera, Demon demon) {
    camera->target = Vector2Lerp(lastCameraTarget, demon.pos, 0.08);
    lastCameraTarget = camera->target;
    float baseOffset = 40 * demon.trauma * demon.trauma;
    camera->offset = Vector2Add((Vector2){W_WIDTH / 2, W_HEIGHT / 2},
                                (Vector2){baseOffset * (randFloat() * 2 - 1),
                                          baseOffset * (randFloat() * 2 - 1)});
    camera->rotation = 5 * demon.trauma * demon.trauma * (randFloat() - 0.5);
}

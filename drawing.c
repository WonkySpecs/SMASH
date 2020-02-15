#include "utils.h"
#include "raymath.h"
#include "drawing.h"
#include "constants.h"

void DrawEntityScaled(Entity *e, float scale) {
    Texture tex = e->texture;
    float w = tex.width;
    float h = tex.height;
    DrawTexturePro(tex,
                   (Rectangle){0, 0, w, h},
                   (Rectangle){e->pos.x, e->pos.y, w * scale, h * scale},
                   (Vector2){(w / 2) * scale, (h / 2) * scale},
                   e->rot,
                   WHITE);
}

void DrawEntity(Entity *e) {
    DrawEntityScaled(e, 1.0);
}

void updateCamera(Camera2D *camera, Demon demon) {
    camera->target = demon.pos;
    float baseOffset = 30 * demon.trauma * demon.trauma;
    camera->offset = Vector2Add((Vector2){W_WIDTH / 2, W_HEIGHT / 2},
                                (Vector2){baseOffset * randFloat(),
                                          baseOffset * randFloat()});
    camera->rotation = 5 * demon.trauma * demon.trauma * (randFloat() - 0.5);
}

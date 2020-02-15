#include "drawing.h"

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

#ifndef __DRAWING
#define __DRAWING

#include "raylib.h"
#include "entities.h"

typedef enum TextureName {
    TEX_DEMON_HEAD, TEX_DEMON_HAND, TEX_IMP,
    TEX_NAMES_SIZE
} TextureName;

Camera2D initCamera(Vector2 target);
void drawEntityScaled(Entity *e, float scale, Camera2D camera);
void drawEntity(Entity *e, Camera2D camera);
void updateCamera(Camera2D *camera, Demon demon);
void drawEntityHitbox(Entity *e, Camera2D camera, Color c);

Texture2D getTexture(TextureName tex);
#endif

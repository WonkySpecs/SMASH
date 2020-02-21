#ifndef __DRAWING
#define __DRAWING

#include "raylib.h"
#include "entities.h"

void drawEntityScaled(Entity *e, float scale, Camera2D camera);
void drawEntity(Entity *e, Camera2D camera);
void updateCamera(Camera2D *camera, Demon demon);
#endif

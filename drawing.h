#ifndef __DRAWING
#define __DRAWING

#include "raylib.h"
#include "entities.h"

void drawEntityScaled(Entity *e, float scale);
void drawEntity(Entity *e);
void updateCamera(Camera2D *camera, Demon demon);
#endif

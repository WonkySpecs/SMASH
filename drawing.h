#ifndef __DRAWING
#define __DRAWING

#include "raylib.h"
#include "data.h"

void DrawEntityScaled(Entity *e, float scale);
void DrawEntity(Entity *e);
void updateCamera(Camera2D *camera, Demon demon);
#endif

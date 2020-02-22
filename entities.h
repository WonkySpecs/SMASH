#ifndef __ENTITIES
#define __ENTITIES

#include "gameTypes.h"
#include "raylib.h"

Demon initDemon();
void updateDemon(World *world, float delta);
void updateEnemies(World *world);
void drawEnemies(World *world, Camera2D camera);
#endif

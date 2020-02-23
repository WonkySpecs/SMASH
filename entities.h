#ifndef __ENTITIES
#define __ENTITIES

#include "gameTypes.h"
#include "raylib.h"

Demon initDemon();
void setHandFlying(Hand *hand, Vector2 target);
void updateDemon(World *world, float delta);
void updateEnemies(World *world);
void drawEnemies(World *world, Camera2D camera);

/* Functions for enemy updates. These can be removed once constructors
for enemy types are included in entities.c instead of happening in main */
void updateImp(Enemy *self, World *world);
#endif

#ifndef __ENTITIES
#define __ENTITIES

#include "gameTypes.h"
#include "raylib.h"

void initDemon(Demon *demon);
void setHandFlying(Hand *hand, Vector2 target);
void updateDemon(World *world, float delta);
void updateEnemies(World *world, float delta);
void updateProjectiles(World *world, float delta);
void drawEnemiesAndProj(World *world, Camera2D camera);
bool didEntitiesCollide(Entity *e1, Entity *e2);

/* Functions for enemy updates. These can be removed once constructors
for enemy types are included in entities.c instead of happening in main */
void updateImp(Enemy *self, World *world, float delta);
#endif

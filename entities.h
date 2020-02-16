#ifndef __ENTITIES
#define __ENTITIES

#include "gameTypes.h"

Demon initDemon();
void updateDemon(Demon *demon, float delta);
void updateEnemies(World *world);
void drawEnemies(World *world);
#endif

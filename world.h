#ifndef __WORLD
#define __WORLD

#include "gameTypes.h"

void initMap(Map *map);
void drawMap(Map map, Camera2D camera);

void addEnemy(World *world, Enemy enemy);
void addProjectile(World *world, Projectile proj);

bool isOffMap(Vector2 pos, Map map);
#endif

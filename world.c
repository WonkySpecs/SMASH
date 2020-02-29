#include "world.h"
#include <stdlib.h>
#include "tiles.h"

bool isLava(int x, int y) {
    int c = x * x + y * y;
    return c > 250 && c < 500;
}

void initMap(Map *map) {
    map->tileTex = loadTiles();

    for(int x = 0; x < MAP_WIDTH; x++) {
        for(int y = 0; y < MAP_HEIGHT; y++) {
            TileCategory cat = isLava(x, y) ? LAVA : GROUND;
            map->tiles[x][y] = tileOffsets[randomTile(cat)];
        }
    }
    const int numObstacles = 4;
    const int w = MAP_WIDTH * TILE_SIZE;
    const int h = MAP_HEIGHT * TILE_SIZE;
    map->obstacles = (Rectangle *)malloc(numObstacles * sizeof(Rectangle));
    map->obstacles[0] = (Rectangle) {0, -10, w, 10};
    map->obstacles[1] = (Rectangle) {-10, 0, 10, h};
    map->obstacles[2] = (Rectangle) {0, h, MAP_WIDTH * TILE_SIZE + 9, 10};
    map->obstacles[3] = (Rectangle) {w, 0, 10, h};
    map->numObstacles = numObstacles;
    return map;
}

void drawMap(Map map, Camera2D camera) {
    for(int x = 0; x < MAP_WIDTH; x++) {
        for(int y = 0; y < MAP_HEIGHT; y++) {
            Vector2 screenPos = GetWorldToScreen2D(
                (Vector2){x * TILE_SIZE, y * TILE_SIZE},
                camera);
            Rectangle dest = (Rectangle) {
                screenPos.x, screenPos.y,
                map.tileTex.width * tilePercentSize.x,
                map.tileTex.height * tilePercentSize.y
            };
            DrawTextureQuad(map.tileTex,
                            tilePercentSize,
                            map.tiles[x][y],
                            dest,
                            WHITE);
        }
    }
}

static const int addEnemyAllocs = 50;
void allocMoreEnemies(World *world) {
    world->enemiesAllocated += addEnemyAllocs;
    world->enemies = realloc(world->enemies, world->enemiesAllocated * sizeof(Enemy));
    for (int i = 0; i < addEnemyAllocs; i++){
        world->enemies[world->enemiesAllocated - (i + 1)].active = false;
    }
}

void addEnemy(World *world, Enemy enemy) {
    int idx = world->enemiesAllocated;
    for (int i = 0; i < world->enemiesAllocated; i++) {
        if (!world->enemies[i].active) {
            idx = i;
            break;
        }
    }
    if (idx == world->enemiesAllocated) {
        allocMoreEnemies(world);
    }
    world->enemies[idx] = enemy;
}

static const int addProjAllocs = 100;
void allocMoreProjectiles(World *world) {
    world->projAllocated += addProjAllocs;
    world->projectiles = realloc(world->projectiles,
                                 world->projAllocated * sizeof(Projectile));
    for (int i = 0; i < addProjAllocs; i++){
        world->projectiles[world->projAllocated - (i + 1)].active = false;
    }
}

void addProjectile(World *world, Projectile proj) {
    int idx = world->projAllocated;
    for (int i = 0; i < world->projAllocated; i++) {
        if (!world->projectiles[i].active) {
            idx = i;
            break;
        }
    }
    if (idx == world->projAllocated) {
        allocMoreProjectiles(world);
    }
    world->projectiles[idx] = proj;
}

bool isOffMap(Vector2 pos, Map map) {
    return (pos.x < 0 || pos.y < 0
         || pos.x > MAP_WIDTH * TILE_SIZE || pos.y > MAP_HEIGHT * TILE_SIZE);
}

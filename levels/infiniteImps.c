#include "infiniteImps.h"
#include "../world.h"
#include "../drawing.h"

static int _wave_size = 4;
static int _wave_number = 0;
static float levelTimer = 0;
static bool initialWaveSpawned = false;

void spawnWave(int numImps, World *world) {
    Texture2D tex = getTexture(TEX_IMP);
    for (int i = 0; i < numImps; i++) {
        Vector2 initPos = (Vector2){randFloat() * 800 + 200, randFloat() * 500 + 200};
        Enemy enemy = {
            .texture = tex,
            .pos = initPos,
            .rot = 0, .maxSpeed = 2 + randFloat() - 0.5,
            .targetPos = (Vector2) {0, 0}, .update = &updateImp,
            .primThresh = 100, .primTimer = (int)(randFloat() * 100),
            .state = NEUTRAL,
            .active = true
        };
        addEnemy(world, enemy);
    }
}

void infiniteImps_update(World *world, float delta) {
    levelTimer += delta;
    if (!initialWaveSpawned) {
        if (levelTimer > 50) {
            spawnWave(_wave_size * 2, world);
            initialWaveSpawned = true;
            levelTimer = 0;
        }
    } else {
        if (levelTimer > 100) {
            levelTimer = 0;
            _wave_number++;
            if (_wave_number % 5 == 0) _wave_size++;
            spawnWave(_wave_size, world);
        }
    }
}

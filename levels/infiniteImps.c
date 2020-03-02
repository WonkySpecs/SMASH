#include "infiniteImps.h"

static int _wave_size = 4;

void infiniteImps_update(World *world, float delta) {
    printf("%d\n", _wave_size++);
}

#ifndef __LEVEL
#define __LEVEL

/*
    Current plan is that each level will have it's own source file.
    Each file contains a bunch of static variables that define the level state,
    and a function which acts as the interface to the level ie.

    void levelName_update(World *world, float delta);

    as well as one for loading the level. This would give map data etc

    We then have a container struct for the world and level, where the level is a
    function pointer to the update function for the currently loaded level.

    This levels.h/c will be used for handling that interface with the actual level
    data.
*/

#include "gameTypes.h"
#include "levels/infiniteImps.h"

typedef enum Level {
    INFINITE_IMPS
} Level;

typedef void (*levelUpdateFn)(World*, float);

levelUpdateFn loadLevelUpdate(Level level) {
    switch (level) {
        case INFINITE_IMPS:
            return infiniteImps_update;
            break;
    };
}
#endif

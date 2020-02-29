#ifndef __GAMETYPES
#define __GAMETYPES

#include "raylib.h"
#include "constants.h"
#include "mathUtils.h"

#define BASEFIELDS struct {\
	Texture2D texture;\
	Vector2 pos;\
	Vector2 vel;\
	float rot;\
    bool active; \
    Hitbox offsetHitbox; \
}

typedef struct World World;

// ### Entities ###
typedef enum EntityState {
    NEUTRAL, PREPARING, ATTACKING, RECOVERING,
} EntityState;

typedef struct Entity {
    BASEFIELDS;
} Entity;

typedef Entity Projectile;

typedef struct Hand {
    BASEFIELDS;
    float speed;
    Vector2 targetPos;
    EntityState state;
} Hand;

typedef struct Demon {
    BASEFIELDS;
    Vector2 targetPos;
    Hand rHand;
    Hand lHand;
    float height;
    float zVel;
    float trauma; // Not convinced this should be here
    bool breathingFire;
} Demon;

typedef struct Enemy Enemy;
struct Enemy {
    BASEFIELDS;
    void (*update)(Enemy*, World*, float);
    Vector2 targetPos;
    float maxSpeed;
    float accel;
    float primTimer;
    float primThresh;
    EntityState state;
};

// #############

// ### Map ###
typedef struct Tile {
    Texture2D texture;
} Tile;

typedef struct Map {
    Texture2D tileTex;
    Vector2 tiles[MAP_WIDTH][MAP_HEIGHT];
    Rectangle *obstacles;
    int numObstacles;
} Map;

// #############

// ### Particles ###
typedef struct Particle {
    Vector2 pos, vel;
    float age, lifetime;
} Particle;

typedef struct ParticleEmitter {
    Particle *particles;
    int numParticles;
    int lastReplaced;
    Texture2D texture;
    bool active;
    Vector2 pos;
    float emissionRate;
    float sinceEmission;
    float facing; // rad
    float emitArc; // rad
} ParticleEmitter;

typedef struct ParticleLayer {
    ParticleEmitter fireBreath;
    ParticleEmitter lHand;
    ParticleEmitter rHand;
    ParticleEmitter *dynamicEffects;
} ParticleLayer;
// #############

typedef struct World {
    Map *map;
    Demon *demon;
    Enemy *enemies;
    int enemiesAllocated;
    Projectile *projectiles;
    int projAllocated;
    ParticleLayer *particles;
} World;
#endif

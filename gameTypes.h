#ifndef __GAMETYPES
#define __GAMETYPES

#include "raylib.h"
#include "constants.h"

#define BASEFIELDS struct {\
	Texture2D texture;\
	Vector2 pos;\
	Vector2 vel;\
	float rot;\
}

typedef struct Entity {
    BASEFIELDS;
} Entity;

typedef struct Hand {
    BASEFIELDS;
    float speed;
    bool flying;
    Vector2 targetPos;
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
typedef struct World World;
struct Enemy {
    BASEFIELDS;
    void (*update)(Enemy*, World*);
    float fireDelay;
    float sinceFired;
};


typedef struct Tile {
    Texture2D texture;
} Tile;

typedef struct Map {
    Texture2D tileTex;
    Vector2 tiles[MAP_WIDTH][MAP_HEIGHT];
    Rectangle *obstacles;
    int numObstacles;
} Map;

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

typedef struct World {
    Map *map;
    Demon *demon;
    Enemy *enemies[MAX_ENEMIES];
    ParticleLayer *particles;
} World;
#endif

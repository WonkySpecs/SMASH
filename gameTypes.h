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

typedef enum EnemyType {
    ENEMY_IMP,
} EnemyType;


typedef struct Enemy {
    BASEFIELDS;
    EnemyType type;
} Enemy;

typedef struct EnemyShooter {
    BASEFIELDS;
    EnemyType type;
    float fireDelay;
    float sinceFired;
} EnemyShooter;

typedef struct Tile {
    Texture2D texture;
} Tile;

typedef struct Map {
    Tile tiles[MAP_WIDTH][MAP_HEIGHT];
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

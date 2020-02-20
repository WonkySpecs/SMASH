#ifndef __PARTICLES
#define __PARTICLES

#include "raylib.h"
#include <stdlib.h>

#define MAX_PARTICLES 1000 // This may or may not stay like this

typedef struct Particle {
    float life;
    Vector2 pos, vel;
} Particle;

typedef struct ParticleEmitter {
    Particle *particles;
    int numParticles;
    Texture2D texture;
    Vector2 pos;
    float emissionRate;
    float sinceEmission;
    float facing; // rad
    float emitArc; // rad
} ParticleEmitter;

ParticleEmitter newParticleEmitter(Vector2 pos, Texture2D texture);
void updateParticleEmitter(ParticleEmitter *emitter, float delta);
void drawParticles(ParticleEmitter *emitter, Camera2D camera);
#endif

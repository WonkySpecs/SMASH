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
    Particle **particles;
    int numParticles;
    Texture2D texture;
    Shader shader;
    Vector2 pos;
} ParticleEmitter;

ParticleEmitter newParticleEmitter(Vector2 pos, Texture2D texture, Shader shader);
void updateParticles(ParticleEmitter *emitter, float delta);
void drawParticles(ParticleEmitter *emitter, Camera2D camera);
#endif

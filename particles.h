#ifndef __PARTICLES
#define __PARTICLES

#include "raylib.h"
#include <stdlib.h>
#include "gameTypes.h"

ParticleEmitter newParticleEmitter(Vector2 pos, Texture2D texture);
void updateParticleEmitter(ParticleEmitter *emitter, float delta);
void drawParticles(ParticleEmitter *emitter, Camera2D camera);
void updateParticleEffects(World *world, float delta);
void drawParticleLayer(ParticleLayer particleLayer, Camera2D camera);
#endif

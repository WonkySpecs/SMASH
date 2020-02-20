#include <stdio.h>
#include "particles.h"
#include "raymath.h"
#include "constants.h"
#include "math.h"
#include "utils.h"


void drawParticles(ParticleEmitter *emitter, Camera2D camera) {
    for (int i = 0; i < emitter->numParticles; i++) {
        Particle *p = emitter->particles[i];
        if (p == 0) continue;
        if (p->life <= 0) continue;
        Vector2 pos = GetWorldToScreen2D((Vector2){emitter->pos.x + p->pos.x,
                                emitter->pos.y + p->pos.y},
                                camera);
        DrawRectanglePro((Rectangle){pos.x, pos.y, 5, 5},
                         (Vector2){0.5, 0.5},
                         RAD2DEG * sin(GetTime()),
                         RED);
    }
}

void expandEmitterParticles(ParticleEmitter *emitter) {
}

void emitParticle(ParticleEmitter *emitter) {
    int idx = MAX_PARTICLES + 1;
    for (int i = 0; i < emitter->numParticles; i++) {
        if (emitter->particles[i]->life <= 0) {
            idx = i;
            break;
        }
    }

    // Assumes expandEmitterParticles is successful. Maybe wrap if not?
    if (idx == MAX_PARTICLES + 1) {
        idx = emitter->numParticles;
        expandEmitterParticles(emitter);
    }

    // TODO: This should depend on the emitter
    Vector2 v = (Vector2){randFloat() * 6 - 3, randFloat() * 6 - 3};
    v = Vector2Scale(v, 3.0 / Vector2Length(v));

    Particle *p = emitter->particles[idx];
    p->life = 10;
    p->pos = (Vector2){0, 0};
    p->vel = v;
}

void updateParticles(ParticleEmitter *emitter, float delta) {
    for (int i = 0; i < emitter->numParticles; i++) {
        Particle *p = emitter->particles[i];
        if (p == 0) continue;
        if (p->life <= 0) continue;
        p->life -= 2 * delta;
        p->pos = Vector2Add(p->pos, p->vel);
    }
}

void updateParticleEmitter(ParticleEmitter *emitter, float delta) {
    emitter-> sinceEmission += delta;
    if (emitter->sinceEmission > emitter->emissionRate) {
        emitParticle(emitter);
    }
    updateParticles(emitter, delta);
}

const int START_PARTICLES = 10;
ParticleEmitter newParticleEmitter(Vector2 pos, Texture2D texture) {
    Particle **particles = (Particle **)malloc(sizeof(Particle *) * START_PARTICLES);
    for (int i = 0; i < START_PARTICLES; i++) {
        // I think this is ultra mega inefficent. Should malloc larger blocks
        Particle *p = (Particle *)malloc(sizeof(Particle));
        *p = (Particle) { 0.0 };
        particles[i] = p;
    }
    return (ParticleEmitter) {
        particles, 10, texture, pos, 50, 50
    };
}

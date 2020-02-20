#include <stdio.h>
#include "particles.h"
#include "raymath.h"
#include "constants.h"
#include "math.h"
#include "utils.h"


void drawParticles(ParticleEmitter *emitter, Camera2D camera) {
    for (int i = 0; i < emitter->numParticles; i++) {
        Particle p = emitter->particles[i];
        if (p.life <= 0) continue;
        Vector2 pos = GetWorldToScreen2D((Vector2){emitter->pos.x + p.pos.x,
                                emitter->pos.y + p.pos.y},
                                camera);
        DrawRectanglePro((Rectangle){pos.x, pos.y, 5, 5},
                         (Vector2){0.5, 0.5},
                         RAD2DEG * sin(GetTime()),
                         RED);
    }
}

void expandEmitterParticles(ParticleEmitter *emitter) {
    const int expandBy = 20;
    emitter->numParticles += expandBy;
    emitter->particles = (Particle *)realloc(emitter->particles,
                                             emitter->numParticles * sizeof(Particle));
}

void emitParticle(ParticleEmitter *emitter) {
    int idx = MAX_PARTICLES + 1;
    for (int i = 0; i < emitter->numParticles; i++) {
        if (emitter->particles[i].life <= 0) {
            idx = i;
            break;
        }
    }

    // Assumes expandEmitterParticles is successful. Maybe wrap if not?
    if (idx == MAX_PARTICLES + 1) {
        idx = emitter->numParticles;
        expandEmitterParticles(emitter);
    }

    Vector2 v = (Vector2){randFloat() * 2 + 1, 0};
    float rot = emitter->facing +
                        (randFloat() * emitter->emitArc) - emitter->emitArc / 2;
    float c = cos(rot);
    float s = sin(rot);
    v = (Vector2){v.x * c - v.y * s, v.x * s + v.y * c};

    Particle *p = &emitter->particles[idx];
    p->life = 30;
    p->pos = (Vector2){0, 0};
    p->vel = v;
}

void updateParticles(ParticleEmitter *emitter, float delta) {
    for (int i = 0; i < emitter->numParticles; i++) {
        Particle *p = &emitter->particles[i];
        if (p->life <= 0) continue;
        p->life -= 0.5 * delta;
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
    Particle *particles = (Particle *)malloc(sizeof(Particle) * START_PARTICLES);
    for (int i = 0; i < START_PARTICLES; i++) {
        particles[i] = (Particle){0};
    }

    return (ParticleEmitter) {
        particles, 10, texture, pos, 10, 50, 0, 180 * DEG2RAD
    };
}

#include <stdio.h>
#include "particles.h"
#include "raymath.h"
#include "constants.h"
#include "math.h"

void drawParticles(ParticleEmitter *emitter, Camera2D camera) {
    for (int i = 0; i < emitter->numParticles; i++) {
        Particle *p = emitter->particles[i];
        if (p == 0) continue;
        if (p->life <= 0) continue;
        Vector2 pos = (Vector2){emitter->pos.x + p->pos.x,
                                emitter->pos.y + p->pos.y};
        DrawRectanglePro((Rectangle){pos.x, pos.y, 5, 5},
                         (Vector2){0.5, 0.5}, RAD2DEG * sin(GetTime()), RED);
    }
}

void emitParticles(ParticleEmitter *emitter) {
}

void updateParticles(ParticleEmitter *emitter, float delta) {
    for (int i = 0; i < emitter->numParticles; i++) {
        Particle *p = emitter->particles[i];
        if (p == 0) continue;
        if (p->life <= 0) continue;
        // p->life -= delta;
        p->pos = Vector2Add(p->pos, p->vel);
    }
}

const int START_PARTICLES = 10;
ParticleEmitter newParticleEmitter(Vector2 pos, Texture2D texture, Shader shader) {
    Particle **particles = (Particle **)malloc(sizeof(Particle *) * START_PARTICLES);
    for (int i = 0; i < START_PARTICLES; i++) {
        // I think this is ultra mega inefficent. Should malloc larger blocks
        Particle *p = (Particle *)malloc(sizeof(Particle));
        *p = (Particle) {
            100.0, (Vector2){0, 0}, (Vector2){i * 0.1, 0.0}
        };
        particles[i] = p;
    }
    return (ParticleEmitter) {
        particles, 10, texture, shader, pos
    };
}

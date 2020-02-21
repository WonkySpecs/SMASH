#include <stdio.h>
#include "particles.h"
#include "raymath.h"
#include "constants.h"
#include "math.h"
#include "mathUtils.h"


#define MAX_PARTICLES 1000 // This may or may not stay like this

void drawParticles(ParticleEmitter *emitter, Camera2D camera) {
    for (int i = 0; i < emitter->numParticles; i++) {
        Particle p = emitter->particles[i];
        float g = fmax(0, 255 - Vector2Length(p.pos));
        Color c = (Color){255, g, g / 3, 255};
        if (p.age >= p.lifetime) continue;
        Vector2 pos = GetWorldToScreen2D((Vector2){emitter->pos.x + p.pos.x,
                                emitter->pos.y + p.pos.y},
                                camera);
        float size = 1 - p.age / p.lifetime;
        DrawRectanglePro((Rectangle){pos.x, pos.y, 8 * size, 8 * size},
                         (Vector2){0.5, 0.5},
                         RAD2DEG * sin(GetTime()),
                         c);
    }
}

void expandEmitterParticles(ParticleEmitter *emitter) {
    const int expandBy = 100;
    emitter->numParticles += expandBy;
    emitter->particles = (Particle *)realloc(emitter->particles,
                                             emitter->numParticles * sizeof(Particle));
}

void newFireParticle(Particle *p, float facing, float arc) {
    Vector2 v = (Vector2){1, 0};
    float rot = facing + (randFloat() * arc) - arc / 2;
    float c = cos(rot);
    float s = sin(rot);
    v = (Vector2){v.x * c - v.y * s, v.x * s + v.y * c};

    if (randFloat() > 0.5) {
        p->vel = Vector2Scale(v, randFloat() * 4 + 3);
        p->lifetime = 20;
    } else {
        p->vel = Vector2Scale(v, randFloat() * 1 + 2);
        p->lifetime = 70;
    }

    p->age = 0;
    p->pos = (Vector2){0, 0};
}

void emitParticle(ParticleEmitter *emitter) {
    int idx = MAX_PARTICLES + 1;
    for (int i = 0; i < emitter->numParticles; i++) {
        Particle p = emitter->particles[i];
        if (p.age >= p.lifetime) {
            idx = i;
            break;
        }
    }

    // Assumes expandEmitterParticles is successful. Maybe wrap if not?
    if (idx == MAX_PARTICLES + 1) {
        idx = emitter->numParticles;
        expandEmitterParticles(emitter);
    }

    Particle *p = &emitter->particles[idx];
    newFireParticle(p, emitter->facing, emitter->emitArc);
}

void updateParticles(ParticleEmitter *emitter, float delta) {
    for (int i = 0; i < emitter->numParticles; i++) {
        Particle *p = &emitter->particles[i];
        if (p->age >= p->lifetime) continue;
        p->vel.x += (randFloat() * 0.4- 0.2) * delta;
        p->vel.y += (randFloat() * 0.4- 0.2) * delta;
        p->age += 0.5 * delta;
        p->pos = Vector2Add(p->pos, Vector2Scale(p->vel, delta));
    }
}

void updateParticleEmitter(ParticleEmitter *emitter, float delta) {
    emitter-> sinceEmission += delta;
    if (emitter->active) {
        while (emitter->sinceEmission > emitter->emissionRate) {
            emitParticle(emitter);
            emitter->sinceEmission -= emitter->emissionRate;
        }
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
        particles, 10, texture, false, pos, 0.2, 50, 0, 100 * DEG2RAD
    };
}

void drawParticleLayer(ParticleLayer particleLayer, Camera2D camera) {
    drawParticles(&particleLayer.fireBreath, camera);
}

void updateParticleEffects(World *world, float delta) {
    ParticleLayer *pl = world->particles;
    Demon demon = *world->demon;
    ParticleEmitter *fireBreath = &pl->fireBreath;
    fireBreath->active = demon.breathingFire;
    fireBreath->facing = (demon.rot + 90) * DEG2RAD;
    fireBreath->pos = Vector2Add(demon.pos, VecLenAngle(15, fireBreath->facing));
    updateParticleEmitter(fireBreath, delta);
}

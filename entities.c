#include <stdio.h>

#include "entities.h"
#include "raymath.h"
#include "mathUtils.h"
#include "constants.h"
#include "drawing.h"
#include "particles.h"

void moveHand(Hand *hand, Vector2 basePos, Vector2 neutralOffset, float rot);
void updateHands(Demon *demon, float delta); 

Demon initDemon() {
    Vector2 demonStartPos = (Vector2){W_WIDTH / 2, W_HEIGHT / 2};
    Image hand = LoadImage("assets/demon_hand.png");
    Hand rHand = {
        LoadTextureFromImage(hand),
        demonStartPos,
        Vector2Zero(),
        0,
        NEUTRAL_HAND_SPEED,
        false,
        Vector2Zero(),
        newParticleEmitter(demonStartPos,
                           LoadTexture("assets/lava_0.png"))
    };
    ImageFlipHorizontal(&hand);
    Hand lHand = {
        LoadTextureFromImage(hand),
        demonStartPos,
        Vector2Zero(),
        0,
        NEUTRAL_HAND_SPEED,
        false,
    };
    UnloadImage(hand);

    return (Demon) {
        LoadTexture("assets/demon_head.png"),
        demonStartPos,
        Vector2Zero(),
        0.0,
        Vector2Zero(),
        rHand, lHand,
        0.0, 0.0, 0.0
    };
}

void updateDemon(Demon *demon, float delta) {
    if (demon->height == 0) demon->vel = Vector2Scale(demon->vel, 0.75);
    if (Vector2Length(demon->vel) > 5) {
        demon->vel = Vector2ToLength(demon->vel, 5);
    }
    demon->pos = Vector2Add(demon->pos, demon->vel);
    demon->rot = Vector2Angle(demon->targetPos, demon->pos) + 90;
    updateHands(demon, delta);

    bool wasInAir = demon->height > 0;
    demon->height += demon->zVel;
    bool inAir = demon->height > 0;

    if (wasInAir && !inAir) {
        demon->trauma += 0.5;
    }

    if (inAir) {
        demon->zVel -= 0.5;
    } else {
        demon->height = 0;
        demon->zVel = 0;
    }
    if (demon->trauma > 1) demon->trauma = 1;
}

void updateHands(Demon *demon, float delta) {
    float waving = sin(GetTime() * 4);
    Vector2 rHandOffset = (Vector2){-X_OFFSET + waving * 3,
                                    Y_OFFSET + waving * 2};
    Vector2 lHandOffset = (Vector2){X_OFFSET - waving * 3,
                                    Y_OFFSET + waving * 2};
    moveHand(&demon->rHand, demon->pos, rHandOffset, demon->rot);
    moveHand(&demon->lHand, demon->pos, lHandOffset, demon->rot);
}

void moveHand(Hand *hand, Vector2 basePos, Vector2 neutralOffset, float rot) {
    Vector2 toTarget;

    if (!hand->flying) {
        Vector2 handOffset = Vector2Rotate(neutralOffset, DEG2RAD * rot);
        Vector2 targetPos = Vector2Add(basePos, handOffset);
        MoveTowardsPoint(&hand->pos, targetPos, hand->speed);
        float waving = sin(GetTime() * 4);
        hand->rot = rot - INITIAL_ROT_OFFSET + waving * 2;
    } else {
        hand->speed += hand->speed / 10;
        MoveTowardsPoint(&hand->pos,
                         hand->targetPos,
                         hand->speed);
        Vector2 toTarget = Vector2Subtract(hand->targetPos,
                                           hand->pos);
        if (Vector2Length(toTarget) < 2) {
            hand->flying = false;
            hand->speed = NEUTRAL_HAND_SPEED;
        }
    }
}

void updateImp(EnemyShooter *enemy, World *world) {
    enemy->pos = Vector2Add(enemy->pos, enemy->vel);
    enemy->rot = Vector2Angle(enemy->pos, world->demon->pos) - 90;
}

void updateEnemies(World *world) {
    for (int i = 0; i < MAX_ENEMIES; i ++) {
        Enemy *e = world->enemies[i];
        if (e != 0) {
            switch (e->type) {
                case ENEMY_IMP:
                    updateImp((EnemyShooter *)e, world);
                    break;
            }
        }
    }
}

void drawEnemies(World *world) {
    for (int i = 0; i < MAX_ENEMIES; i ++) {
        Enemy *e = world->enemies[i];
        if (e != 0) {
            drawEntity((Entity *)e);
        }
    }
}

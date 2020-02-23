#include <stdio.h>

#include "entities.h"
#include "raymath.h"
#include "mathUtils.h"
#include "constants.h"
#include "drawing.h"

void moveHand(Hand *hand, Vector2 basePos, Vector2 neutralOffset, float rot, float delta);
void updateHands(Demon *demon, float delta); 

const float HAND_X_OFFSET = 65;
const float HAND_Y_OFFSET = 15;

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
        .texture = LoadTexture("assets/demon_head.png"),
        .pos = demonStartPos,
        .vel = Vector2Zero(),
        .rot = 0.0,
        .targetPos = Vector2Zero(),
        .rHand = rHand, .lHand = lHand,
        .height = 0.0, .zVel = 0.0, .trauma = 0.0
    };
}

void updateDemon(World *world, float delta) {
    Demon *demon = world->demon;
    if (demon->height == 0) demon->vel = Vector2Scale(demon->vel, 0.75);
    if (Vector2Length(demon->vel) > 5) {
        demon->vel = Vector2ToLength(demon->vel, 5);
    }
    Vector2 v = Vector2Scale(demon->vel, delta);
    Circle hb = (Circle){demon->pos, 20};
    for (int i = 0; i < world->map->numObstacles; i++) {
        v = moveWithoutHitting(hb, v, world->map->obstacles[i]);
    }
    demon->pos = Vector2Add(demon->pos, v);
    demon->rot = Vector2Angle(demon->targetPos, demon->pos) + 90;
    updateHands(demon, delta);

    bool wasInAir = demon->height > 0;
    demon->height += demon->zVel * delta;
    bool inAir = demon->height > 0;

    if (wasInAir && !inAir) {
        demon->trauma += 0.4;
    }

    if (inAir) {
        demon->zVel -= 0.5 * delta;
    } else {
        demon->height = 0;
        demon->zVel = 0;
    }
    if (demon->trauma > 1) demon->trauma = 1;
    if (demon->trauma > 0) demon->trauma -= 0.01;
}

void updateHands(Demon *demon, float delta) {
    float waving = sin(GetTime() * 4);
    Vector2 rHandOffset = (Vector2){-HAND_X_OFFSET + waving * 3,
                                    HAND_Y_OFFSET + waving * 2};
    Vector2 lHandOffset = (Vector2){HAND_X_OFFSET - waving * 3,
                                    HAND_Y_OFFSET + waving * 2};
    moveHand(&demon->rHand, demon->pos, rHandOffset, demon->rot, delta);
    moveHand(&demon->lHand, demon->pos, lHandOffset, demon->rot, delta);
}

void moveHand(Hand *hand, Vector2 basePos, Vector2 neutralOffset, float rot, float delta) {
    Vector2 toTarget;

    if (!hand->flying) {
        Vector2 handOffset = Vector2Rotate(neutralOffset, DEG2RAD * rot);
        Vector2 targetPos = Vector2Add(basePos, handOffset);
        moveTowardsPoint(&hand->pos, targetPos, hand->speed * delta);
        float waving = sin(GetTime() * 4);
        hand->rot = rot - INITIAL_ROT_OFFSET + waving * 2;
    } else {
        hand->speed += hand->speed / 10;
        moveTowardsPoint(&hand->pos,
                         hand->targetPos,
                         hand->speed * delta);
        Vector2 toTarget = Vector2Subtract(hand->targetPos,
                                           hand->pos);
        if (Vector2Length(toTarget) < 2) {
            hand->flying = false;
            hand->speed = NEUTRAL_HAND_SPEED;
        }
    }
}

void setHandFlying(Hand *hand, Vector2 target) {
    hand->flying = true;
    hand->targetPos = target;
    hand->speed = 3;
    hand->rot = Vector2Angle(hand->pos,
                             hand->targetPos) - 90;
}

void updateImp(Enemy *enemy, World *world) {
    enemy->pos = Vector2Add(enemy->pos, enemy->vel);
    enemy->rot = Vector2Angle(enemy->pos, world->demon->pos) - 90;
}

void updateEnemies(World *world) {
    for (int i = 0; i < MAX_ENEMIES; i ++) {
        Enemy *e = world->enemies[i];
        if (e != 0) {
            e->update(e, world);
        }
    }
}

void drawEnemies(World *world, Camera2D camera) {
    for (int i = 0; i < MAX_ENEMIES; i ++) {
        Enemy *e = world->enemies[i];
        if (e != 0) {
            drawEntity((Entity *)e, camera);
        }
    }
}

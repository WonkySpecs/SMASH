#include <stdio.h>

#include "entities.h"
#include "raymath.h"
#include "mathUtils.h"
#include "constants.h"
#include "drawing.h"
#include "world.h"

void moveHand(Hand *hand, Vector2 basePos, Vector2 neutralOffset, float rot, float delta);
void updateHands(Demon *demon, float delta); 
Hitbox worldHitbox(Entity e);

const float HAND_X_OFFSET = 65;
const float HAND_Y_OFFSET = 15;

void initDemon(Demon *demon) {
    Vector2 startPos = (Vector2){W_WIDTH / 2, W_HEIGHT / 2};
    Image hand = LoadImage("assets/demon_hand.png");
    Hand rHand = {
        .texture = LoadTextureFromImage(hand),
        .pos = startPos,
        .vel = Vector2Zero(),
        .rot = 0,
        .speed = NEUTRAL_HAND_SPEED,
        .targetPos = Vector2Zero(),
        .state = NEUTRAL,
    };
    ImageFlipHorizontal(&hand);
    Hand lHand = {
        .texture = LoadTextureFromImage(hand),
        .pos = startPos,
        .vel = Vector2Zero(),
        .rot = 0,
        .speed = NEUTRAL_HAND_SPEED,
        .targetPos = Vector2Zero(),
        .state = NEUTRAL,
    };
    UnloadImage(hand);

    demon->texture = getTexture(TEX_DEMON_HEAD);
    demon->pos = startPos;
    demon->vel = Vector2Zero();
    demon->rot = 0.0;
    demon->targetPos = Vector2Zero();
    demon->rHand = rHand;
    demon->lHand = lHand;
    demon->height = 0.0;
    demon->zVel = 0.0;
    demon->trauma = 0.0;
    demon-> offsetHitbox = (Hitbox) {.circle = (Circle){Vector2Zero(), 15}, CIRCLE};
}

void updateDemon(World *world, float delta) {
    Demon *demon = world->demon;
    if (demon->height == 0) demon->vel = Vector2Scale(demon->vel, 0.75);
    if (Vector2Length(demon->vel) > 5) {
        demon->vel = Vector2ToLength(demon->vel, 5);
    }
    Vector2 v = Vector2Scale(demon->vel, delta);
    for (int i = 0; i < world->map->numObstacles; i++) {
        v = moveWithoutHitting(worldHitbox(*((Entity*)demon)).circle,
                               v,
                               world->map->obstacles[i]);
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
    switch (hand->state) {
        case NEUTRAL:
        case RECOVERING: {
            Vector2 handOffset = Vector2Rotate(neutralOffset, DEG2RAD * rot);
            Vector2 targetPos = Vector2Add(basePos, handOffset);
            moveTowardsPoint(&hand->pos, targetPos, hand->speed * delta);
            if (Vector2Distance(hand->pos, targetPos) < 50) {
                hand->state = NEUTRAL;
            }
            float waving = sin(GetTime() * 4);
            hand->rot = rot - INITIAL_ROT_OFFSET + waving * 2;
            break;
        }
        case ATTACKING: {
            hand->speed += hand->speed / 10;
            moveTowardsPoint(&hand->pos,
                             hand->targetPos,
                             hand->speed * delta);
            if (Vector2Distance(hand->targetPos, hand->pos) < 2) {
                hand->state = RECOVERING;
                hand->speed = NEUTRAL_HAND_SPEED;
            }
            break;
        }
    }

    if (isnan(hand->pos.x) || isnan(hand->pos.y)) {
        printf("Correcting that nan bug\n");
        hand->pos = basePos;
    }
}

void setHandFlying(Hand *hand, Vector2 target) {
    hand->state = ATTACKING;
    hand->targetPos = target;
    hand->speed = 3;
    hand->rot = Vector2Angle(hand->pos,
                             hand->targetPos) - 90;
}

bool primTimeStateTransition(Enemy *enemy, float timerThresh, EntityState nextState) {
    if (enemy->primTimer > timerThresh) {
        enemy->state = nextState;
        enemy->primTimer = 0;
        return true;
    }
    return false;
}

Vector2 calcImpTargetPos(Vector2 curPos, Vector2 demonPos) {
    Vector2 diff = Vector2ToLength(
                        Vector2Rotate(Vector2Subtract(curPos,
                                                      demonPos),
                                      randFloat() - 0.5),
                        400 + randFloat() * 100 - 50);
    return (Vector2) {
        diff.x + demonPos.x,
        diff.y + demonPos.y
    };
}

/*
    Imp uses primTimer/primThresh as:
        in NEUTRAL -> time since last attack / time required until next shot.
        in PRERPARING -> time since started preparing / time to charge attack.
        in RECOVERING -> time since attack was fired / time required to recover.
*/
void updateImp(Enemy *imp, World *world, float delta) {

    imp->primTimer += delta;

    if (imp->state == NEUTRAL) {
        if (!(imp->targetPos.x)) {
            imp->targetPos = calcImpTargetPos(imp->pos, world->demon->pos);
        }
        moveTowardsPoint(&imp->pos, imp->targetPos, imp->maxSpeed);
        if (Vector2Distance(imp->pos, imp->targetPos) < 10) {
            primTimeStateTransition(imp, imp->primThresh, PREPARING);
        }
    }

    if (imp->state == NEUTRAL || imp->state == PREPARING) {
        imp->rot = Vector2Angle(imp->pos, world->demon->pos) - 90;
    }

    if (imp->state == PREPARING) {
        if(primTimeStateTransition(imp, imp->primThresh / 2.0, RECOVERING)) {
            addProjectile(world,
                          (Projectile){.pos = imp->pos,
                                       .vel = Vector2Rotate((Vector2){3, 0},
                                                            (imp->rot + 90) * DEG2RAD),
                                       .active = true,
                                       .offsetHitbox = (Hitbox) {
                                           (Circle) { Vector2Zero(), 5}, CIRCLE
                                       }});
        }
    }

    if (imp->state == RECOVERING) {
        if (primTimeStateTransition(imp, imp->primThresh / 2.0, NEUTRAL)) {
            imp->targetPos = calcImpTargetPos(imp->pos, world->demon->pos);
        }
    }
}

void updateProjectiles(World *world, float delta) {
    for (int i = 0; i < world->projAllocated; i ++) {
        Projectile *p = &world->projectiles[i];
        if (p && p->active) {
            p->pos = Vector2Add(p->pos, Vector2Scale(p->vel, delta));
            if (didEntitiesCollide((Entity*)p, (Entity*)world->demon)) {
                puts("HIT");
                p->active = false;
                continue;
            }
            if (isOffMap(p->pos, *world->map)) {
                p->active = false;
            }
        }
    }
}

void updateEnemies(World *world, float delta) {
    for (int i = 0; i < world->enemiesAllocated; i ++) {
        Enemy *e = &world->enemies[i];
        if (e && e->active) {
            e->update(e, world, delta);
        }
    }
}

void drawEnemiesAndProj(World *world, Camera2D camera) {
    for (int i = 0; i < world->enemiesAllocated; i ++) {
        Enemy *e = &world->enemies[i];
        if (e && e->active) {
            drawEntity((Entity *)e, camera);
        }
    }
    for (int i = 0; i < world->projAllocated; i ++) {
        Projectile *p = &world->projectiles[i];
        if (p && p->active) {
            DrawCircleV(GetWorldToScreen2D(p->pos, camera), p->offsetHitbox.circle.r, BLUE);
            // drawEntity((Entity *)p, camera);
        }
    }
}

bool didEntitiesCollide(Entity *e1, Entity *e2) {
    Hitbox hb1 = worldHitbox(*e1);
    Hitbox hb2 = worldHitbox(*e2);

    if (hb1.type == CIRCLE) {
        if (e2->offsetHitbox.type == CIRCLE) {
            return circlesColliding(hb1.circle, hb2.circle);
        } else {
            return rectCircleColliding(hb2.rect, hb1.circle);
        }
    } else {
        if (e2->offsetHitbox.type == CIRCLE) {
            return rectCircleColliding(hb1.rect, hb2.circle);
        } else {
            return CheckCollisionRecs(hb1.rect, hb2.rect);
        }
    }
}

Hitbox worldHitbox(Entity e) {
    if (e.offsetHitbox.type == CIRCLE) {
        Circle hb = e.offsetHitbox.circle;
        return (Hitbox) { .circle = {Vector2Add(hb.c, e.pos), hb.r}, CIRCLE };
    } else {
        Rectangle hb = e.offsetHitbox.rect;
        return (Hitbox) { .rect = { hb.x + e.pos.x, hb.y + e.pos.y,
                                    hb.width, hb.height }, RECT };
    }
}

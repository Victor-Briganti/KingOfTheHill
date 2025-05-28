#include "enemy/pawn.h"
#include "player/player.h"
#include "sprites.h"

#define MAX_SPRITES_ANIM (7)

static const SpriteDefinition *const sprites[MAX_SPRITES_ANIM] = {
    &pawn_sprite1, &pawn_sprite2, &pawn_sprite3, &pawn_sprite4,
    &pawn_sprite5, &pawn_sprite6, &pawn_sprite7,
};

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

inline static s8 startMovement(Enemy *enemy) {
    s16 x = enemy->actor.collisionCurPos.x;
    s16 y = clamp(enemy->actor.collisionCurPos.y + 2, 0, mapLevelHeight - 2);
    if (map[y][x] != COLLISION_TYPE_EMPTY) {
        return 0;
    }

    // Player on diagonal right
    if (map[y][clamp(x + 2, 0, mapLevelHeight - 2)] != 0 &&
        (map[y][clamp(x + 2, 0, mapLevelHeight - 2)] & ~COLLISION_TYPE_PLAYER) ==
        0)
        x = clamp(x + 2, 0, mapLevelHeight - 2);

    // Player on diagonal left
    if (map[y][clamp(x - 2, 0, mapLevelHeight - 2)] != 0 &&
        (map[y][clamp(x - 2, 0, mapLevelHeight - 2)] & ~COLLISION_TYPE_PLAYER) ==
        0)
        x = clamp(x - 2, 0, mapLevelHeight - 2);

    ACTOR_setTargetAnimPos(&enemy->actor, x, y);
    enemy->state = ENEMY_MOVING;

    return 1;
}

inline static s8 moveAnimation(Enemy *enemy) {
    if (frame % FRAME_ANIMATION == 0) {
        ACTOR_animateTo(&enemy->actor);

        if (!enemy->actor.moving) {
            if (ACTOR_checkCollision(&enemy->actor)) {
                sceneManager[sceneIndex]->hit(enemy->actor.collisionCurPos);
                return 0;
            }

            if (enemy->actor.collisionCurPos.y == mapLevelHeight - 2) {
                enemy->state = ENEMY_ANIMATING;
                return 1;
            }

            enemy->state = ENEMY_IDLE;
            return 0;
        }
    }

    return 1;
}

inline static s8 promotionAnimation(Enemy *enemy) {
    if (frame % FRAME_ANIMATION == 0) {
        if (enemy->indexSprite >= MAX_SPRITES_ANIM) {
            const Vect2D_s16 pos = enemy->actor.collisionCurPos;
            enemy->destroy(enemy);
            ENEMY_init(enemy, QUEEN_TYPE, pos.x, pos.y);
            return 0;
        }

        ACTOR_deallocSprite(&enemy->actor);
        ACTOR_init(&enemy->actor, sprites[enemy->indexSprite], ENEMY_PAL,
                   enemy->actor.collisionCurPos.x, enemy->actor.collisionCurPos.y,
                   COLLISION_TYPE_PAWN);
        enemy->indexSprite++;
    }

    return 1;
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

s8 PAWN_update(Enemy *enemy) {
    switch (enemy->state) {
        case ENEMY_IDLE:
            return startMovement(enemy);
        case ENEMY_MOVING:
            return moveAnimation(enemy);
        case ENEMY_ANIMATING:
            return promotionAnimation(enemy);
        default:
            return 0;
    }
}

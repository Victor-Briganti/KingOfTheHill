#include "enemy/pawn2.h"
#include "player/player.h"

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
    if ((map[y][clamp(x + 2, 0, mapLevelHeight - 2)] != 0) &&
        (map[y][clamp(x + 2, 0, mapLevelHeight - 2)] & ~COLLISION_TYPE_PLAYER) ==
        0)
        x = clamp(x + 2, 0, mapLevelHeight - 2);

    // Player on diagonal left
    if ((map[y][clamp(x - 2, 0, mapLevelHeight - 2)] != 0) &&
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
                kprintf("Player hit");
                player.health--;
                player.state = PLAYER_DEAD;
                return 0;
            }

            // if (enemy->actor.collisionCurPos.y == mapLevelHeight - 2) {
            //     enemy->state = PAWN_PROMOTION;
            //     return 1;
            // }

            enemy->state = ENEMY_IDLE;
            return 0;
        }
    }

    return 1;
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

s8 PAWN_update2(Enemy *enemy) {
    switch (enemy->state) {
        case ENEMY_IDLE:
            return startMovement(enemy);
        case ENEMY_MOVING:
            return moveAnimation(enemy);
        default:
            return 0;
    }
}

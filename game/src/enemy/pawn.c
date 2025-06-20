#include "enemy/pawn.h"
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
  const s16 y =
      clamp(enemy->actor.collisionCurPos.y + 2, 0, mapLevelHeight - 2);
  if (map[y][x] != COLLISION_TYPE_EMPTY) {
    return 0;
  }

  ACTOR_setTargetAnimPos(&enemy->actor, x, y);
  enemy->state = ENEMY_MOVING;
  XGM_startPlayPCM(ID_MOVE_SOUND, PRIO_MOVE_SOUND, CHN_MOVE_SOUND);

  return 1;
}

inline static s8 tryAttack(Enemy *enemy) {
  s16 x = enemy->actor.collisionCurPos.x;
  const s16 y =
      clamp(enemy->actor.collisionCurPos.y + 2, 0, mapLevelHeight - 2);
  if (map[y][x] != COLLISION_TYPE_EMPTY) {
    return 0;
  }

  bool enemyAttacked = 0;

  // Player on diagonal right
  if (map[y][clamp(x + 2, 0, mapLevelHeight - 2)] != 0 &&
      (map[y][clamp(x + 2, 0, mapLevelHeight - 2)] & ~COLLISION_TYPE_PLAYER) ==
          0) {
    enemyAttacked = 1;
    x = clamp(x + 2, 0, mapLevelHeight - 2);
  }

  // Player on diagonal left
  if (map[y][clamp(x - 2, 0, mapLevelHeight - 2)] != 0 &&
      (map[y][clamp(x - 2, 0, mapLevelHeight - 2)] & ~COLLISION_TYPE_PLAYER) ==
          0) {
    enemyAttacked = 1;
    x = clamp(x - 2, 0, mapLevelHeight - 2);
  }

  if (enemyAttacked) {
    ACTOR_setTargetAnimPos(&enemy->actor, x, y);
    enemy->state = ENEMY_MOVING;
    XGM_startPlayPCM(ID_MOVE_SOUND, PRIO_MOVE_SOUND, CHN_MOVE_SOUND);
    return 1;
  }
  
  enemy->state = ENEMY_IDLE;
  return 0;
}

inline static s8 moveAnimation(Enemy *enemy) {
  if (frame % (FRAME_ANIMATION / 2) == 0) {
    ACTOR_animateTo(&enemy->actor);

    if (!enemy->actor.moving) {
      MAP_updateCollision(enemy->actor.collisionPrevPos,
                          enemy->actor.collisionCurPos,
                          enemy->actor.collisionType);

      if (ACTOR_checkCollision(&enemy->actor)) {
        XGM_startPlayPCM(ID_DEATH_SOUND, PRIO_DEATH_SOUND, CHN_DEATH_SOUND);
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
      MAP_updateCollision(enemy->actor.collisionPrevPos,
                          enemy->actor.collisionCurPos,
                          enemy->actor.collisionType);

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
  case ENEMY_ATTACKING:
    return tryAttack(enemy);
  case ENEMY_MOVING:
    return moveAnimation(enemy);
  case ENEMY_ANIMATING:
    return promotionAnimation(enemy);
  default:
    return 0;
  }
}

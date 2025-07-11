#include "enemy/knight.h"
#include "map/map.h"
#include "player/player.h"
#include "types/collision.h"

typedef Vect2D_s16 (*MovementCheck)(const Vect2D_s16, const Vect2D_s16);

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

inline static s8 startMovement(Enemy *enemy) {
  const Vect2D_s16 from = enemy->actor.collisionCurPos;
  const Vect2D_s16 to = player.actor.collisionCurPos;

  const Vect2D_s16 movements[8] = {
      {from.x - 2, from.y - 4}, {from.x + 2, from.y - 4},
      {from.x - 2, from.y + 4}, {from.x + 2, from.y + 4},
      {from.x + 4, from.y + 2}, {from.x + 4, from.y - 2},
      {from.x - 4, from.y + 2}, {from.x - 4, from.y - 2},
  };

  u32 bestDist = MAX_U32;
  Vect2D_s16 bestMovement = {-1, -1};

  for (u8 i = 0; i < 8; i++) {
    if (movements[i].x < 0 || movements[i].y < 0 ||
        MAP_getCollision(movements[i]) != COLLISION_TYPE_EMPTY)
      continue;

    const s32 dx = to.x - movements[i].x;
    const s32 dy = to.y - movements[i].y;
    const u32 dist = getApproximatedDistance(dx, dy);
    if (dist < bestDist) {
      bestDist = dist;
      bestMovement = movements[i];
    }
  }

  if (bestMovement.x >= 0 && bestMovement.y >= 0) {
    ACTOR_setTargetAnimPos(&enemy->actor, bestMovement.x, bestMovement.y);
    enemy->state = ENEMY_MOVING;
    XGM_startPlayPCM(ID_MOVE_SOUND, PRIO_MOVE_SOUND, CHN_MOVE_SOUND);
    return 1;
  }

  return -1;
}

inline static s8 startAttack(Enemy *enemy) {
  const Vect2D_s16 from = enemy->actor.collisionCurPos;
  const Vect2D_s16 movements[8] = {
      {from.x - 2, from.y - 4}, {from.x + 2, from.y - 4},
      {from.x - 2, from.y + 4}, {from.x + 2, from.y + 4},
      {from.x + 4, from.y + 2}, {from.x + 4, from.y - 2},
      {from.x - 4, from.y + 2}, {from.x - 4, from.y - 2},
  };

  for (u8 i = 0; i < 8; i++) {
    if (movements[i].x < 0 || movements[i].y < 0)
      continue;

    if (MAP_getCollision(movements[i]) == COLLISION_TYPE_PLAYER) {
      ACTOR_setTargetAnimPos(&enemy->actor, movements[i].x, movements[i].y);
      enemy->state = ENEMY_MOVING;
      XGM_startPlayPCM(ID_MOVE_SOUND, PRIO_MOVE_SOUND, CHN_MOVE_SOUND);
      return 1;
    }
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
        sceneManager[sceneIndex]->hit(enemy->actor.collisionCurPos);
        XGM_startPlayPCM(ID_DEATH_SOUND, PRIO_DEATH_SOUND, CHN_DEATH_SOUND);
        return 0;
      }

      enemy->state = ENEMY_IDLE;
      return 0;
    }
  }

  return 1;
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

s8 KNIGHT_update(Enemy *enemy) {
  switch (enemy->state) {
  case ENEMY_IDLE:
    return startMovement(enemy);
  case ENEMY_ATTACKING:
    return startAttack(enemy);
  case ENEMY_MOVING:
    return moveAnimation(enemy);
  default:
    return 0;
  }
}

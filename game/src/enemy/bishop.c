#include "enemy/bishop.h"
#include "enemy/enemy.h"
#include "map/map.h"
#include "maths.h"
#include "player/player.h"
#include "types.h"

#define DIAGONAL_NUM 4

typedef Vect2D_s16 (*MovementCheck)(const Vect2D_s16, const Vect2D_s16);

const s16 diagonals[DIAGONAL_NUM][2] = {{-2, -2}, {2, -2}, {-2, 2}, {2, 2}};

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

// Return TRUE if the path is empty, FALSE otherwise
inline static bool verifyMovement(const Vect2D_s16 from, const Vect2D_s16 to,
                                  const MovementCheck check) {
  const Vect2D_s16 res = check(from, to);
  if (res.x < 0)
    return TRUE;

  return FALSE;
}

static s8 tryAttack(Enemy *enemy, const Vect2D_s16 from, const Vect2D_s16 to) {
  // Diagonal attack
  if (abs(from.y - to.y) == abs(from.x - to.x)) {
    const s16 x = to.x > from.x ? to.x - 1 : to.x + 1;
    const s16 y = to.y > from.y ? to.y - 1 : to.y + 1;
    if (verifyMovement(from, (Vect2D_s16){x, y}, MAP_checkDiagonal)) {
      ACTOR_setTargetAnimPos(&enemy->actor, to.x, to.y);
      enemy->state = ENEMY_MOVING;
      XGM_startPlayPCM(ID_MOVE_SOUND, PRIO_MOVE_SOUND, CHN_MOVE_SOUND);
      return 1;
    }
  }

  return 0;
}

static s8 tryMovement(Enemy *enemy, const Vect2D_s16 from, s16 x, s16 y) {
  // Move closer to the player
  if (x != from.x && y != from.y &&
      verifyMovement(from, (Vect2D_s16){x, y}, MAP_checkDiagonal)) {
    ACTOR_setTargetAnimPos(&enemy->actor, x, y);
    enemy->state = ENEMY_MOVING;
    XGM_startPlayPCM(ID_MOVE_SOUND, PRIO_MOVE_SOUND, CHN_MOVE_SOUND);
    return 1;
  }

  return 0;
}

inline static s8 startMovement(Enemy *enemy) {
  const Vect2D_s16 from = enemy->actor.collisionCurPos;
  const Vect2D_s16 to = player.actor.collisionCurPos;

  u32 bestDist = MAX_U32;
  s32 bestX = from.x;
  s32 bestY = from.y;

  for (u8 i = 0; i < DIAGONAL_NUM; i++) {
    const s32 x = clamp(from.x + diagonals[i][0], 0, mapLevelWidth - 2);
    const s32 y = clamp(from.y + diagonals[i][1], 0, mapLevelHeight - 2);

    if (verifyMovement(from, (Vect2D_s16){x, y}, MAP_checkDiagonal)) {
      const s32 dx = to.x - x;
      const s32 dy = to.y - y;
      const u32 dist = getApproximatedDistance(dx, dy);

      if (dist < bestDist) {
        bestDist = dist;
        bestX = x;
        bestY = y;
      }
    }
  }

  if (bestX != from.x || bestY != from.y)
    return tryMovement(enemy, from, bestX, bestY);

  return -1;
}

inline static s8 startAttack(Enemy *enemy) {
  const Vect2D_s16 from = enemy->actor.collisionCurPos;
  const Vect2D_s16 to = player.actor.collisionCurPos;
  const s8 value = tryAttack(enemy, from, to);

  if (!value)
    enemy->state = ENEMY_IDLE;

  return value;
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

s8 BISHOP_update(Enemy *enemy) {
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

#include "enemy/tower.h"
#include "player/player.h"

typedef Vect2D_s16 (*MovementCheck)(const Vect2D_s16, const Vect2D_s16);

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
  // Vertical attack
  if (from.x == to.x) {
    const s16 y = to.y > from.y ? to.y - 1 : to.y + 1;
    if (verifyMovement(from, (Vect2D_s16){from.x, y}, MAP_checkVertical)) {
      ACTOR_setTargetAnimPos(&enemy->actor, from.x, to.y);
      enemy->state = ENEMY_MOVING;
      XGM_startPlayPCM(ID_MOVE_SOUND, PRIO_MOVE_SOUND, CHN_MOVE_SOUND);
      return 1;
    }
  }

  // Horizontal attack
  if (from.y == to.y) {
    const s16 x = to.x > from.x ? to.x - 1 : to.x + 1;
    if (verifyMovement(from, (Vect2D_s16){x, from.y}, MAP_checkHorizontal)) {
      ACTOR_setTargetAnimPos(&enemy->actor, to.x, from.y);
      enemy->state = ENEMY_MOVING;
      XGM_startPlayPCM(ID_MOVE_SOUND, PRIO_MOVE_SOUND, CHN_MOVE_SOUND);
      return 1;
    }
  }

  return 0;
}

static s8 tryMovement(Enemy *enemy, const Vect2D_s16 from, s16 x, s16 y) {
  // Move closer to the player
  if (x == from.x &&
      verifyMovement(from, (Vect2D_s16){x, y}, MAP_checkVertical)) {
    ACTOR_setTargetAnimPos(&enemy->actor, x, y);
    enemy->state = ENEMY_MOVING;
      XGM_startPlayPCM(ID_MOVE_SOUND, PRIO_MOVE_SOUND, CHN_MOVE_SOUND);
    return 1;
  }

  if (y == from.y &&
      verifyMovement(from, (Vect2D_s16){x, y}, MAP_checkHorizontal)) {
    ACTOR_setTargetAnimPos(&enemy->actor, x, y);
    enemy->state = ENEMY_MOVING;
      XGM_startPlayPCM(ID_MOVE_SOUND, PRIO_MOVE_SOUND, CHN_MOVE_SOUND);
    return 1;
  }

  return 0;
}

inline static s8 startMovement(Enemy *enemy) {
  const Vect2D_s16 from = {enemy->actor.collisionCurPos.x,
                           enemy->actor.collisionCurPos.y};
  const Vect2D_s16 to = {player.actor.collisionCurPos.x,
                         player.actor.collisionCurPos.y};

  // Try to move vertically
  if (to.y != from.y) {
    const s16 dy = to.y > from.y ? 2 : -2;
    const s16 newY = clamp(from.y + dy, 0, mapLevelHeight - 2);
    if (tryMovement(enemy, from, from.x, newY))
      return 1;
  }

  // Try to move horizontally
  if (to.x != from.x) {
    const s16 dx = to.x > from.x ? 2 : -2;
    const s16 newX = clamp(from.x + dx, 0, mapLevelWidth - 2);
    if (tryMovement(enemy, from, newX, from.y))
      return 1;
  }

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
        XGM_startPlayPCM(ID_DEATH_SOUND, PRIO_DEATH_SOUND, CHN_DEATH_SOUND);
        sceneManager[sceneIndex]->hit(enemy->actor.collisionCurPos);
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

s8 TOWER_update(Enemy *enemy) {
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

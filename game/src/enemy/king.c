#include "enemy/king.h"
#include "enemy/enemy.h"
#include "maths.h"
#include "player/player.h"
#include "scene/scene15.h"
#include "scene/scene_manager.h"
#include "sprites.h"

#define MAX_SPRITES_ANIM (2)
#define ANIMATION_TIME (15)

static const SpriteDefinition *const sprites[MAX_SPRITES_ANIM] = {
    &king_sprite1, &king_sprite2};

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

static s8 tryMovement(Enemy *enemy, const Vect2D_s16 from, s16 x, s16 y) {
  // Move away from the player
  if (x == from.x &&
      verifyMovement(from, (Vect2D_s16){x, y}, MAP_checkVertical)) {
    ACTOR_setTargetAnimPos(&enemy->actor, x, y);
    enemy->state = ENEMY_MOVING;
    return 1;
  }

  if (y == from.y &&
      verifyMovement(from, (Vect2D_s16){x, y}, MAP_checkHorizontal)) {
    ACTOR_setTargetAnimPos(&enemy->actor, x, y);
    enemy->state = ENEMY_MOVING;
    return 1;
  }

  if (x != from.x && y != from.y &&
      verifyMovement(from, (Vect2D_s16){x, y}, MAP_checkDiagonal)) {
    ACTOR_setTargetAnimPos(&enemy->actor, x, y);
    enemy->state = ENEMY_MOVING;
    return 1;
  }

  return 0;
}

inline static s8 startMovement(Enemy *enemy) {
  const Vect2D_s16 from = enemy->actor.collisionCurPos;
  const Vect2D_s16 to = player.actor.collisionCurPos;

  const Vect2D_s16 movements[8] = {
      {from.x, clamp(from.y - 2, 0, mapLevelHeight - 2)},
      {clamp(from.x - 2, 0, mapLevelWidth - 2),
       clamp(from.y - 2, 0, mapLevelHeight - 2)},
      {clamp(from.x + 2, 0, mapLevelWidth - 2),
       clamp(from.y - 2, 0, mapLevelHeight - 2)},
      {clamp(from.x - 2, 0, mapLevelWidth - 2), from.y},
      {clamp(from.x + 2, 0, mapLevelWidth - 2), from.y},
      {from.x, clamp(from.y + 2, 0, mapLevelHeight - 2)},
      {clamp(from.x - 2, 0, mapLevelWidth - 2),
       clamp(from.y + 2, 0, mapLevelHeight - 2)},
      {clamp(from.x + 2, 0, mapLevelWidth - 2),
       clamp(from.y + 2, 0, mapLevelHeight - 2)},
  };

  u32 bestDist = 0;
  u8 index = -1;

  for (u8 i = 0; i < 8; i++) {
    if (movements[i].x < 0 || movements[i].y < 0)
      continue;

    const s32 dx = to.x - movements[i].x;
    const s32 dy = to.y - movements[i].y;
    const u32 dist = getApproximatedDistance(dx, dy);
    if (dist > bestDist) {
      bestDist = dist;
      index = i;
    }
  }

  if (index >= 0 &&
      tryMovement(enemy, from, movements[index].x, movements[index].y))
    return 1;

  return -1;
}

inline static s8 tryAttack(Enemy *enemy) {
  static u8 tries = 0;
  static u16 count = 0;
  s8 res = 1;

  if (tries != 3) {
    tries++;
    return -1;
  }

  if (frame % FRAME_ANIMATION == 0) {
    if (count == ANIMATION_TIME) {
      count = 0;
      res = 0;
      enemy->indexSprite = 0;
      enemy->state = ENEMY_IDLE;
      tries = 0;
      SCENE15_resurrectEnemy();
    } else
      enemy->indexSprite = (enemy->indexSprite + 1) % MAX_SPRITES_ANIM;

    ACTOR_deallocSprite(&enemy->actor);
    ACTOR_init(&enemy->actor, sprites[enemy->indexSprite], ENEMY_PAL,
               enemy->actor.collisionCurPos.x, enemy->actor.collisionCurPos.y,
               COLLISION_TYPE_KING);
    count++;
  }

  return res;
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

s8 KING_update(Enemy *enemy) {
  switch (enemy->state) {
  case ENEMY_IDLE:
    return startMovement(enemy);
  case ENEMY_ATTACKING:
    return tryAttack(enemy);
  case ENEMY_MOVING:
    return moveAnimation(enemy);
  default:
    return 0;
  }
}

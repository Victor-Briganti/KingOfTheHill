#include "enemy/queen.h"
#include "map/map.h"
#include "node/actor.h"
#include "player/player.h"
#include "scene/scene_manager.h"

#include <genesis.h>

typedef Vect2D_s16 (*MovementCheck)(const Vect2D_s16, const Vect2D_s16);

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

// Return TRUE if the path is empty, FALSE otherwise
inline static bool verifyMovement(Vect2D_s16 from, Vect2D_s16 to,
                                  MovementCheck check) {
  const Vect2D_s16 res = check(from, to);
  if (res.x < 0)
    return TRUE;

  return FALSE;
}

inline static s8 startMovement(Queen *queen) {
  const Vect2D_s16 from = {queen->actor.collisionCurPos.x,
                           queen->actor.collisionCurPos.y};
  const Vect2D_s16 to = {player.actor.collisionCurPos.x,
                         player.actor.collisionCurPos.y};

  // Vertical attack
  if (from.x == to.x) {
    const s16 y = to.y > from.y ? to.y - 1 : to.y + 1;
    if (verifyMovement(from, (Vect2D_s16){from.x, y}, MAP_checkVertical)) {
      ACTOR_setTargetAnimPos(&queen->actor, from.x, to.y);
      queen->state = QUEEN_MOVING;
      return 1;
    }
  }

  // Horizontal attack
  if (from.y == to.y) {
    const s16 x = to.x > from.x ? to.x - 1 : to.x + 1;
    if (verifyMovement(from, (Vect2D_s16){x, from.y}, MAP_checkHorizontal)) {
      ACTOR_setTargetAnimPos(&queen->actor, to.x, from.y);
      queen->state = QUEEN_MOVING;
      return 1;
    }
  }

  // Diagonal attack
  if (abs(from.y - to.y) == abs(from.x - to.x)) {
    const s16 x = to.x > from.x ? to.x - 1 : to.x + 1;
    const s16 y = to.y > from.y ? to.y - 1 : to.y + 1;
    if (verifyMovement(from, (Vect2D_s16){x, y}, MAP_checkDiagonal)) {
      ACTOR_setTargetAnimPos(&queen->actor, to.x, to.y);
      queen->state = QUEEN_MOVING;
      return 1;
    }
  }

  s16 x = to.x > from.x ? 2 : (to.x < from.x ? -2 : 0);
  s16 y = to.y > from.y ? 2 : (to.y < from.y ? -2 : 0);
  x = clamp(from.x + x, 0, mapLevelWidth - 2);
  y = clamp(from.y + y, 0, mapLevelHeight - 2);

  // Move closer to the player
  if (x == from.x &&
      verifyMovement(from, (Vect2D_s16){x, y}, MAP_checkVertical)) {
    ACTOR_setTargetAnimPos(&queen->actor, x, y);
    queen->state = QUEEN_MOVING;
    return 1;
  }

  if (y == from.y &&
      verifyMovement(from, (Vect2D_s16){x, y}, MAP_checkHorizontal)) {
    ACTOR_setTargetAnimPos(&queen->actor, x, y);
    queen->state = QUEEN_MOVING;
    return 1;
  }

  if (x != from.x && y != from.y &&
      verifyMovement(from, (Vect2D_s16){x, y}, MAP_checkDiagonal)) {
    ACTOR_setTargetAnimPos(&queen->actor, x, y);
    queen->state = QUEEN_MOVING;
    return 1;
  }

  return 0;
}

inline static s8 moveAnimation(Queen *queen) {
  if (frame % 32 == 0) {
    ACTOR_animateTo(&queen->actor);

    if (!queen->actor.moving) {
      if (ACTOR_checkCollision(&queen->actor)) {
        kprintf("Player hit");
        player.health--;
        player.state = PLAYER_DEAD;
        return 0;
      }

      queen->state = QUEEN_IDLE;
      return 0;
    }
  }

  return 1;
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void QUEEN_init(Queen *queen, const SpriteDefinition *sprite, const u16 palette,
                const s16 x, const s16 y) {
  queen->state = QUEEN_IDLE;
  queen->indexSprite = 0;
  ACTOR_init(&queen->actor, sprite, palette, x, y, COLLISION_TYPE_QUEEN);
}

void QUEEN_deallocDestroy(Queen *queen) {
  queen->state = QUEEN_DESTROYED;
  ACTOR_destroy(&queen->actor);
}

void QUEEN_dealloc(Queen *queen) { ACTOR_deallocSprite(&queen->actor); }

s8 QUEEN_update(Queen *queen) {
  switch (queen->state) {
  case QUEEN_DEAD:
  case QUEEN_DESTROYED:
    return 0;
  case QUEEN_MOVING:
    return moveAnimation(queen);
  case QUEEN_IDLE:
    return startMovement(queen);
  }

  return 1;
}

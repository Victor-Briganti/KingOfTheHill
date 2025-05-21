#include "enemy/pawn.h"
#include "player/player.h"

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

inline static void startMovement(Pawn *pawn) {
  s16 x = pawn->actor.collisionCurPos.x;
  s16 y = clamp(pawn->actor.collisionCurPos.y + 2, 0, mapLevelHeight - 2);

  if (map[y][x] != COLLISION_TYPE_EMPTY) {
    turn = PLAYER;
    return;
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

  ACTOR_setTargetAnimPos(&pawn->actor, x, y);
  pawn->state = PAWN_MOVING;
}

inline static void callAnimation(Pawn *pawn) {
  if (frame % FRAME_ANIMATION == 0) {
    ACTOR_animateTo(&pawn->actor);

    if (!pawn->actor.moving) {
      if (ACTOR_checkCollision(&pawn->actor)) {
        kprintf("Player hit");
        player.health--;
        player.state = PLAYER_DEAD;
      }

      turn = PLAYER;
      pawn->state = PAWN_IDLE;
    }
  }
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void PAWN_init(Pawn *pawn, const SpriteDefinition *sprite, const u16 palette,
               const s16 x, const s16 y) {
  pawn->state = PAWN_IDLE;
  ACTOR_init(&pawn->actor, sprite, palette, x, y, COLLISION_TYPE_PAWN);
}

void PAWN_deallocDestroy(Pawn *pawn) {
  pawn->state = PAWN_DESTROYED;
  ACTOR_destroy(&pawn->actor);
}

void PAWN_dealloc(Pawn *pawn) { ACTOR_destroy(&pawn->actor); }

s8 PAWN_update(Pawn *pawn) {
  if (turn == PLAYER)
    return -1;

  if (pawn->state == PAWN_DEAD || pawn->state == PAWN_DESTROYED)
    return 0;

  if (pawn->state == PAWN_MOVING) {
    callAnimation(pawn);

    if (pawn->state == PAWN_IDLE)
      return 0;
    else
      return 1;
  }

  startMovement(pawn);
  return 1;
}

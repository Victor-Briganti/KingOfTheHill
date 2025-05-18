#include "enemy/pawn.h"

Pawn pawn;

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

inline static void startMovement() {
  s16 x = pawn.actor.collisionCurPos.x; 
  s16 y = clamp(pawn.actor.collisionCurPos.y + 2, 0, mapLevelHeight - 2) ; 
  ACTOR_setTargetAnimPos(&pawn.actor, x, y);
  pawn.state = PAWN_MOVING;
}

inline static void callAnimation() {
  if (frame % FRAME_ANIMATION == 0) {
    ACTOR_animateTo(&pawn.actor);
    
    if (!pawn.actor.moving) {
      turn = PLAYER;
      pawn.state = PAWN_IDLE;
    }
  }
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void PAWN_init() {
  pawn.state = PAWN_IDLE;
}

void PAWN_update() {
  if (turn == PLAYER)
    return;

  if (pawn.state == PAWN_MOVING) {
    callAnimation();
    return;
  }

  startMovement();
}

void PAWN_levelInit(const SpriteDefinition *sprite, u16 palette, s16 x, s16 y) {
  ACTOR_init(&pawn.actor, sprite, palette, x, y, COLLISION_TYPE_PAWN);
}

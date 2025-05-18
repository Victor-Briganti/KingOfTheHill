#include "enemy/pawn.h"
#include "gameobject/gameobject.h"

Pawn pawn;

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

inline static void startMovement() {
  s16 x = pawn.object.pos.x; 
  s16 y = pawn.object.pos.y + 2; 
  y = clamp(y, 0, mapLevelHeight - 2);
  GAMEOBJECT_setTargetPos(&pawn.object, x, y);
  pawn.state = PAWN_MOVING;
}

inline static void callAnimation() {
  if (frame % FRAME_ANIMATION == 0) {
    GAMEOBJECT_animateTo(&pawn.object);
    
    if (!pawn.object.moving) {
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
  GAMEOBJECT_initInBoard(&pawn.object, sprite, palette, x, y, COLLISION_OBJECT_PAWN);
}

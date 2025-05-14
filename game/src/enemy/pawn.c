#include "enemy/pawn.h"
#include "gameobject/gameobject.h"

Pawn pawn;

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

inline static void updatePosition() {
  pawn.previousX = pawn.posX;
  pawn.previousY = pawn.posY;
  pawn.posY += 2;
  pawn.posY = clamp(pawn.posY, 0, mapLevelHeight - 2);
  pawn.state = PAWN_MOVING;
}

inline static void callAnimation() {
  if (frame % FRAME_ANIMATION == 0) {
    if (GAMEOBJECT_animateTo(&pawn.object, pawn.posX, pawn.posY)) {
      turn = PLAYER;
      pawn.state = PAWN_IDLE;
    }
  }
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void PAWN_init() {
  pawn.previousX = 0;
  pawn.previousY = 0;
  pawn.state = PAWN_IDLE;
}

void PAWN_update() {
  if (turn == PLAYER)
    return;

  if (pawn.state == PAWN_MOVING) {
    callAnimation();
    return;
  }

  updatePosition();
}

void PAWN_levelInit(const SpriteDefinition *sprite, u16 palette, s16 x, s16 y) {
  pawn.posX = x;
  pawn.posY = y;
  GAMEOBJECT_initInBoard(&pawn.object, sprite, palette, x, y);
}

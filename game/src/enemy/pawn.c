#include "enemy/pawn.h"

Pawn pawn;

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

inline static void PAWN_updatePosition() {
  pawn.previousX = pawn.posX;
  pawn.previousY = pawn.posY;
  pawn.posY += 2;
  pawn.posY = clamp(pawn.posY, 0, mapLevelHeight - 2);
  pawn.state = PAWN_MOVING;
}

inline static void PAWN_moveAnimation() {
  if (frame % FRAME_ANIMATION == 0) {
    if (pawn.object.x < pawn.posX)
      pawn.object.x++;
    else if (pawn.object.x > pawn.posX)
      pawn.object.x--;

    if (pawn.object.y < pawn.posY)
      pawn.object.y++;
    else if (pawn.object.y > pawn.posY)
      pawn.object.y--;

    if (pawn.object.x == pawn.posX && pawn.object.y == pawn.posY) {
      turn = PLAYER;
      pawn.state = PAWN_IDLE;
    }

    GAMEOBJECT_updatePos(&pawn.object);
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
    PAWN_moveAnimation();
    return;
  }

  PAWN_updatePosition();
}

void PAWN_levelInit(const SpriteDefinition *sprite, u16 palette, s16 x, s16 y) {
  pawn.posX = x;
  pawn.posY = y;
  GAMEOBJECT_init(&pawn.object, sprite, palette, x, y);
}

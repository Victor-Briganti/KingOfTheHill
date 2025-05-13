#ifndef __PAWN_H__
#define __PAWN_H__

#include "gameobject/gameobject.h"

typedef struct Pawn {
  GameObject object;
  
  s16 previousX;
  s16 previousY;
} Pawn;

extern Pawn pawn;

void PAWN_init();

void PAWN_update();

void PAWN_levelInit(const SpriteDefinition *sprite, u16 palette, s16 x, s16 y);

#endif // __PAWN_H__

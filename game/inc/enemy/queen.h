#ifndef __QUEEN_H__
#define __QUEEN_H__

#include "node/actor.h"

typedef enum QueenState {
  QUEEN_MOVING,
  QUEEN_IDLE,
  QUEEN_DEAD,
  QUEEN_DESTROYED,
} QueenState;

typedef struct Queen {
  ActorNode actor;
  QueenState state;
  u8 indexSprite;
} Queen;

void QUEEN_init(Queen *queen, const SpriteDefinition *sprite, const u16 palette,
               const s16 x, const s16 y);

void QUEEN_deallocDestroy(Queen *queen);

void QUEEN_dealloc(Queen *queen);

// Return 0 when ends its movement and 1 if still updating
s8 QUEEN_update(Queen *queen);

#endif // __QUEEN_H__

#ifndef __PAWN_H__
#define __PAWN_H__

#include "enemy/queen.h"
#include "node/actor.h"

typedef enum PawnState {
  PAWN_MOVING,
  PAWN_IDLE,
  PAWN_PROMOTION,
  PAWN_PROMOTED,
  PAWN_DEAD,
  PAWN_DESTROYED,
} PawnState;

typedef struct Pawn {
  Queen queen;
  ActorNode actor;
  PawnState state;
  u8 indexSprite;
} Pawn;

void PAWN_init(Pawn *pawn, const SpriteDefinition *sprite, const u16 palette,
               const s16 x, const s16 y);

void PAWN_deallocDestroy(Pawn *pawn);

void PAWN_dealloc(Pawn *pawn);

// Return 0 when ends its movement and 1 if still updating
s8 PAWN_update(Pawn *pawn);

#endif // __PAWN_H__

#ifndef __PAWN_H__
#define __PAWN_H__

#include "node/actor.h"

typedef enum PawnState {
  PAWN_MOVING,
  PAWN_IDLE,
  PAWN_DEAD,
} PawnState;

typedef struct Pawn {
  ActorNode actor;
  PawnState state;
} Pawn;

void PAWN_init(Pawn *pawn, const SpriteDefinition *sprite, const u16 palette,
               const s16 x, const s16 y);

void PAWN_destroy(Pawn *pawn);

void PAWN_update(Pawn *pawn);

#endif // __PAWN_H__

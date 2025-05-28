#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "node/actor.h"

#include <maths.h>

typedef enum PlayerState {
  PLAYER_MOVING,
  PLAYER_IDLE,
  PLAYER_DAMAGED,
  PLAYER_DEAD,
} PlayerState;

typedef struct Player {
  ActorNode actor;

  // Define the cursor position
  Vect2D_s16 cursor;

  PlayerState state;

  // Define the health of the player
  u8 health, totalHealth;
} Player;

extern Player player;

void PLAYER_init();

void PLAYER_destroy();

// Return 0 if the animation finish or the player could not move
// Return 1 if the animation is still happening
s8 PLAYER_update();

void PLAYER_levelInit(const SpriteDefinition *sprite, u16 palette, s16 x,
                      s16 y);

#endif //  __PLAYER_H__

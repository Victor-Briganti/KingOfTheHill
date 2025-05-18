#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "node/actor.h"

#include <maths.h>

typedef enum PlayerState {
  PLAYER_MOVING,
  PLAYER_IDLE,
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

void PLAYER_update();

void PLAYER_levelInit(const SpriteDefinition *sprite, u16 palette, s16 x,
                      s16 y);

#endif //  __PLAYER_H__

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "gameobject/gameobject.h"

#include <maths.h>

typedef enum PlayerState {
  PLAYER_MOVING,
  PLAYER_IDLE,
} PlayerState;

typedef struct Player {
  GameObject object;
  
  // Define the cursor position
  Vect2D_s16 cursor;
  
  // Define the previous player position
  s16 previousX;
  s16 previousY;
  
  // Define the final position of the player
  s16 posX;
  s16 posY;
  
  u8 health;
  u8 totalHealth;
  
  PlayerState state;
} Player;

extern Player player;

void PLAYER_init();

void PLAYER_update();

void PLAYER_levelInit(const SpriteDefinition *sprite, u16 palette, s16 x,
                      s16 y);

#endif //  __PLAYER_H__

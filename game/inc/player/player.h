#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "gameobject/gameobject.h"

typedef struct Player {
  GameObject object;
  u8 health;
  u8 totalHealth;
  
  // Define the cursor position
  s16 cursorX;
  s16 cursorY;
  
  // Define the previous player position
  s16 previousX;
  s16 previousY;
} Player;

extern Player player;

void PLAYER_init();

void PLAYER_update();

void PLAYER_levelInit(const SpriteDefinition *sprite, u16 palette, s16 x, s16 y);

#endif //  __PLAYER_H__

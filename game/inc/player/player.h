#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "gameobject/gameobject.h"

typedef struct Player {
  GameObject object;
  u8 health;
  u8 totalHealth;
  
  //  Define the selector position
  s8 cursorX; 
  s8 cursorY; 
} Player;

extern Player player;

void PLAYER_init();

void PLAYER_update();

void PLAYER_levelInit(const SpriteDefinition *sprite, u16 palette, s8 x, s8 y);

#endif //  __PLAYER_H__

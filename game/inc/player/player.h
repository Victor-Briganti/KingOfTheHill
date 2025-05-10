#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "gameobject/gameobject.h"

typedef struct Player {
  GameObject object;
} Player;

extern Player player;

void PLAYER_init(const SpriteDefinition *sprite, u16 palette, s8 x, s8 y);

void PLAYER_update();

#endif //  __PLAYER_H__

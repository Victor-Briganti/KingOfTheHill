#include "player/player.h"

Player player;

void PLAYER_init(const SpriteDefinition *sprite, u16 palette, s8 x, s8 y) {
  player.object.x = x;
  player.object.y = y;

  GAMEOBJECT_init(&player.object, sprite, palette, x, y);
}

void PLAYER_update() {
  GAMEOBJECT_clamp(&player.object, MAP_LEVEL1_WIDTH - 1, MAP_LEVEL1_HEIGHT - 1);
  SPR_setPosition(player.object.sprite, POS_X(player.object.x),
                  POS_Y(player.object.y));
}

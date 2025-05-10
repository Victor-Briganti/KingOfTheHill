#include "player/player.h"
#include "tilemap/tilemap.h"

Player player;

static void PLAYER_inputHandler(u16 joy, u16 changed, u16 state) {
  if (joy != JOY_1)
    return;

  // Verify if the directionals are pressed
  u16 directional = state & BUTTON_DIR;

  // Move when the buttons are released
  if (!(directional)) {
    if (changed & BUTTON_LEFT) {
      player.object.x -= 2;
    } else if (changed & BUTTON_RIGHT) {
      player.object.x += 2;
    } else if (changed & BUTTON_DOWN) {
      player.object.y += 2;
    } else if (changed & BUTTON_UP) {
      player.object.y -= 2;
    }

    GAMEOBJECT_clamp(&player.object, mapLevelWidth - 1, mapLevelHeight - 1);
    SPR_setPosition(player.object.sprite, POS_X(player.object.x),
                    POS_Y(player.object.y));
  }
}

static void PLAYER_updateSelecTile() {
  if (player.object.x < mapLevelWidth - 2)
    TILEMAP_updateRightTile(player.object.x, player.object.y, mapLevelX,
                            mapLevelY, GREEN_TILE);

  if (player.object.x != 0)
    TILEMAP_updateLeftTile(player.object.x, player.object.y, mapLevelX,
                           mapLevelY, GREEN_TILE);

  if (player.object.y != 0)
    TILEMAP_updateUpTile(player.object.x, player.object.y, mapLevelX, mapLevelY,
                         GREEN_TILE);

  if (player.object.y < mapLevelHeight - 2)
    TILEMAP_updateBottomTile(player.object.x, player.object.y, mapLevelX,
                             mapLevelY, GREEN_TILE);

  if (player.object.x < mapLevelWidth - 2 && player.object.y != 0)
    TILEMAP_updateUpRighTile(player.object.x, player.object.y, mapLevelX,
                             mapLevelY, GREEN_TILE);

  if (player.object.x != 0 && player.object.y != 0)
    TILEMAP_updateUpLeftTile(player.object.x, player.object.y, mapLevelX,
                             mapLevelY, GREEN_TILE);

  if (player.object.x < mapLevelWidth - 2 &&
      player.object.y < mapLevelHeight - 2)
    TILEMAP_updateBottomRightTile(player.object.x, player.object.y, mapLevelX,
                                  mapLevelY, GREEN_TILE);

  if (player.object.x != 0 && player.object.y < mapLevelHeight - 2)
    TILEMAP_updateBottomLeftTile(player.object.x, player.object.y, mapLevelX,
                                 mapLevelY, GREEN_TILE);
}

void PLAYER_init() {
  player.object.x = 0;
  player.object.y = 0;
  player.health = 0;
  player.totalHealth = 0;
}

void PLAYER_update() { PLAYER_updateSelecTile(); }

void PLAYER_levelInit(const SpriteDefinition *sprite, u16 palette, s8 x, s8 y) {
  player.object.x = x;
  player.object.y = y;

  GAMEOBJECT_init(&player.object, sprite, palette, x, y);
  JOY_setEventHandler(PLAYER_inputHandler);
}

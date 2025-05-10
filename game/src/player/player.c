#include "player/player.h"
#include "global.h"
#include "tilemap/tilemap.h"

Player player;

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

static inline bool PLAYER_onBottom() {
  return (player.object.y < mapLevelHeight - 2);
}

static inline bool PLAYER_onUp() { return (player.object.y != 0); }

static inline bool PLAYER_onLeft() { return (player.object.x != 0); }

static inline bool PLAYER_onRight() {
  return (player.object.x < mapLevelWidth - 2);
}

static void PLAYER_handleCursorPos(s8 x,  s8 y) {
  if (x == player.object.x && y == player.object.y)
   return;

  x = clamp(x, player.object.x - 2, player.object.x + 2);
  y = clamp(y, player.object.y - 2, player.object.y + 2);
  player.cursorX = clamp(x, 0, mapLevelWidth - 2);
  player.cursorY = clamp(y, 0, mapLevelHeight - 2);
}

static void PLAYER_inputHandler(u16 joy, u16 changed, u16 state) {
  if (joy != JOY_1)
    return;

  // NOTE: The BUTTON_X is the z in the keyboard

  // Verify if the directionals are pressed
  u16 directional = state & BUTTON_DIR;

  // Move when the buttons are released
  if (!(directional)) {
    s8 x = player.cursorX;
    s8 y = player.cursorY;
    if (changed & BUTTON_LEFT) {
      x = player.cursorX - 2;
    } else if (changed & BUTTON_RIGHT) {
      x = player.cursorX + 2;
    } else if (changed & BUTTON_DOWN) {
      y = player.cursorY + 2;
    } else if (changed & BUTTON_UP) {
      y = player.cursorY - 2;
    }

    PLAYER_handleCursorPos(x, y);

    // player.object.x = clamp(player.object.x, 0, mapLevelWidth - 2);
    // player.object.y = clamp(player.object.y, 0, mapLevelHeight - 2);
    // GAMEOBJECT_updatePos(&player.object, mapLevelWidth - 2, mapLevelHeight -
    // 2);
  }
}

inline static void PLAYER_updateSelectTile() {
  if (PLAYER_onRight())
    TILEMAP_updateRightTile(player.object.x, player.object.y, mapLevelX,
                            mapLevelY, GREEN_TILE);

  if (PLAYER_onLeft())
    TILEMAP_updateLeftTile(player.object.x, player.object.y, mapLevelX,
                           mapLevelY, GREEN_TILE);

  if (PLAYER_onUp())
    TILEMAP_updateUpTile(player.object.x, player.object.y, mapLevelX, mapLevelY,
                         GREEN_TILE);

  if (PLAYER_onBottom())
    TILEMAP_updateBottomTile(player.object.x, player.object.y, mapLevelX,
                             mapLevelY, GREEN_TILE);

  if (PLAYER_onUp() && PLAYER_onRight())
    TILEMAP_updateUpRighTile(player.object.x, player.object.y, mapLevelX,
                             mapLevelY, GREEN_TILE);

  if (PLAYER_onUp() && PLAYER_onLeft())
    TILEMAP_updateUpLeftTile(player.object.x, player.object.y, mapLevelX,
                             mapLevelY, GREEN_TILE);

  if (PLAYER_onBottom() && PLAYER_onRight())
    TILEMAP_updateBottomRightTile(player.object.x, player.object.y, mapLevelX,
                                  mapLevelY, GREEN_TILE);

  if (PLAYER_onBottom() && PLAYER_onLeft())
    TILEMAP_updateBottomLeftTile(player.object.x, player.object.y, mapLevelX,
                                 mapLevelY, GREEN_TILE);
}

inline static void PLAYER_updateCursorTile() {
  TILEMAP_setTile(player.cursorX, player.cursorY, mapLevelX, mapLevelY,
                  BLUE_TILE);
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void PLAYER_init() {
  player.object.x = 0;
  player.object.y = 0;
  player.health = 0;
  player.totalHealth = 0;
  player.cursorX = 0;
  player.cursorY = 0;
}

void PLAYER_update() {
  PLAYER_updateSelectTile();
  PLAYER_updateCursorTile();
}

void PLAYER_levelInit(const SpriteDefinition *sprite, u16 palette, s8 x, s8 y) {
  player.object.x = x;
  player.object.y = y;

  player.cursorX = x;
  player.cursorY = y - 2;

  GAMEOBJECT_init(&player.object, sprite, palette, x, y);
  JOY_setEventHandler(PLAYER_inputHandler);
}

#include "player/player.h"
#include "gameobject/gameobject.h"
#include "global.h"
#include "tilemap/tilemap.h"

#include <joy.h>

Player player;

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

static inline bool isOnBottom() { return (player.posY < mapLevelHeight - 2); }

static inline bool isOnTop() { return (player.posY != 0); }

static inline bool isOnLeft() { return (player.posX != 0); }

static inline bool isOnRight() { return (player.posX < mapLevelWidth - 2); }

static void handleCursorPos(s16 x, s16 y, u8 direction) {
  if (x == player.posX && y == player.posY) {
    // Jump the player if the button was preset against it
    if (direction & BUTTON_LEFT) {
      x -= 2;
    } else if (direction & BUTTON_RIGHT) {
      x += 2;
    } else if (direction & BUTTON_DOWN) {
      y += 2;
    } else if (direction & BUTTON_UP) {
      y -= 2;
    }
  }

  // Clamp player limit
  x = clamp(x, player.posX - 2, player.posX + 2);
  y = clamp(y, player.posY - 2, player.posY + 2);

  // Clamp map limit
  x = clamp(x, 0, mapLevelWidth - 2);
  y = clamp(y, 0, mapLevelHeight - 2);

  if (x == player.posX && y == player.posY)
    return;

  player.cursorX = x;
  player.cursorY = y;
}

static void cursorInnertia() {
  s16 x = player.cursorX;
  s16 y = player.cursorY;
  u8 direction = BUTTON_UP;

  if (x == player.posX && y > player.posY) { /* Down of the player */
    y += 2;
    direction = BUTTON_DOWN;
  } else if (x == player.posX && y < player.posY) { /* Up of the player */
    y -= 2;
    direction = BUTTON_UP;
  } else if (x < player.posX && y == player.posY) { /* Left of the player */
    x -= 2;
    direction = BUTTON_LEFT;
  } else if (x > player.posX && y == player.posY) { /* Right of the player */
    x += 2;
    direction = BUTTON_RIGHT;
  } else if (x > player.posX &&
             y > player.posY) { /* Down Right of the player */
    x += 2;
    y += 2;
    direction = BUTTON_RIGHT | BUTTON_DOWN;
  } else if (x > player.posX && y < player.posY) { /* Up Right of the player */
    x += 2;
    y -= 2;
    direction = BUTTON_RIGHT | BUTTON_UP;
  } else if (x < player.posX && y > player.posY) { /* Down Left of the player */
    x -= 2;
    y += 2;
    direction = BUTTON_LEFT | BUTTON_DOWN;
  } else if (x < player.posX &&
             y < player.posY) { /* Down Right of the player */
    x -= 2;
    y -= 2;
    direction = BUTTON_RIGHT | BUTTON_DOWN;
  }

  // Save the old values
  player.previousX = player.posX;
  player.previousY = player.posY;

  player.posX = player.cursorX;
  player.posY = player.cursorY;

  handleCursorPos(x, y, direction);

  if (player.cursorX == player.posX && player.posY == player.cursorY) {
    if (isOnRight())
      player.cursorX -= 4;

    if (isOnLeft())
      player.cursorX += 4;

    if (isOnTop())
      player.cursorY += 4;

    if (isOnBottom())
      player.cursorY -= 4;

    if (isOnTop() && isOnRight()) {
      player.cursorX = player.posX + 2;
      player.cursorY = player.posY;
    }

    if (isOnTop() && isOnLeft()) {
      player.cursorX = player.posX - 2;
      player.cursorY = player.posY;
    }

    if (isOnBottom() && isOnRight()) {
      player.cursorX = player.posX + 2;
      player.cursorY = player.posY;
    }

    if (isOnBottom() && isOnLeft()) {
      player.cursorX = player.posX - 2;
      player.cursorY = player.posY;
    }
  }

  // GAMEOBJECT_updatePos(&player.object);
}

static void inputHandler(u16 joy, u16 changed, u16 state) {
  if (joy != JOY_1 || turn == ENEMY || player.state == PLAYER_MOVING)
    return;

  // Verify if the directionals are pressed
  u16 directional = state & BUTTON_DIR;

  // Verify if the command button was pressed
  u16 command = state & BUTTON_A;

  // Act when the command is pressed
  if (command) {
    cursorInnertia();
    player.state = PLAYER_MOVING;
    return;
  }

  // Move when the buttons are released
  if (!directional) {
    s16 x = player.cursorX;
    s16 y = player.cursorY;
    if (changed & BUTTON_LEFT) {
      x = player.cursorX - 2;
      handleCursorPos(x, y, BUTTON_LEFT);
    } else if (changed & BUTTON_RIGHT) {
      x = player.cursorX + 2;
      handleCursorPos(x, y, BUTTON_RIGHT);
    } else if (changed & BUTTON_DOWN) {
      y = player.cursorY + 2;
      handleCursorPos(x, y, BUTTON_DOWN);
    } else if (changed & BUTTON_UP) {
      y = player.cursorY - 2;
      handleCursorPos(x, y, BUTTON_UP);
    }
  }
}

inline static void updateSelectTile() {
  if (isOnRight())
    TILEMAP_updateRightTile(player.posX, player.posY, mapLevelX, mapLevelY,
                            GREEN_TILE);

  if (isOnLeft())
    TILEMAP_updateLeftTile(player.posX, player.posY, mapLevelX, mapLevelY,
                           GREEN_TILE);

  if (isOnTop())
    TILEMAP_updateUpTile(player.posX, player.posY, mapLevelX, mapLevelY,
                         GREEN_TILE);

  if (isOnBottom())
    TILEMAP_updateBottomTile(player.posX, player.posY, mapLevelX, mapLevelY,
                             GREEN_TILE);

  if (isOnTop() && isOnRight())
    TILEMAP_updateUpRighTile(player.posX, player.posY, mapLevelX, mapLevelY,
                             GREEN_TILE);

  if (isOnTop() && isOnLeft())
    TILEMAP_updateUpLeftTile(player.posX, player.posY, mapLevelX, mapLevelY,
                             GREEN_TILE);

  if (isOnBottom() && isOnRight())
    TILEMAP_updateBottomRightTile(player.posX, player.posY, mapLevelX,
                                  mapLevelY, GREEN_TILE);

  if (isOnBottom() && isOnLeft())
    TILEMAP_updateBottomLeftTile(player.posX, player.posY, mapLevelX, mapLevelY,
                                 GREEN_TILE);
}

inline static void updateCursorTile() {
  TILEMAP_setTile(player.cursorX, player.cursorY, mapLevelX, mapLevelY,
                  BLUE_TILE);
}

inline static void callAnimation() {
  if (frame % FRAME_ANIMATION == 0) {
    if (GAMEOBJECT_animateTo(&player.object, player.posX, player.posY)) {
      turn = ENEMY;
      player.state = PLAYER_IDLE;
    }
  }
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void PLAYER_init() {
  player.state = PLAYER_IDLE;
  player.health = 6;
  player.totalHealth = 6;
  player.cursorX = 0;
  player.cursorY = 0;
  player.posX = 0;
  player.posY = 0;
}

void PLAYER_update() {
  if (turn == ENEMY)
    return;

  if (player.state == PLAYER_MOVING) {
    callAnimation();
    return;
  }

  updateSelectTile();
  updateCursorTile();
}

void PLAYER_levelInit(const SpriteDefinition *sprite, u16 palette, s16 x,
                      s16 y) {
  GAMEOBJECT_initInBoard(&player.object, sprite, palette, x, y);
  JOY_setEventHandler(inputHandler);

  player.cursorX = x;
  player.cursorY = y - 2;

  player.posX = x;
  player.posY = y;
}

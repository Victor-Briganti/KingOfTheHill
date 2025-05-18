#include "player/player.h"
#include "gameobject/gameobject.h"
#include "global.h"
#include "tilemap/tilemap.h"

#include <joy.h>

Player player;

typedef struct CursorMovement {
  s16 x, y, direction;
} CursorMovement;

static const CursorMovement cursorMoves[] = {
    {.x = 2, .y = 0, .direction = BUTTON_RIGHT},
    {.x = -2, .y = 0, .direction = BUTTON_LEFT},
    {.x = 0, .y = 2, .direction = BUTTON_DOWN},
    {.x = 0, .y = -2, .direction = BUTTON_UP},
};

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

static inline bool playerBottomPos() {
  return (player.posY < mapLevelHeight - 2);
}

static inline bool playerTopPos() { return (player.posY != 0); }

static inline bool playerLeftPos() { return (player.posX != 0); }

static inline bool playerRightPos() {
  return (player.posX < mapLevelWidth - 2);
}

static void handleCursorPos(s16 x, s16 y, u8 direction) {
  if (x == player.posX && y == player.posY) {
    // Jump the player if the button was preset against it
    for (u8 i = 0; i < 4; i++) {
      if (cursorMoves[i].direction & direction) {
        x += cursorMoves[i].x;
        y += cursorMoves[i].y;
        break;
      }
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

  player.cursor = (Vect2D_s16){x, y};
}

static void cursorInertia() {
  s16 x = player.cursor.x;
  s16 y = player.cursor.y;
  s16 dx = x - player.posX;
  s16 dy = y - player.posY;
  u8 direction = 0;

  if (dx > 0)
    direction |= BUTTON_RIGHT;
  if (dx < 0)
    direction |= BUTTON_LEFT;

  if (dy > 0)
    direction |= BUTTON_DOWN;
  if (dy < 0)
    direction |= BUTTON_UP;

  for (u8 i = 0; i < 4; i++) {
    if (cursorMoves[i].direction & direction) {
      x += cursorMoves[i].x;
      y += cursorMoves[i].y;
    }
  }

  // Save the old values
  player.previousX = player.posX;
  player.previousY = player.posY;

  player.posX = player.cursor.x;
  player.posY = player.cursor.y;

  handleCursorPos(x, y, direction);

  if (player.cursor.x == player.posX && player.posY == player.cursor.y) {
    if (playerRightPos())
      player.cursor.x -= 4;

    if (playerLeftPos())
      player.cursor.x += 4;

    if (playerTopPos())
      player.cursor.y += 4;

    if (playerBottomPos())
      player.cursor.y -= 4;

    if ((playerTopPos() && playerRightPos()) ||
        (playerBottomPos() && playerRightPos()))
      player.cursor = (Vect2D_s16){player.posX + 2, player.posY};

    if ((playerTopPos() && playerLeftPos()) ||
        (playerBottomPos() && playerLeftPos()))
      player.cursor = (Vect2D_s16){player.posX - 2, player.posY};
  }
}

static void inputHandler(const u16 joy, const u16 changed, const u16 state) {
  if (joy != JOY_1 || turn == ENEMY || player.state == PLAYER_MOVING)
    return;

  // Verify if the directionals are pressed
  const u16 directional = state & BUTTON_DIR;

  // Verify if the command button was pressed
  const u16 command = state & BUTTON_A;

  // Act when the command is pressed
  if (command) {
    cursorInertia();
    player.state = PLAYER_MOVING;
    return;
  }

  // Move when the buttons are released
  if (!directional) {
    s16 x = player.cursor.x;
    s16 y = player.cursor.y;
    for (u8 i = 0; i < 4; i++) {
      if (cursorMoves[i].direction & changed) {
        x += cursorMoves[i].x;
        y += cursorMoves[i].y;
        handleCursorPos(x, y, cursorMoves[i].direction);
        break;
      }
    }
  }
}

inline static void updateSelectTile() {
  if (playerRightPos())
    TILEMAP_updateRightTile(player.posX, player.posY, mapLevelX, mapLevelY,
                            GREEN_TILE);

  if (playerLeftPos())
    TILEMAP_updateLeftTile(player.posX, player.posY, mapLevelX, mapLevelY,
                           GREEN_TILE);

  if (playerTopPos())
    TILEMAP_updateUpTile(player.posX, player.posY, mapLevelX, mapLevelY,
                         GREEN_TILE);

  if (playerBottomPos())
    TILEMAP_updateBottomTile(player.posX, player.posY, mapLevelX, mapLevelY,
                             GREEN_TILE);

  if (playerTopPos() && playerRightPos())
    TILEMAP_updateUpRighTile(player.posX, player.posY, mapLevelX, mapLevelY,
                             GREEN_TILE);

  if (playerTopPos() && playerLeftPos())
    TILEMAP_updateUpLeftTile(player.posX, player.posY, mapLevelX, mapLevelY,
                             GREEN_TILE);

  if (playerBottomPos() && playerRightPos())
    TILEMAP_updateBottomRightTile(player.posX, player.posY, mapLevelX,
                                  mapLevelY, GREEN_TILE);

  if (playerBottomPos() && playerLeftPos())
    TILEMAP_updateBottomLeftTile(player.posX, player.posY, mapLevelX, mapLevelY,
                                 GREEN_TILE);
}

inline static void updateCursorTile() {
  TILEMAP_setTile(player.cursor.x, player.cursor.y, mapLevelX, mapLevelY,
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
  player.cursor.x = 0;
  player.cursor.y = 0;
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

  player.cursor = (Vect2D_s16){x, y - 2}; 

  player.posX = x;
  player.posY = y;
}

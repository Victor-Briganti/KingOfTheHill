#include "player/player.h"
#include "gameobject/gameobject.h"
#include "global.h"
#include "map/map.h"
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
  return (GAMEOBJECT_getCurCollisionY(&player.object) < mapLevelHeight - 2);
}

static inline bool playerTopPos() {
  return (GAMEOBJECT_getCurCollisionY(&player.object) != 0);
}

static inline bool playerLeftPos() {
  return (GAMEOBJECT_getCurCollisionX(&player.object) != 0);
}

static inline bool playerRightPos() {
  return (GAMEOBJECT_getCurCollisionX(&player.object) < mapLevelWidth - 2);
}

static void handleCursorPos(s16 x, s16 y, u8 direction) {
  if (x == GAMEOBJECT_getCurCollisionX(&player.object) &&
      y == GAMEOBJECT_getCurCollisionY(&player.object)) {
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
  x = clamp(x, GAMEOBJECT_getCurCollisionX(&player.object) - 2,
            GAMEOBJECT_getCurCollisionX(&player.object) + 2);
  y = clamp(y, GAMEOBJECT_getCurCollisionY(&player.object) - 2,
            GAMEOBJECT_getCurCollisionY(&player.object) + 2);

  // Clamp map limit
  x = clamp(x, 0, mapLevelWidth - 2);
  y = clamp(y, 0, mapLevelHeight - 2);

  if (x == GAMEOBJECT_getCurCollisionX(&player.object) &&
      y == GAMEOBJECT_getCurCollisionY(&player.object))
    return;

  player.cursor = (Vect2D_s16){x, y};
}

static void cursorInertia() {
  s16 x = player.cursor.x;
  s16 y = player.cursor.y;
  s16 dx = x - GAMEOBJECT_getCurCollisionX(&player.object);
  s16 dy = y - GAMEOBJECT_getCurCollisionY(&player.object);
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
  GAMEOBJECT_setTargetPos(&player.object, player.cursor.x, player.cursor.y);
  handleCursorPos(x, y, direction);

  if (player.cursor.x == GAMEOBJECT_getCurCollisionX(&player.object) &&
      GAMEOBJECT_getCurCollisionY(&player.object) == player.cursor.y) {
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
      player.cursor =
          (Vect2D_s16){GAMEOBJECT_getCurCollisionX(&player.object) + 2,
                       GAMEOBJECT_getCurCollisionY(&player.object)};

    if ((playerTopPos() && playerLeftPos()) ||
        (playerBottomPos() && playerLeftPos()))
      player.cursor =
          (Vect2D_s16){GAMEOBJECT_getCurCollisionX(&player.object) - 2,
                       GAMEOBJECT_getCurCollisionY(&player.object)};
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
    TILEMAP_updateRightTile(GAMEOBJECT_getCurCollisionX(&player.object),
                            GAMEOBJECT_getCurCollisionY(&player.object),
                            mapLevelX, mapLevelY, GREEN_TILE);

  if (playerLeftPos())
    TILEMAP_updateLeftTile(GAMEOBJECT_getCurCollisionX(&player.object),
                           GAMEOBJECT_getCurCollisionY(&player.object),
                           mapLevelX, mapLevelY, GREEN_TILE);

  if (playerTopPos())
    TILEMAP_updateUpTile(GAMEOBJECT_getCurCollisionX(&player.object),
                         GAMEOBJECT_getCurCollisionY(&player.object), mapLevelX,
                         mapLevelY, GREEN_TILE);

  if (playerBottomPos())
    TILEMAP_updateBottomTile(GAMEOBJECT_getCurCollisionX(&player.object),
                             GAMEOBJECT_getCurCollisionY(&player.object),
                             mapLevelX, mapLevelY, GREEN_TILE);

  if (playerTopPos() && playerRightPos())
    TILEMAP_updateUpRighTile(GAMEOBJECT_getCurCollisionX(&player.object),
                             GAMEOBJECT_getCurCollisionY(&player.object),
                             mapLevelX, mapLevelY, GREEN_TILE);

  if (playerTopPos() && playerLeftPos())
    TILEMAP_updateUpLeftTile(GAMEOBJECT_getCurCollisionX(&player.object),
                             GAMEOBJECT_getCurCollisionY(&player.object),
                             mapLevelX, mapLevelY, GREEN_TILE);

  if (playerBottomPos() && playerRightPos())
    TILEMAP_updateBottomRightTile(GAMEOBJECT_getCurCollisionX(&player.object),
                                  GAMEOBJECT_getCurCollisionY(&player.object),
                                  mapLevelX, mapLevelY, GREEN_TILE);

  if (playerBottomPos() && playerLeftPos())
    TILEMAP_updateBottomLeftTile(GAMEOBJECT_getCurCollisionX(&player.object),
                                 GAMEOBJECT_getCurCollisionY(&player.object),
                                 mapLevelX, mapLevelY, GREEN_TILE);
}

inline static void updateCursorTile() {
  kprintf("Cursor: %d", map[player.cursor.y][player.cursor.x]);
  if (map[player.cursor.y][player.cursor.x] != 0) {
    TILEMAP_setTile(player.cursor.x, player.cursor.y, mapLevelX, mapLevelY,
                    RED_TILE);
  } else {
    TILEMAP_setTile(player.cursor.x, player.cursor.y, mapLevelX, mapLevelY,
                    BLUE_TILE);
  }
}

inline static void callAnimation() {
  if (frame % FRAME_ANIMATION == 0) {
    GAMEOBJECT_animateTo(&player.object);

    if (!player.object.moving) {
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
  GAMEOBJECT_initInBoard(&player.object, sprite, palette, x, y,
                         COLLISION_OBJECT_PLAYER);
  JOY_setEventHandler(inputHandler);

  player.cursor = (Vect2D_s16){x, y - 2};
}

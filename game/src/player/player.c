#include "player/player.h"
#include "enemy/pawn.h"
#include "global.h"
#include "map/map.h"
#include "node/actor.h"
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
  return (player.actor.collisionCurPos.y < mapLevelHeight - 2);
}

static inline bool playerTopPos() {
  return (player.actor.collisionCurPos.y != 0);
}

static inline bool playerLeftPos() {
  return (player.actor.collisionCurPos.x != 0);
}

static inline bool playerRightPos() {
  return (player.actor.collisionCurPos.x < mapLevelWidth - 2);
}

static void handleCursorPos(s16 x, s16 y, u8 direction) {
  if (x == player.actor.collisionCurPos.x &&
      y == player.actor.collisionCurPos.y) {
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
  x = clamp(x, player.actor.collisionCurPos.x - 2,
            player.actor.collisionCurPos.x + 2);
  y = clamp(y, player.actor.collisionCurPos.y - 2,
            player.actor.collisionCurPos.y + 2);

  // Clamp map limit
  x = clamp(x, 0, mapLevelWidth - 2);
  y = clamp(y, 0, mapLevelHeight - 2);

  if (x == player.actor.collisionCurPos.x &&
      y == player.actor.collisionCurPos.y)
    return;

  player.cursor = (Vect2D_s16){x, y};
}

static void cursorInertia() {
  s16 x = player.cursor.x;
  s16 y = player.cursor.y;
  s16 dx = x - player.actor.collisionCurPos.x;
  s16 dy = y - player.actor.collisionCurPos.y;
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
  ACTOR_setTargetAnimPos(&player.actor, player.cursor.x, player.cursor.y);
  handleCursorPos(x, y, direction);

  if (player.cursor.x == player.actor.collisionCurPos.x &&
      player.actor.collisionCurPos.y == player.cursor.y) {
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
      player.cursor = (Vect2D_s16){player.actor.collisionCurPos.x + 2,
                                   player.actor.collisionCurPos.y};

    if ((playerTopPos() && playerLeftPos()) ||
        (playerBottomPos() && playerLeftPos()))
      player.cursor = (Vect2D_s16){player.actor.collisionCurPos.x - 2,
                                   player.actor.collisionCurPos.y};
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
    TILEMAP_updateRightTile(player.actor.collisionCurPos.x,
                            player.actor.collisionCurPos.y, mapLevelX,
                            mapLevelY, GREEN_TILE);

  if (playerLeftPos())
    TILEMAP_updateLeftTile(player.actor.collisionCurPos.x,
                           player.actor.collisionCurPos.y, mapLevelX, mapLevelY,
                           GREEN_TILE);

  if (playerTopPos())
    TILEMAP_updateUpTile(player.actor.collisionCurPos.x,
                         player.actor.collisionCurPos.y, mapLevelX, mapLevelY,
                         GREEN_TILE);

  if (playerBottomPos())
    TILEMAP_updateBottomTile(player.actor.collisionCurPos.x,
                             player.actor.collisionCurPos.y, mapLevelX,
                             mapLevelY, GREEN_TILE);

  if (playerTopPos() && playerRightPos())
    TILEMAP_updateUpRightTile(player.actor.collisionCurPos.x,
                             player.actor.collisionCurPos.y, mapLevelX,
                             mapLevelY, GREEN_TILE);

  if (playerTopPos() && playerLeftPos())
    TILEMAP_updateUpLeftTile(player.actor.collisionCurPos.x,
                             player.actor.collisionCurPos.y, mapLevelX,
                             mapLevelY, GREEN_TILE);

  if (playerBottomPos() && playerRightPos())
    TILEMAP_updateBottomRightTile(player.actor.collisionCurPos.x,
                                  player.actor.collisionCurPos.y, mapLevelX,
                                  mapLevelY, GREEN_TILE);

  if (playerBottomPos() && playerLeftPos())
    TILEMAP_updateBottomLeftTile(player.actor.collisionCurPos.x,
                                 player.actor.collisionCurPos.y, mapLevelX,
                                 mapLevelY, GREEN_TILE);
}

inline static void updateCursorTile() {
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
    ACTOR_animateTo(&player.actor);

    if (!player.actor.moving) {
      if (ACTOR_checkCollision(&player.actor)) {
        kprintf("Pawn hit");
        pawn.state = PAWN_DEAD;
      }

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

void PLAYER_destroy() {
  ACTOR_destroy(&player.actor);
}

void PLAYER_update() {
  if (turn == ENEMY || player.state == PLAYER_DEAD)
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
  ACTOR_init(&player.actor, sprite, palette, x, y, COLLISION_TYPE_PLAYER);
  JOY_setEventHandler(inputHandler);
  player.state = PLAYER_IDLE;
  player.cursor = (Vect2D_s16){x, y - 2};
}

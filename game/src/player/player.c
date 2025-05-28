#include "player/player.h"
#include "global.h"
#include "map/map.h"
#include "node/actor.h"
#include "scene/scene_manager.h"
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

static u16 prevJoyState = 0;

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

static void inputHandler() {
  const u16 state = JOY_readJoypad(JOY_1);
  const u16 pressed = (state ^ prevJoyState) & state;
  const u16 directional = pressed & BUTTON_DIR;
  const u16 command = pressed & BUTTON_A;

  if (command) {
    cursorInertia();
    player.state = PLAYER_MOVING;
    prevJoyState = state;
    return;
  }

  if (directional) {
    s16 x = player.cursor.x;
    s16 y = player.cursor.y;
    for (u8 i = 0; i < 4; i++) {
      if (cursorMoves[i].direction & pressed) {
        x += cursorMoves[i].x;
        y += cursorMoves[i].y;
        handleCursorPos(x, y, cursorMoves[i].direction);
        break;
      }
    }
  }

  prevJoyState = state;
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

inline static s8 callAnimation() {
  if (frame % FRAME_ANIMATION == 0) {
    ACTOR_animateTo(&player.actor);

    if (!player.actor.moving) {
      if (ACTOR_checkCollision(&player.actor)) {
        kprintf("Pawn hit");
        sceneManager[sceneIndex]->hit(player.actor.collisionCurPos);
      }

      player.state = PLAYER_IDLE;
      return 0;
    }
  }

  return 1;
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

void PLAYER_destroy() { ACTOR_destroy(&player.actor); }

s8 PLAYER_update() {
  if (player.state == PLAYER_DAMAGED)
    ACTOR_blink(&player.actor);
  else
    ACTOR_setVisible(&player.actor);

  if (player.state == PLAYER_DEAD)
    return 0;

  if (player.state == PLAYER_MOVING) {
    return callAnimation();
  }

  inputHandler();
  updateSelectTile();
  updateCursorTile();
  return 1;
}

void PLAYER_levelInit(const SpriteDefinition *sprite, u16 palette, s16 x,
                      s16 y) {
  ACTOR_init(&player.actor, sprite, palette, x, y, COLLISION_TYPE_PLAYER);
  player.cursor = (Vect2D_s16){x, y - 2};
}

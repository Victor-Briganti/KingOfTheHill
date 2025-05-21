#include "scene/scene1.h"
#include "background/background.h"
#include "enemy/pawn.h"
#include "global.h"
#include "hud/heart.h"
#include "map/map.h"
#include "maths.h"
#include "player/player.h"
#include "scene/scene_manager.h"
#include "sprites.h"
#include "tilemap/tilemap.h"

Scene scene1 = {SCENE1_init, SCENE1_update, SCENE1_hitEnemy, SCENE1_destroy};

// Total enemies on this scene
#define MAX_ENEMIES 3

// Player initial position
#define PLAYER_SCENE1_X_POS (6)  /* In Tile */
#define PLAYER_SCENE1_Y_POS (14) /* In Tile */

// Array with every enemy of the scene
Pawn pawns[MAX_ENEMIES];

// Defines the initial position of every enemy
static const Vect2D_s16 ENEMIES_POS[MAX_ENEMIES] = {{6, 0}, {2, 0}, {10, 0}};

// Defines the index of the current enemy
static u8 indexEnemy = 0;

// Total number of current enemies still alive
static u8 totalEnemies = MAX_ENEMIES;

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

static inline void SCENE1_initGlobals() {
  mapLevelHeight = MAP_SCENE1_HEIGHT;
  mapLevelWidth = MAP_SCENE1_WIDTH;

  mapLevelX = MAP_SCENE1_X_POS;
  mapLevelY = MAP_SCENE1_Y_POS;

  playerInitX = PLAYER_SCENE1_X_POS;
  playerInitY = PLAYER_SCENE1_Y_POS;

  turn = PLAYER;
}

static inline void SCENE1_initBackground() {
  BACKGROUND_init();
  TILEMAP_init(&tileset);
  MAP_initLevel(mapLevelHeight, mapLevelWidth);
}

static inline void SCENE1_initPlayer() {
  PLAYER_levelInit(&goblin_sprite1, PLAYER_PAL, playerInitX, playerInitY);
}

static inline void SCENE1_initEnemies() {
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    PAWN_init(&pawns[i], &pawn_sprite1, ENEMY_PAL, ENEMIES_POS[i].x,
              ENEMIES_POS[i].y);

    MAP_updateCollision(pawns[i].actor.collisionPrevPos,
                        pawns[i].actor.collisionCurPos,
                        pawns[i].actor.collisionType);
  }
}

static inline void SCENE1_updateMapCollision() {
  MAP_updateCollision(player.actor.collisionPrevPos,
                      player.actor.collisionCurPos, player.actor.collisionType);

  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (pawns[i].state != PAWN_DEAD && pawns[i].state != PAWN_DESTROYED)
      MAP_updateCollision(pawns[i].actor.collisionPrevPos,
                          pawns[i].actor.collisionCurPos,
                          pawns[i].actor.collisionType);
  }
}

static inline void SCENE1_updateBackground() {
  BACKGROUND_setText("LEVEL 1-1");
  BACKGROUND_setScore(0);
  TILEMAP_update(&level_map1);
}

static inline void SCENE1_updatePlayer() { PLAYER_update(); }

static inline void SCENE1_updateEnemies() {
  u8 id = indexEnemy;
  u8 res = -1;
  u8 tried = 0;

  while (tried < MAX_ENEMIES) {

    if (pawns[id].state != PAWN_DEAD && pawns[id].state != PAWN_DESTROYED) {
      res = PAWN_update(&pawns[id]);
      indexEnemy = id;
      break;
    }

    // Try the next enemy
    id = (id + 1) % MAX_ENEMIES;
    tried++;
  }

  // Enemy finished animation, go to the next one
  if (res == 0) {
    indexEnemy = (indexEnemy + 1) % MAX_ENEMIES;
  }
}

static inline void SCENE1_destroyPlayer() {
  PLAYER_destroy();
  HEART_update();

  if (player.health > 0)
    player.state = PLAYER_DAMAGED;
}

static inline void SCENE1_destroyEnemies() {
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (pawns[i].state == PAWN_DEAD) {
      PAWN_deallocDestroy(&pawns[i]);
      totalEnemies--;
    }
  }
}

static inline void SCENE1_restartEnemies() {
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (pawns[i].state == PAWN_DEAD || pawns[i].state == PAWN_DESTROYED)
      continue;

    PAWN_init(&pawns[i], &pawn_sprite1, ENEMY_PAL, ENEMIES_POS[i].x,
              ENEMIES_POS[i].y);
  }
}

static inline void SCENE1_restart() {
  SCENE1_initPlayer();
  SCENE1_restartEnemies();
  SCENE1_updateMapCollision();
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE1_init() {
  SCENE1_initGlobals();
  SCENE1_initBackground();
  SCENE1_initPlayer();
  SCENE1_initEnemies();
  SYS_doVBlankProcess();
}

SceneId SCENE1_update() {
  SCENE1_updateBackground();
  SCENE1_updateMapCollision();
  SCENE1_updatePlayer();
  SCENE1_updateEnemies();
  SPR_update();
  SYS_doVBlankProcess();

  SCENE1_destroyEnemies();
  if (totalEnemies == 0)
    return SCENE_ID_PASSED;

  if (player.state == PLAYER_DEAD) {
    SCENE1_destroyPlayer();

    if (player.health == 0) {
      return SCENE_ID_GAME_OVER;
    } else {
      // Restart the scene
      SCENE1_destroy();
      SCENE1_restart();
    }
  }

  return SCENE_ID_LEVEL01;
}

void SCENE1_hitEnemy(const Vect2D_s16 hitPos) {
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (pawns[i].state == PAWN_DEAD || pawns[i].state == PAWN_DESTROYED)
      continue;

    if (pawns[i].actor.collisionCurPos.x == hitPos.x &&
        pawns[i].actor.collisionCurPos.y == hitPos.y) {
      pawns[i].state = PAWN_DEAD;
    }
  }
}

void SCENE1_destroy() {
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (pawns[i].state != PAWN_DESTROYED)
      PAWN_dealloc(&pawns[i]);
  }

  SYS_doVBlankProcess();
}

#include "scene/scene2.h"
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

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Total enemies on this scene
#define MAX_ENEMIES 1

// Player initial position
#define PLAYER_SCENE1_X_POS (6)  /* In Tile */
#define PLAYER_SCENE1_Y_POS (14) /* In Tile */

typedef enum GameTurn {
  PLAYER = 0,
  ENEMY = 1,
  GAME_OVER = 2,
} GameTurn;

typedef struct SceneContext {
  // Array with every enemy of the scene
  Pawn pawns[MAX_ENEMIES];

  // Defines the initial position of every enemy
  const Vect2D_s16 enemiesPos[MAX_ENEMIES];

  // Defines whose turn it is
  GameTurn turn;

  // Defines the index of the current enemy
  u8 indexEnemy;

  // Total number of current enemies still alive
  u8 totalEnemies;
} SceneContext;

//===----------------------------------------------------------------------===//
// GLOBALS
//===----------------------------------------------------------------------===//

Scene scene2 = {SCENE2_init, SCENE2_update, SCENE2_hitEnemy, SCENE2_destroy};

static SceneContext context = {
    .turn = PLAYER,
    .enemiesPos = {{6, 0}},
    .indexEnemy = 0,
    .totalEnemies = MAX_ENEMIES,
};

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

static inline void initGlobals() {
  mapLevelHeight = MAP_SCENE2_HEIGHT;
  mapLevelWidth = MAP_SCENE2_WIDTH;

  mapLevelX = MAP_SCENE2_X_POS;
  mapLevelY = MAP_SCENE2_Y_POS;

  playerInitX = PLAYER_SCENE1_X_POS;
  playerInitY = PLAYER_SCENE1_Y_POS;
}

static inline void initBackground() {
  BACKGROUND_init();
  TILEMAP_init(&tileset);
  MAP_initLevel(mapLevelHeight, mapLevelWidth);
}

static inline void initPlayer() {
  HEART_update();
  PLAYER_levelInit(&goblin_sprite1, PLAYER_PAL, playerInitX, playerInitY);
}

static inline void initEnemies() {}

static inline void updateMapCollision() {
  MAP_updateCollision(player.actor.collisionPrevPos,
                      player.actor.collisionCurPos, player.actor.collisionType);
}

static inline void updateBackground() {
  BACKGROUND_setText("LEVEL 1-2");
  BACKGROUND_setScore(0);
  TILEMAP_update(&level_map1);
}

static inline void updatePlayer() {
  s8 res = PLAYER_update();
  if (res)
    return;

  //   context.turn = ENEMY;
}

static inline void updateEnemies() {}

static inline void destroyPlayer() {
  PLAYER_destroy();
  HEART_update();

  if (player.health > 0)
    player.state = PLAYER_DAMAGED;
}

static inline void destroyEnemies() {}

static inline void restartEnemies() {}

static inline void restart() {
  SCENE2_destroy();
  initPlayer();
  restartEnemies();
  updateMapCollision();
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE2_init() {
  initGlobals();
  initBackground();
  initPlayer();
  initEnemies();
  SYS_doVBlankProcess();
}

SceneId SCENE2_update() {
  updateBackground();
  updateMapCollision();
  if (context.turn == PLAYER)
    updatePlayer();
  else
    updateEnemies();

  SPR_update();
  SYS_doVBlankProcess();

  destroyEnemies();
  if (context.totalEnemies == 0)
    return SCENE_ID_PASSED;

  if (player.state == PLAYER_DEAD) {
    destroyPlayer();

    if (player.health == 0)
      return SCENE_ID_GAME_OVER;
    else
      restart();
  }

  return SCENE_ID_LEVEL02;
}

void SCENE2_hitEnemy(const Vect2D_s16 hitPos) {}

void SCENE2_destroy() { SYS_doVBlankProcess(); }

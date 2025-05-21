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

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Total enemies on this scene
#define MAX_ENEMIES 3

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

Scene scene1 = {SCENE1_init, SCENE1_update, SCENE1_hitEnemy,SCENE1_destroy};

static SceneContext context = {
    .turn = PLAYER,
    .enemiesPos = {{2, 0}, {6, 0}, {10, 0}},
    .indexEnemy = 0,
    .totalEnemies = MAX_ENEMIES,
};

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

static inline void initGlobals() {
  mapLevelHeight = MAP_SCENE1_HEIGHT;
  mapLevelWidth = MAP_SCENE1_WIDTH;

  mapLevelX = MAP_SCENE1_X_POS;
  mapLevelY = MAP_SCENE1_Y_POS;

  playerInitX = PLAYER_SCENE1_X_POS;
  playerInitY = PLAYER_SCENE1_Y_POS;
}

static inline void initBackground() {
  BACKGROUND_init();
  TILEMAP_init(&tileset);
  MAP_initLevel(mapLevelHeight, mapLevelWidth);
}

static inline void initPlayer() {
  PLAYER_levelInit(&goblin_sprite1, PLAYER_PAL, playerInitX, playerInitY);
}

static inline void initEnemies() {
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    PAWN_init(&context.pawns[i], &pawn_sprite1, ENEMY_PAL,
              context.enemiesPos[i].x, context.enemiesPos[i].y);

    MAP_updateCollision(context.pawns[i].actor.collisionPrevPos,
                        context.pawns[i].actor.collisionCurPos,
                        context.pawns[i].actor.collisionType);
  }
}

static inline void updateMapCollision() {
  MAP_updateCollision(player.actor.collisionPrevPos,
                      player.actor.collisionCurPos, player.actor.collisionType);

  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (context.pawns[i].state != PAWN_DEAD &&
        context.pawns[i].state != PAWN_DESTROYED)
      MAP_updateCollision(context.pawns[i].actor.collisionPrevPos,
                          context.pawns[i].actor.collisionCurPos,
                          context.pawns[i].actor.collisionType);
  }
}

static inline void updateBackground() {
  BACKGROUND_setText("LEVEL 1-1");
  BACKGROUND_setScore(0);
  TILEMAP_update(&level_map1);
}

static inline void updatePlayer() { 
  s8 res = PLAYER_update(); 
  if (res)
    return;

  context.turn = ENEMY;
}

static inline void updateEnemies() {
  u8 id = context.indexEnemy;
  u8 res = -1;
  u8 tried = 0;

  while (tried < MAX_ENEMIES) {

    if (context.pawns[id].state != PAWN_DEAD &&
        context.pawns[id].state != PAWN_DESTROYED) {
      res = PAWN_update(&context.pawns[id]);
      context.indexEnemy = id;
      break;
    }

    // Try the next enemy
    id = (id + 1) % MAX_ENEMIES;
    tried++;
  }

  // Enemy finished animation, go to the next one
  if (res == 0) {
    context.indexEnemy = (context.indexEnemy + 1) % MAX_ENEMIES;
    context.turn = PLAYER;
  }
}

static inline void destroyPlayer() {
  PLAYER_destroy();
  HEART_update();

  if (player.health > 0)
    player.state = PLAYER_DAMAGED;
}

static inline void destroyEnemies() {
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (context.pawns[i].state == PAWN_DEAD) {
      PAWN_deallocDestroy(&context.pawns[i]);
      context.totalEnemies--;
    }
  }
}

static inline void restartEnemies() {
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (context.pawns[i].state == PAWN_DEAD ||
        context.pawns[i].state == PAWN_DESTROYED)
      continue;

    PAWN_init(&context.pawns[i], &pawn_sprite1, ENEMY_PAL,
              context.enemiesPos[i].x, context.enemiesPos[i].y);
  }
}

static inline void restart() {
  SCENE1_destroy();
  initPlayer();
  restartEnemies();
  updateMapCollision();
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE1_init() {
  initGlobals();
  initBackground();
  initPlayer();
  initEnemies();
  SYS_doVBlankProcess();
}

SceneId SCENE1_update() {
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

  return SCENE_ID_LEVEL01;
}

void SCENE1_hitEnemy(const Vect2D_s16 hitPos) {
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (context.pawns[i].state == PAWN_DEAD ||
        context.pawns[i].state == PAWN_DESTROYED)
      continue;

    if (context.pawns[i].actor.collisionCurPos.x == hitPos.x &&
        context.pawns[i].actor.collisionCurPos.y == hitPos.y) {
      context.pawns[i].state = PAWN_DEAD;
    }
  }
}

void SCENE1_destroy() {
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (context.pawns[i].state != PAWN_DESTROYED)
      PAWN_dealloc(&context.pawns[i]);
  }

  SYS_doVBlankProcess();
}

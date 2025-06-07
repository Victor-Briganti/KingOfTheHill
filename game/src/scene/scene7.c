#include "scene/scene7.h"
#include "background/background.h"
#include "enemy/enemy.h"
#include "global.h"
#include "hud/heart.h"
#include "map/map.h"
#include "maths.h"
#include "node/actor.h"
#include "player/player.h"
#include "scene/scene_manager.h"
#include "sprites.h"
#include "tilemap/tilemap.h"

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Total enemies on this scene
#define MAX_ENEMIES 8

// Player initial position
#define PLAYER_SCENE7_X_POS (6)  /* In Tile */
#define PLAYER_SCENE7_Y_POS (14) /* In Tile */

typedef enum GameTurn {
  PLAYER = 0,
  ENEMY = 1,
  GAME_OVER = 2,
} GameTurn;

typedef struct SceneContext {
  // Array with every enemy of the scene
  Enemy enemies[MAX_ENEMIES];

  // Array with every enemy type
  EnemyType enemiesType[MAX_ENEMIES];

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

Scene scene7 = {SCENE7_init, SCENE7_update, SCENE7_hit, SCENE7_destroy};

static SceneContext context = {
    .turn = PLAYER,
    .enemiesType = {BISHOP_TYPE, BISHOP_TYPE, TOWER_TYPE, TOWER_TYPE, PAWN_TYPE,
                    PAWN_TYPE, PAWN_TYPE},
    .enemiesPos =
        {{4, 0}, {10, 0}, {2, 0}, {12, 0}, {2, 2}, {0, 2}, {4, 4}, {10, 2}},
    .indexEnemy = 0,
    .totalEnemies = MAX_ENEMIES,
};

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

static inline void initGlobals() {
  mapLevelHeight = MAP_SCENE7_HEIGHT;
  mapLevelWidth = MAP_SCENE7_WIDTH;

  mapLevelX = MAP_SCENE7_X_POS;
  mapLevelY = MAP_SCENE7_Y_POS;

  playerInitX = PLAYER_SCENE7_X_POS;
  playerInitY = PLAYER_SCENE7_Y_POS;
}

static inline void initBackground() {
  BACKGROUND_init();
  TILEMAP_init(&tileset);
  MAP_initLevel(mapLevelHeight, mapLevelWidth);
}

static inline void initPlayer() {
  PLAYER_levelInit(&goblin_sprite1, PLAYER_PAL, playerInitX, playerInitY);
  MAP_updateCollision(player.actor.collisionPrevPos,
                      player.actor.collisionCurPos, player.actor.collisionType);
}

static inline void initEnemies() {
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    ENEMY_init(&context.enemies[i], context.enemiesType[i],
               context.enemiesPos[i].x, context.enemiesPos[i].y);
    MAP_updateCollision(context.enemies[i].actor.collisionPrevPos,
                        context.enemies[i].actor.collisionCurPos,
                        context.enemies[i].actor.collisionType);
  }
}

static inline void updateBackground() {
  BACKGROUND_setText("LEVEL 3-1");
  BACKGROUND_setScore(0);
  TILEMAP_update(&level_map1);
}

static inline void updatePlayer() {
  if (PLAYER_update())
    return;

  context.turn = ENEMY;
}

static inline void updateEnemies() {
  // First check if any enemy is currently moving
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (context.enemies[i].state == ENEMY_MOVING ||
        context.enemies[i].state == ENEMY_ANIMATING) {
      const s8 res = context.enemies[i].update(&context.enemies[i]);
      if (res == 0) {
        context.turn = PLAYER;
        context.indexEnemy = (i + 1) % MAX_ENEMIES;
      }
      return;
    }
  }

  // No enemies are moving - try attacks first
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    u8 id = (context.indexEnemy + i) % MAX_ENEMIES;
    if (context.enemies[id].state != ENEMY_DEAD &&
        context.enemies[id].state != ENEMY_DESTROYED) {
      context.enemies[id].state = ENEMY_ATTACKING;
      const s8 res = context.enemies[id].update(&context.enemies[id]);
      if (res > 0) {
        context.indexEnemy = id;
        return;
      }
    }
  }

  // No attacks possible - try to move one enemy
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    u8 id = (context.indexEnemy + i) % MAX_ENEMIES;
    if (context.enemies[id].state != ENEMY_DEAD &&
        context.enemies[id].state != ENEMY_DESTROYED) {
      context.enemies[id].state = ENEMY_IDLE;
      const s8 res = context.enemies[id].update(&context.enemies[id]);
      if (res > 0) {
        context.indexEnemy = id;
        return;
      }
    }
  }

  // No attacks or moves possible - turn ends
  context.turn = PLAYER;
  context.indexEnemy = (context.indexEnemy + 1) % MAX_ENEMIES;
}

static inline void damagePlayer() {
  player.health--;
  PLAYER_destroy();
  HEART_update();

  if (player.health > 0)
    player.state = PLAYER_DAMAGED;
}

static inline void destroyEnemies() {
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (context.enemies[i].state == ENEMY_DEAD) {
      context.enemies[i].destroy(&context.enemies[i]);
      context.totalEnemies--;
    }
  }
}

static inline void restartEnemies() {
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (context.enemies[i].state == ENEMY_DEAD ||
        context.enemies[i].state == ENEMY_DESTROYED)
      continue;

    context.enemies[i].dealloc(&context.enemies[i]);
    ENEMY_init(&context.enemies[i], context.enemiesType[i],
               context.enemiesPos[i].x, context.enemiesPos[i].y);
    MAP_updateCollision(context.enemies[i].actor.collisionPrevPos,
                        context.enemies[i].actor.collisionCurPos,
                        context.enemies[i].actor.collisionType);
  }
}

static inline void restart() {
  MAP_initLevel(mapLevelHeight, mapLevelWidth);
  initPlayer();
  restartEnemies();
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE7_init() {
  player.health++;
  if (player.health > player.totalHealth) {
    player.health = player.totalHealth;
  }
  HEART_update();

  initGlobals();
  initBackground();
  initPlayer();
  initEnemies();
  SYS_doVBlankProcess();
}

SceneId SCENE7_update() {
  updateBackground();
  if (context.turn == PLAYER)
    updatePlayer();
  else
    updateEnemies();

  SPR_update();
  SYS_doVBlankProcess();

  destroyEnemies();
  if (context.totalEnemies == 0)
    return SCENE_ID_LEVEL08;

  if (player.state == PLAYER_DEAD) {
    damagePlayer();

    if (player.health == 0)
      return SCENE_ID_GAME_OVER;

    restart();
  }

  return SCENE_ID_LEVEL07;
}

void SCENE7_hit(const Vect2D_s16 hitPos) {
  if (context.turn == ENEMY) {
    if (player.actor.collisionCurPos.x == hitPos.x &&
        player.actor.collisionCurPos.y == hitPos.y) {
      player.state = PLAYER_DEAD;
    }

    return;
  }

  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (context.enemies[i].state == ENEMY_DEAD ||
        context.enemies[i].state == ENEMY_DESTROYED)
      continue;

    if (context.enemies[i].actor.collisionCurPos.x == hitPos.x &&
        context.enemies[i].actor.collisionCurPos.y == hitPos.y) {
      context.enemies[i].state = ENEMY_DEAD;
    }
  }
}

void SCENE7_destroy() {
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (context.enemies[i].state != ENEMY_DESTROYED)
      context.enemies[i].destroy(&context.enemies[i]);
  }

  // Destroy Player
  PLAYER_destroy();
  HEART_update();

  SYS_doVBlankProcess();
}

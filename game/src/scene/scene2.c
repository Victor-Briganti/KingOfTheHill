#include "scene/scene2.h"
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
#define MAX_ENEMIES 5

// Player initial position
#define PLAYER_SCENE2_X_POS (6)  /* In Tile */
#define PLAYER_SCENE2_Y_POS (14) /* In Tile */

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

Scene scene2 = {SCENE2_init, SCENE2_update, SCENE2_hit, SCENE2_destroy};

static SceneContext context = {
    .turn = PLAYER,
    .enemiesType = {TOWER_TYPE, PAWN_TYPE, PAWN_TYPE, PAWN_TYPE, PAWN_TYPE},
    .enemiesPos = {{6, 0}, {2, 0}, {0, 0}, {10, 0}, {12, 0}},
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

  playerInitX = PLAYER_SCENE2_X_POS;
  playerInitY = PLAYER_SCENE2_Y_POS;
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
    ENEMY_init(&context.enemies[i], context.enemiesType[i],
               context.enemiesPos[i].x, context.enemiesPos[i].y);
    MAP_updateCollision(context.enemies[i].actor.collisionPrevPos,
                        context.enemies[i].actor.collisionCurPos,
                        context.enemies[i].actor.collisionType);
  }
}

static inline void updateMapCollision() {
  MAP_updateCollision(player.actor.collisionPrevPos,
                      player.actor.collisionCurPos, player.actor.collisionType);

  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (context.enemies[i].state != ENEMY_DEAD &&
        context.enemies[i].state != ENEMY_DESTROYED)
      MAP_updateCollision(context.enemies[i].actor.collisionPrevPos,
                          context.enemies[i].actor.collisionCurPos,
                          context.enemies[i].actor.collisionType);
  }
}

static inline void updateBackground() {
  BACKGROUND_setText("LEVEL 1-2");
  BACKGROUND_setScore(0);
  TILEMAP_update(&level_map1);
}

static inline void updatePlayer() {
  if (PLAYER_update())
    return;

  context.turn = ENEMY;
}

static inline void updateEnemies() {
  u8 id = context.indexEnemy;
  s8 res = -1;
  u8 tried = 0;

  while (tried < MAX_ENEMIES) {
    if (context.enemies[id].state != ENEMY_DEAD &&
        context.enemies[id].state != ENEMY_DESTROYED) {
      res = context.enemies[id].update(&context.enemies[id]);
      context.indexEnemy = id;
    }

    // Try the next enemy
    id = (id + 1) % MAX_ENEMIES;
    tried++;
    if (res >= 0) {
      break;
    }
  }

  // Enemy finished animation, go to the next one
  if (res == 0) {
    context.indexEnemy = (context.indexEnemy + 1) % MAX_ENEMIES;
    context.turn = PLAYER;
  }
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
  }
}

static inline void restart() {
  initPlayer();
  restartEnemies();
  MAP_initLevel(mapLevelHeight, mapLevelWidth);
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
    return SCENE_ID_LEVEL03;

  if (player.state == PLAYER_DEAD) {
    damagePlayer();

    if (player.health == 0)
      return SCENE_ID_GAME_OVER;

    restart();
  }

  return SCENE_ID_LEVEL02;
}

void SCENE2_hit(const Vect2D_s16 hitPos) {
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

void SCENE2_destroy() {
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (context.enemies[i].state != ENEMY_DESTROYED)
      context.enemies[i].destroy(&context.enemies[i]);
  }

  // Destroy Player
  PLAYER_destroy();
  HEART_update();

  SYS_doVBlankProcess();
}

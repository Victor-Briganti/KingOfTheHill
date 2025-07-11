#include "scene/scene4.h"
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
#define MAX_ENEMIES 7

// Player initial position
#define PLAYER_SCENE4_X_POS (6)  /* In Tile */
#define PLAYER_SCENE4_Y_POS (14) /* In Tile */

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

Scene scene4 = {SCENE4_init, SCENE4_update, SCENE4_hit, SCENE4_destroy};

static SceneContext context = {
    .turn = PLAYER,
    .enemiesType = {TOWER_TYPE, TOWER_TYPE, TOWER_TYPE, PAWN_TYPE, PAWN_TYPE,
                    PAWN_TYPE, PAWN_TYPE},
    .enemiesPos = {{6, 0}, {0, 6}, {10, 4}, {2, 0}, {0, 2}, {10, 0}, {12, 2}},
    .indexEnemy = 0,
    .totalEnemies = MAX_ENEMIES,
};

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

static inline void initGlobals() {
  mapLevelHeight = MAP_SCENE4_HEIGHT;
  mapLevelWidth = MAP_SCENE4_WIDTH;

  mapLevelX = MAP_SCENE4_X_POS;
  mapLevelY = MAP_SCENE4_Y_POS;

  playerInitX = PLAYER_SCENE4_X_POS;
  playerInitY = PLAYER_SCENE4_Y_POS;
}

static inline void initTransition() {
  VDP_init();
  BACKGROUND_initTransition(&level2_1_transition);
  MAP_initLevel(mapLevelHeight, mapLevelWidth);
}

static inline void initBackground() {
  // Release the background level transition
  BACKGROUND_release();

  // Init the scene background
  BACKGROUND_init();
  TILEMAP_init(&tileset);

  HEART_draw();
}

static inline void initPlayer() {
  PLAYER_levelInit(&demon_sprite, PLAYER_PAL, playerInitX, playerInitY);
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
  BACKGROUND_setText("LEVEL 2-1", LEVEL_TEXT_X, LEVEL_TEXT_Y);
  
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

static inline bool loadingScene() {
  static u16 count = 0;
  static bool loading = TRUE;

  if (count < 4096) {
    if (frame % FRAME_ANIMATION == 0) {
      count++;
      if (count == 4096) {
        initBackground();
        initPlayer();
        initEnemies();
        loading = FALSE;
      }
    }
  }

  return loading;
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE4_init() {
  player.health++;
  if (player.health > player.totalHealth) {
    player.health = player.totalHealth;
  }

  initGlobals();
  initTransition();
  SYS_doVBlankProcess();
}

SceneId SCENE4_update() {
  if (loadingScene())
    return SCENE_ID_LEVEL04;

  updateBackground();
  if (context.turn == PLAYER)
    updatePlayer();
  else
    updateEnemies();

  SPR_update();
  SYS_doVBlankProcess();

  destroyEnemies();
  if (context.totalEnemies == 0)
    return SCENE_ID_LEVEL05;

  if (player.state == PLAYER_DEAD) {
    damagePlayer();

    if (player.health == 0)
      return SCENE_ID_GAME_OVER;

    restart();
  }

  return SCENE_ID_LEVEL04;
}

void SCENE4_hit(const Vect2D_s16 hitPos) {
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

void SCENE4_destroy() {
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (context.enemies[i].state != ENEMY_DESTROYED)
      context.enemies[i].destroy(&context.enemies[i]);
  }

  // Destroy Player
  PLAYER_destroy();
  HEART_release();
  BACKGROUND_release();
  SPR_defragVRAM();

  SYS_doVBlankProcess();
}

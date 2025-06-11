#include "scene/scene15.h"
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
#include "types/collision.h"

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Total enemies on this scene
#define MAX_ENEMIES 9

// Player initial position
#define PLAYER_SCENE15_X_POS (6)  /* In Tile */
#define PLAYER_SCENE15_Y_POS (14) /* In Tile */

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

Scene scene15 = {SCENE15_init, SCENE15_update, SCENE15_hit, SCENE15_destroy};

static SceneContext context = {
    .turn = PLAYER,
    .enemiesType =
        {
          PAWN_TYPE,
          PAWN_TYPE,
          PAWN_TYPE,
          TOWER_TYPE,
          BISHOP_TYPE,
          KNIGHT_TYPE,
          KNIGHT_TYPE,
          QUEEN_TYPE,
          KING_TYPE,
        },
    .enemiesPos =
        {
          {6, 4},
          {4, 4},
          {10, 4},
          {0, 0},
          {2, 0},
          {0, 6},
          {12, 8},
          {12, 0},
          {6, 0},
        },
    .indexEnemy = 0,
    .totalEnemies = MAX_ENEMIES,
};

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

static inline void initGlobals() {
  mapLevelHeight = MAP_SCENE15_HEIGHT;
  mapLevelWidth = MAP_SCENE15_WIDTH;

  mapLevelX = MAP_SCENE15_X_POS;
  mapLevelY = MAP_SCENE15_Y_POS;

  playerInitX = PLAYER_SCENE15_X_POS;
  playerInitY = PLAYER_SCENE15_Y_POS;
}

static inline void initTransition() {
  VDP_init();
  BACKGROUND_initTransition(&level5_3_transition);
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
  BACKGROUND_setText("LEVEL 5-3");
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

static inline bool loadingScene() {
  static u16 count = 0;
  static bool loading = TRUE;

  if (count < 4096) {
    if (frame % 32 == 0) {
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

void SCENE15_init() {
  initGlobals();
  initTransition();
  SYS_doVBlankProcess();
}

SceneId SCENE15_update() {
  if (loadingScene())
    return SCENE_ID_LEVEL15;

  updateBackground();
  if (context.turn == PLAYER)
    updatePlayer();
  else
    updateEnemies();

  SPR_update();
  SYS_doVBlankProcess();

  destroyEnemies();
  if (context.totalEnemies == 0)
    return SCENE_ID_LEVEL15;

  if (player.state == PLAYER_DEAD) {
    damagePlayer();

    if (player.health == 0)
      return SCENE_ID_GAME_OVER;

    restart();
  }

  return SCENE_ID_LEVEL15;
}

void SCENE15_hit(const Vect2D_s16 hitPos) {
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

void SCENE15_destroy() {
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (context.enemies[i].state != ENEMY_DESTROYED)
      context.enemies[i].destroy(&context.enemies[i]);
  }

  // Release everything else
  PLAYER_destroy();
  HEART_release();
  BACKGROUND_release();
  SPR_defragVRAM();

  SYS_doVBlankProcess();
}

void SCENE15_resurrectEnemy() {
  for (u8 i = 0; i < MAX_ENEMIES; i++) {
    if (context.enemies[i].state == ENEMY_DESTROYED) {
      Vect2D_s16 vec = context.enemiesPos[i];

      if (MAP_getCollision(vec) != COLLISION_TYPE_EMPTY) {
        continue;
      }

      ENEMY_init(&context.enemies[i], context.enemiesType[i], vec.x, vec.y);
      MAP_updateCollision(context.enemies[i].actor.collisionPrevPos,
                          context.enemies[i].actor.collisionCurPos,
                          context.enemies[i].actor.collisionType);
      return;
    }
  }
}

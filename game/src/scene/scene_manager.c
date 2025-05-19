#include "scene/scene_manager.h"
#include "background/background.h"
#include "enemy/pawn.h"
#include "global.h"
#include "hud/heart.h"
#include "map/map.h"
#include "player/player.h"
#include "scene/scene1.h"
#include "sprites.h"
#include "tilemap/tilemap.h"

Scene scene = {SCENE1_init, SCENE1_update, SCENE1_destroy};

Pawn pawn;

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
  PAWN_init(&pawn, &pawn_sprite1, ENEMY_PAL, PAWN_SCENE1_X_POS,
            PAWN_SCENE1_Y_POS);
}

static inline void SCENE1_updateMapCollision() {
  MAP_updateCollision(player.actor.collisionPrevPos,
                      player.actor.collisionCurPos, player.actor.collisionType);
  MAP_updateCollision(pawn.actor.collisionPrevPos, pawn.actor.collisionCurPos,
                      pawn.actor.collisionType);
}

static inline void SCENE1_updateBackground() {
  BACKGROUND_setText("LEVEL 1-1");
  BACKGROUND_setScore(0);
  TILEMAP_update(&level_map1);
}

static inline void SCENE1_updatePlayer() { PLAYER_update(); }

static inline void SCENE1_updateEnemies() { PAWN_update(&pawn); }

static inline void SCENE1_destroyPlayer() {
  PLAYER_destroy();
  PAWN_destroy(&pawn);
  HEART_update();
}

static inline void SCENE1_destroyEnemies() {
  PAWN_destroy(&pawn);
  turn = PLAYER;
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

s8 SCENE1_update() {
  SCENE1_updateBackground();
  SCENE1_updateMapCollision();
  SCENE1_updatePlayer();
  SCENE1_updateEnemies();
  SPR_update();
  SYS_doVBlankProcess();

  if (player.state == PLAYER_DEAD) {
    SCENE1_destroyPlayer();
    return -1;
  }

  if (pawn.state == PAWN_DEAD) {
    SCENE1_destroyEnemies();
    return 1;
  }

  return 0;
}

void SCENE1_hitEnemy() { pawn.state = PAWN_DEAD; }

void SCENE1_destroy() {}

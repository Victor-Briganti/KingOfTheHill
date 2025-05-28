#include "scene/scene2.h"
#include "background/background.h"
#include "enemy/enemy.h"
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
// #define MAX_ENEMIES 1

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
    Enemy enemy;

    // Defines the initial position of every enemy
    const Vect2D_s16 enemiesPos;

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
    .enemiesPos = {6, 0},
    .indexEnemy = 0,
    .totalEnemies = 1,
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

static inline void initEnemies() {
    ENEMY_init(&context.enemy, TOWER_TYPE, context.enemiesPos.x, context.enemiesPos.y);
    MAP_updateCollision(context.enemy.actor.collisionPrevPos,
                        context.enemy.actor.collisionCurPos,
                        context.enemy.actor.collisionType);
}

static inline void updateMapCollision() {
    MAP_updateCollision(player.actor.collisionPrevPos,
                        player.actor.collisionCurPos, player.actor.collisionType);
    MAP_updateCollision(context.enemy.actor.collisionPrevPos,
                        context.enemy.actor.collisionCurPos,
                        context.enemy.actor.collisionType);
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
    u8 res = -1;

    res = context.enemy.update(&context.enemy);
    if (res == 0)
        context.turn = PLAYER;
}

static inline void damagePlayer() {
    player.health--;
    PLAYER_destroy();
    HEART_update();

    if (player.health > 0)
        player.state = PLAYER_DAMAGED;
}

static inline void destroyEnemies() {
    if (context.enemy.state == ENEMY_DEAD) {
        context.enemy.destroy(&context.enemy);
        context.totalEnemies--;
    }
}

static inline void restartEnemies() {
    ENEMY_init(&context.enemy, TOWER_TYPE, context.enemiesPos.x, context.enemiesPos.y);
}

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
        damagePlayer();

        if (player.health == 0)
            return SCENE_ID_GAME_OVER;

        restart();
    }

    return SCENE_ID_LEVEL02;
}

void SCENE2_hit(const Vect2D_s16 hitPos) {
    if (context.turn == ENEMY) {
        if (player.actor.collisionCurPos.x == hitPos.x && player.actor.collisionCurPos.y == hitPos.y) {
            player.state = PLAYER_DEAD;
        }

        return;
    }

    if (context.enemy.state == ENEMY_DEAD ||
       context.enemy.state == ENEMY_DESTROYED)
        return;

    if (context.enemy.actor.collisionCurPos.x == hitPos.x &&
        context.enemy.actor.collisionCurPos.y == hitPos.y) {
        context.enemy.state = ENEMY_DEAD;
    }
}

void SCENE2_destroy() {
    if (context.enemy.state != ENEMY_DESTROYED)
        context.enemy.dealloc(&context.enemy);

    // Destroy Player
    PLAYER_destroy();
    HEART_update();

    SYS_doVBlankProcess();
}

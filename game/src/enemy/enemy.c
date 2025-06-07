#include "enemy/enemy.h"
#include "enemy/bishop.h"
#include "enemy/pawn.h"
#include "enemy/queen.h"
#include "enemy/tower.h"
#include "enemy/knight.h"

#include <sprites.h>

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

static void defaultDealloc(const Enemy *const enemy) {
  ACTOR_deallocSprite(&enemy->actor);
}

static void defaultDestroy(Enemy *const enemy) {
  enemy->state = ENEMY_DESTROYED;
  ACTOR_destroy(&enemy->actor);
}

static void actorInit(Enemy *enemy, const EnemyType type, const s16 x,
                      const s16 y) {
  switch (type) {
  case PAWN_TYPE: {
    ACTOR_init(&enemy->actor, &pawn_sprite1, ENEMY_PAL, x, y,
               COLLISION_TYPE_PAWN);
    return;
  }
  case QUEEN_TYPE: {
    ACTOR_init(&enemy->actor, &queen_sprite, ENEMY_PAL, x, y,
               COLLISION_TYPE_QUEEN);
    return;
  }
  case TOWER_TYPE: {
    ACTOR_init(&enemy->actor, &tower_sprite, ENEMY_PAL, x, y,
               COLLISION_TYPE_TOWER);
    return;
  }
  case BISHOP_TYPE: {
    ACTOR_init(&enemy->actor, &bishop_sprite, ENEMY_PAL, x, y,
               COLLISION_TYPE_BISHOP);
    return;
  }
  case KNIGHT_TYPE: {
    ACTOR_init(&enemy->actor, &knight_sprite, ENEMY_PAL, x, y,
               COLLISION_TYPE_KNIGHT);
    return;
  }
  default:
    kprintf("[%s:%d]Enemy not defined", __FILE__, __LINE__);
  }
}

static void functionsInit(Enemy *enemy, const EnemyType type) {
  enemy->dealloc = defaultDealloc;
  enemy->destroy = defaultDestroy;

  switch (type) {
  case QUEEN_TYPE: {
    enemy->update = QUEEN_update;
    return;
  }
  case PAWN_TYPE: {
    enemy->update = PAWN_update;
    return;
  }
  case TOWER_TYPE: {
    enemy->update = TOWER_update;
    return;
  }
  case BISHOP_TYPE: {
    enemy->update = BISHOP_update;
    return;
  }
  case KNIGHT_TYPE: {
    enemy->update = KNIGHT_update;
    return;
  }
  default: {
    kprintf("[%s:%d]Enemy not defined", __FILE__, __LINE__);
  }
  }
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void ENEMY_init(Enemy *enemy, const EnemyType type, const s16 x, const s16 y) {
  enemy->state = ENEMY_IDLE;
  enemy->indexSprite = 0;
  enemy->type = type;
  actorInit(enemy, type, x, y);
  functionsInit(enemy, type);
}

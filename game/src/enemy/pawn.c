#include "enemy/pawn.h"
#include "global.h"
#include "node/actor.h"
#include "player/player.h"
#include "scene/scene_manager.h"
#include "sprites.h"

#include <genesis.h>

#define MAX_SPRITES_ANIM (7)

static const SpriteDefinition *sprites[MAX_SPRITES_ANIM] = {
    &pawn_sprite1, &pawn_sprite2, &pawn_sprite3, &pawn_sprite4,
    &pawn_sprite5, &pawn_sprite6, &pawn_sprite7,
};

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

inline static s8 startMovement(Pawn *pawn) {
  s16 x = pawn->actor.collisionCurPos.x;
  s16 y = clamp(pawn->actor.collisionCurPos.y + 2, 0, mapLevelHeight - 2);

  if (map[y][x] != COLLISION_TYPE_EMPTY) {
    return 0;
  }

  // Player on diagonal right
  if ((map[y][clamp(x + 2, 0, mapLevelHeight - 2)] != 0) &&
      (map[y][clamp(x + 2, 0, mapLevelHeight - 2)] & ~COLLISION_TYPE_PLAYER) ==
          0)
    x = clamp(x + 2, 0, mapLevelHeight - 2);

  // Player on diagonal left
  if ((map[y][clamp(x - 2, 0, mapLevelHeight - 2)] != 0) &&
      (map[y][clamp(x - 2, 0, mapLevelHeight - 2)] & ~COLLISION_TYPE_PLAYER) ==
          0)
    x = clamp(x - 2, 0, mapLevelHeight - 2);

  ACTOR_setTargetAnimPos(&pawn->actor, x, y);
  pawn->state = PAWN_MOVING;

  return 1;
}

inline static s8 moveAnimation(Pawn *pawn) {
  if (frame % FRAME_ANIMATION == 0) {
    ACTOR_animateTo(&pawn->actor);

    if (!pawn->actor.moving) {
      if (ACTOR_checkCollision(&pawn->actor)) {
        kprintf("Player hit");
        player.health--;
        player.state = PLAYER_DEAD;
        return 0;
      }

      if (pawn->actor.collisionCurPos.y == mapLevelHeight - 2) {
        pawn->state = PAWN_PROMOTION;
        return 1;
      }

      pawn->state = PAWN_IDLE;
      return 0;
    }
  }

  return 1;
}

inline static s8 promotionAnimation(Pawn *pawn) {
  if (frame % FRAME_ANIMATION == 0) {
    if (pawn->indexSprite >= MAX_SPRITES_ANIM)
      return 0;

    ACTOR_deallocSprite(&pawn->actor);
    ACTOR_init(&pawn->actor, sprites[pawn->indexSprite], ENEMY_PAL,
               pawn->actor.collisionCurPos.x, pawn->actor.collisionCurPos.y,
               COLLISION_TYPE_PAWN);
    pawn->indexSprite++;
  }

  return 1;
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void PAWN_init(Pawn *pawn, const SpriteDefinition *sprite, const u16 palette,
               const s16 x, const s16 y) {
  pawn->state = PAWN_IDLE;
  pawn->indexSprite = 0;
  ACTOR_init(&pawn->actor, sprite, palette, x, y, COLLISION_TYPE_PAWN);
}

void PAWN_deallocDestroy(Pawn *pawn) {
  pawn->state = PAWN_DESTROYED;
  ACTOR_destroy(&pawn->actor);
}

void PAWN_dealloc(Pawn *pawn) { ACTOR_destroy(&pawn->actor); }

s8 PAWN_update(Pawn *pawn) {
  switch (pawn->state) {
  case PAWN_DEAD:
  case PAWN_DESTROYED:
    return 0;
  case PAWN_MOVING:
    return moveAnimation(pawn);
  case PAWN_IDLE:
    return startMovement(pawn);
  case PAWN_PROMOTION:
    return promotionAnimation(pawn);
  }

  return 1;
}

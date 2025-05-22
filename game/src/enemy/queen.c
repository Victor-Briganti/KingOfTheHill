#include "enemy/queen.h"
#include "map/map.h"
#include "node/actor.h"
#include "player/player.h"
#include "scene/scene_manager.h"

#include <genesis.h>

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

inline static s8 startMovement(Queen *queen) {
  const s16 queenX = queen->actor.collisionCurPos.x;
  const s16 queenY = queen->actor.collisionCurPos.y;
  const s16 playerX = player.actor.collisionCurPos.x;
  const s16 playerY = player.actor.collisionCurPos.y;

  // Vertical attack
  if (queenX == playerX) {
    const s16 y = playerY > queenY ? playerY - 1 : playerY + 1;
    Vect2D_s16 res = MAP_checkVertical((Vect2D_s16){queenX, queenY},
                                         (Vect2D_s16){queenX, y});

    if (res.x >= 0) {
      return 0;
    }

    ACTOR_setTargetAnimPos(&queen->actor, queenX, playerY);
    queen->state = QUEEN_MOVING;
    return 1;
  }

  // Horizontal attack
  if (queenY == playerY) {
    const s16 x = playerX > queenX ? playerX - 1 : playerX + 1;
    Vect2D_s16 res = MAP_checkHorizontal((Vect2D_s16){queenX, queenY},
                                         (Vect2D_s16){x, queenY});

    if (res.x >= 0) {
      return 0;
    }

    ACTOR_setTargetAnimPos(&queen->actor, playerX, queenY);
    queen->state = QUEEN_MOVING;
    return 1;
  }

  // Diagonal attack
  if (abs(queenY - playerY) == abs(queenX - playerX)) {
    const s16 x = playerX > queenX ? playerX - 1 : playerX + 1;
    const s16 y = playerY > queenY ? playerY - 1 : playerY + 1;

    Vect2D_s16 res =
        MAP_checkDiagonal((Vect2D_s16){queenX, queenY}, (Vect2D_s16){x, y});
    if (res.x >= 0) {
      return 0;
    }

    ACTOR_setTargetAnimPos(&queen->actor, playerX, playerY);
    queen->state = QUEEN_MOVING;
    return 1;
  }

  // Not aligned — move one step closer (queen-style)
  s16 dx = playerX > queenX ? 2 : (playerX < queenX ? -2 : 0);
  s16 dy = playerY > queenY ? 2 : (playerY < queenY ? -2 : 0);
  dx = clamp(queenX + dx, 0, mapLevelWidth - 2);
  dy = clamp(queenY + dy, 0, mapLevelHeight - 2);

  Vect2D_s16 res =
      MAP_checkVertical((Vect2D_s16){queenX, queenY}, (Vect2D_s16){dx, dy});
  if (res.x >= 0) {
    return 0;
  }

  res = MAP_checkHorizontal((Vect2D_s16){queenX, queenY}, (Vect2D_s16){dx, dy});
  if (res.x >= 0) {
    return 0;
  }

  res = MAP_checkDiagonal((Vect2D_s16){queenX, queenY}, (Vect2D_s16){dx, dy});
  if (res.x >= 0) {
    return 0;
  }
  
  ACTOR_setTargetAnimPos(&queen->actor, dx, dy);

  queen->state = QUEEN_MOVING;
  return 1;
}

inline static s8 moveAnimation(Queen *queen) {
  if (frame % FRAME_ANIMATION == 0) {
    ACTOR_animateTo(&queen->actor);

    if (!queen->actor.moving) {
      if (ACTOR_checkCollision(&queen->actor)) {
        kprintf("Player hit");
        player.health--;
        player.state = PLAYER_DEAD;
        return 0;
      }

      queen->state = QUEEN_IDLE;
      return 0;
    }
  }

  return 1;
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void QUEEN_init(Queen *queen, const SpriteDefinition *sprite, const u16 palette,
                const s16 x, const s16 y) {
  queen->state = QUEEN_IDLE;
  queen->indexSprite = 0;
  ACTOR_init(&queen->actor, sprite, palette, x, y, COLLISION_TYPE_QUEEN);
}

void QUEEN_deallocDestroy(Queen *queen) {
  queen->state = QUEEN_DESTROYED;
  ACTOR_destroy(&queen->actor);
}

void QUEEN_dealloc(Queen *queen) { ACTOR_destroy(&queen->actor); }

s8 QUEEN_update(Queen *queen) {
  switch (queen->state) {
  case QUEEN_DEAD:
  case QUEEN_DESTROYED:
    return 0;
  case QUEEN_MOVING:
    return moveAnimation(queen);
  case QUEEN_IDLE:
    return startMovement(queen);
  }

  return 1;
}

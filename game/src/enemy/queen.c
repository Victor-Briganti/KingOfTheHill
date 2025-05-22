#include "enemy/queen.h"
#include "node/actor.h"
#include "player/player.h"

#include <genesis.h>

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

inline static s8 startMovement(Queen *queen) {
  s16 x = queen->actor.collisionCurPos.x;
  s16 y = clamp(queen->actor.collisionCurPos.y + 2, 0, mapLevelHeight - 2);

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

  ACTOR_setTargetAnimPos(&queen->actor, x, y);
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

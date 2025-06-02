#ifndef __ACTOR_H__
#define __ACTOR_H__

#include "global.h"
#include "map/map.h"
#include "types/collision.h"

#include <genesis.h>
#include <maths.h>

typedef struct ActorNode {
  // Animation components
  Sprite *sprite;                // Current node position in tiles
  Vect2D_s16 animationPos;       // Current node position in tiles
  Vect2D_s16 animationTargetPos; // Target node position in tiles
  bool moving;

  // Collision components
  Vect2D_s16 collisionCurPos;  // Current object position in tiles
  Vect2D_s16 collisionPrevPos; // Previously object position in tiles
  CollisionType collisionType;
} ActorNode;

inline void ACTOR_updatePos(const ActorNode *const node) {
  SPR_setPosition(node->sprite, POS_X(node->animationPos.x),
                  POS_Y(node->animationPos.y));
}

inline void ACTOR_init(ActorNode *const node,
                       const SpriteDefinition *const sprite, const u16 palette,
                       const s16 x, const s16 y, const CollisionType type) {
  PAL_setPalette(palette, sprite->palette->data, DMA);
  node->sprite = SPR_addSprite(sprite, POS_X(x), POS_Y(y),
                               TILE_ATTR(palette, FALSE, FALSE, FALSE));

  const Vect2D_s16 vec = {x, y};
  // Animation init
  node->animationPos = vec;
  node->animationTargetPos = (Vect2D_s16){0, 0};
  node->moving = FALSE;

  // Collision init
  node->collisionPrevPos = vec;
  node->collisionCurPos = vec;
  node->collisionType = type;
}

inline void ACTOR_destroy(ActorNode *const node) {
  // Release the resources
  SPR_releaseSprite(node->sprite);

  // Clean the map
  MAP_removeCollision(node->collisionPrevPos, node->collisionType);
  MAP_removeCollision(node->collisionCurPos, node->collisionType);

  // Clean other values
  const Vect2D_s16 vec = {-1, -1};
  node->animationPos = vec;
  node->animationTargetPos = vec;
  node->collisionPrevPos = vec;
  node->collisionCurPos = vec;
  node->collisionType = COLLISION_TYPE_EMPTY;
  node->moving = FALSE;
}

inline void ACTOR_deallocSprite(const ActorNode *const node) {
  // Release the resources
  SPR_releaseSprite(node->sprite);
}

inline void ACTOR_animateTo(ActorNode *const node) {
  if (node->moving == FALSE)
    return;

  if (node->animationPos.x < node->animationTargetPos.x)
    node->animationPos.x++;
  else if (node->animationPos.x > node->animationTargetPos.x)
    node->animationPos.x--;

  if (node->animationPos.y < node->animationTargetPos.y)
    node->animationPos.y++;
  else if (node->animationPos.y > node->animationTargetPos.y)
    node->animationPos.y--;

  ACTOR_updatePos(node);

  if (node->animationPos.x == node->animationTargetPos.x &&
      node->animationPos.y == node->animationTargetPos.y) {
    node->collisionCurPos = node->animationPos;
    node->moving = FALSE;
  }
}

inline void ACTOR_setTargetAnimPos(ActorNode *const node, const s16 x,
                                   const s16 y) {
  const Vect2D_s16 vec = (Vect2D_s16){x, y};

  node->collisionPrevPos = node->animationPos;
  node->collisionCurPos = vec;

  node->animationTargetPos = vec;
  node->moving = TRUE;
}

inline bool ACTOR_checkCollision(const ActorNode *const node) {
  const s16 entry = map[node->collisionCurPos.y][node->collisionCurPos.x];
  return ((entry != 0) && ((~node->collisionType & entry) != 0));
}

inline void ACTOR_blink(const ActorNode *const node) {
  if (frame % FRAME_ANIMATION == 0) {
    if (SPR_getVisibility(node->sprite) == HIDDEN) {
      SPR_setVisibility(node->sprite, VISIBLE);
    } else {
      SPR_setVisibility(node->sprite, HIDDEN);
    }
  }
}

inline void ACTOR_setVisible(const ActorNode *const node) {
  SPR_setVisibility(node->sprite, VISIBLE);
}

#endif // __ACTOR_H__

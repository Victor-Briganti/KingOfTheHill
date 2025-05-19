#ifndef __ACTOR_H__
#define __ACTOR_H__

#include "global.h"
#include "map/map.h"

#include <genesis.h>
#include <maths.h>

typedef enum CollisionType {
  COLLISION_TYPE_EMPTY = 0,
  COLLISION_TYPE_PLAYER = 1 << 0,
  COLLISION_TYPE_PAWN = 1 << 1,
  COLLISION_TYPE_TOWER = 1 << 2,
  COLLISION_TYPE_BISHOP = 1 << 3,
  COLLISION_TYPE_KNIGHT = 1 << 4,
  COLLISION_TYPE_QUEEN = 1 << 5,
  COLLISION_TYPE_KING = 1 << 6
} CollisionType;

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

inline void ACTOR_init(ActorNode *const node, const SpriteDefinition *sprite,
                       const u16 palette, const s16 x, const s16 y,
                       const CollisionType type) {
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
  map[node->collisionPrevPos.y][node->collisionPrevPos.x] =
      COLLISION_TYPE_EMPTY;
  map[node->collisionCurPos.y][node->collisionCurPos.x] = COLLISION_TYPE_EMPTY;

  // Clean other values
  const Vect2D_s16 vec = {-1, -1};
  node->animationPos = vec;
  node->animationTargetPos = vec;
  node->collisionPrevPos = vec;
  node->collisionCurPos = vec;
  node->collisionType = COLLISION_TYPE_EMPTY;
  node->moving = FALSE;
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

#endif // __ACTOR_H__

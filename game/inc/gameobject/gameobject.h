#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "global.h"
#include "map/map.h"

#include <genesis.h>
#include <maths.h>

//===----------------------------------------------------------------------===//
// COLLISION OBJECT
//===----------------------------------------------------------------------===//

typedef enum CollisionObjectType {
  COLLISION_OBJECT_EMPTY = 0,
  COLLISION_OBJECT_PLAYER = 1 << 0,
  COLLISION_OBJECT_PAWN = 1 << 1,
  COLLISION_OBJECT_TOWER = 1 << 2,
  COLLISION_OBJECT_BISHOP = 1 << 3,
  COLLISION_OBJECT_KNIGHT = 1 << 4,
  COLLISION_OBJECT_QUEEN = 1 << 5,
  COLLISION_OBJECT_KING = 1 << 6
} CollisionObjectType;

typedef struct CollisionObject {
  Vect2D_s16 cur;  // Current object position in tiles (for collision)
  Vect2D_s16 prev; // Previously object position in tiles (for collision)
  CollisionObjectType type;
} CollisionObject;

inline void COLLISION_init(CollisionObject *const object, s16 x, s16 y,
                           CollisionObjectType type) {
  object->prev = (Vect2D_s16){x, y};
  object->cur = (Vect2D_s16){x, y};
  object->type = type;
}

inline bool COLLISION_check(CollisionObject *const object) {
  CollisionObjectType type = map[object->cur.y][object->cur.x];
  return type != COLLISION_OBJECT_EMPTY && (object->type & type) == 0;
}

inline void COLLISION_setPrevPos(CollisionObject *const object, s16 x, s16 y) {
  object->prev = (Vect2D_s16){x, y};
}

inline void COLLISION_setCurPos(CollisionObject *const object, s16 x, s16 y) {
  object->cur = (Vect2D_s16){x, y};
}

//===----------------------------------------------------------------------===//
// GAME OBJECT
//===----------------------------------------------------------------------===//

typedef struct GameObject {
  Sprite *sprite;
  Vect2D_s16 pos;    // Current object position in tiles (for animations)
  Vect2D_s16 target; // Target object position in tiles (for animations)

  CollisionObject collisionObj;

  Vect2D_s16 pixel; // Current object position in raw coords
  bool moving;
} GameObject;

inline void GAMEOBJECT_initInBoard(GameObject *const object,
                                   const SpriteDefinition *sprite, u16 palette,
                                   s16 x, s16 y, CollisionObjectType type) {
  PAL_setPalette(palette, sprite->palette->data, DMA);
  object->sprite = SPR_addSprite(sprite, POS_X(x), POS_Y(y),
                                 TILE_ATTR(palette, FALSE, FALSE, FALSE));

  object->pos = (Vect2D_s16){x, y};
  object->moving = FALSE;

  COLLISION_init(&object->collisionObj, x, y, type);
}

inline void GAMEOBJECT_initRawCoords(GameObject *const object,
                                     const SpriteDefinition *sprite,
                                     u16 palette, s16 x, s16 y) {
  PAL_setPalette(palette, sprite->palette->data, DMA);
  object->sprite =
      SPR_addSprite(sprite, x, y, TILE_ATTR(palette, FALSE, FALSE, FALSE));

  object->pixel = (Vect2D_s16){x, y};
  object->moving = FALSE;
}

inline void GAMEOBJECT_updatePos(GameObject *object) {
  SPR_setPosition(object->sprite, POS_X(object->pos.x), POS_Y(object->pos.y));
}

inline void GAMEOBJECT_setTargetPos(GameObject *const object, s16 x, s16 y) {
  COLLISION_setPrevPos(&object->collisionObj, object->pos.x, object->pos.y);
  COLLISION_setCurPos(&object->collisionObj, x, y);
  object->target = (Vect2D_s16){x, y};
  object->moving = TRUE;
}

inline void GAMEOBJECT_releaseSprite(GameObject *const object) {
  SPR_releaseSprite(object->sprite);
}

inline void GAMEOBJECT_animateTo(GameObject *const object) {
  if (object->moving == FALSE)
    return;

  if (object->pos.x < object->target.x)
    object->pos.x++;
  else if (object->pos.x > object->target.x)
    object->pos.x--;

  if (object->pos.y < object->target.y)
    object->pos.y++;
  else if (object->pos.y > object->target.y)
    object->pos.y--;

  GAMEOBJECT_updatePos(object);

  if (object->pos.x == object->target.x && object->pos.y == object->target.y) {
    COLLISION_setCurPos(&object->collisionObj, object->pos.x, object->pos.y);
    object->moving = FALSE;
  }
}

inline s16 GAMEOBJECT_getCurCollisionX(GameObject *const object) {
  return object->collisionObj.cur.x;
}

inline s16 GAMEOBJECT_getCurCollisionY(GameObject *const object) {
  return object->collisionObj.cur.y;
}

inline s16 GAMEOBJECT_getPrevCollisionX(GameObject *const object) {
  return object->collisionObj.prev.x;
}

inline s16 GAMEOBJECT_getPrevCollisionY(GameObject *const object) {
  return object->collisionObj.prev.y;
}

inline CollisionObjectType
GAMEOBJECT_getCollisionType(GameObject *const object) {
  return object->collisionObj.type;
}

#endif //  __GAMEOBJECT_H__

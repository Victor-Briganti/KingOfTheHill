#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "global.h"

#include <genesis.h>
#include <maths.h>

typedef struct GameObject {
  Sprite *sprite;
  Vect2D_s16 pos;    // Current object position in tiles (for animations)
  Vect2D_s16 target; // Target object position in tiles (for animations)

  Vect2D_s16 cur;  // Current object position in tiles (for collision)
  Vect2D_s16 prev; // Previously object position in tiles (for collision)

  Vect2D_s16 pixel; // Current object position in raw coords
  bool moving;
} GameObject;

inline void GAMEOBJECT_initInBoard(GameObject *const object,
                                   const SpriteDefinition *sprite, u16 palette,
                                   s16 x, s16 y) {
  PAL_setPalette(palette, sprite->palette->data, DMA);
  object->sprite = SPR_addSprite(sprite, POS_X(x), POS_Y(y),
                                 TILE_ATTR(palette, FALSE, FALSE, FALSE));

  object->pos = (Vect2D_s16){x, y};
  object->prev = (Vect2D_s16){x, y};
  object->cur = (Vect2D_s16){x, y};
  object->moving = FALSE;
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

inline void GAMEOBJECT_setPrevPos(GameObject *const object) {
  object->prev = object->pos;
}

inline void GAMEOBJECT_setTargetPos(GameObject *const object, s16 x, s16 y) {
  object->prev = object->pos;
  object->cur = (Vect2D_s16){x, y};
  object->target = (Vect2D_s16){x, y};
  object->moving = TRUE;
}

inline void GAMEOBJECT_releaseSprite(GameObject *const object) {
  SPR_releaseSprite(object->sprite);
}

inline bool GAMEOBJECT_animateTo(GameObject *const object, s16 posX, s16 posY) {
  if (object->pos.x < posX)
    object->pos.x++;
  else if (object->pos.x > posX)
    object->pos.x--;

  if (object->pos.y < posY)
    object->pos.y++;
  else if (object->pos.y > posY)
    object->pos.y--;

  GAMEOBJECT_updatePos(object);

  return (object->pos.x == posX && object->pos.y == posY);
}

inline void GAMEOBJECT_animateTo2(GameObject *const object) {
  if (object->moving == FALSE)
    return;

  if (object->pos.x <object->target.x)
    object->pos.x++;
  else if (object->pos.x >object->target.x)
    object->pos.x--;

  if (object->pos.y <object->target.y)
    object->pos.y++;
  else if (object->pos.y >object->target.y)
    object->pos.y--;

  GAMEOBJECT_updatePos(object);

  if (object->pos.x ==object->target.x && object->pos.y ==object->target.y) {
    object->cur = object->pos;
    object->moving = FALSE;
  }
}

#endif //  __GAMEOBJECT_H__

#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <genesis.h>

typedef struct GameObject {
    Sprite *sprite;
    s8 x_pos;
    s8 y_pos;
} GameObject;

void GAMEOBJECT_init(GameObject *const obj, const SpriteDefinition *sprite, u16 palette,
                     s8 x_pos, s8 y_pos);

void GAMEOBJECT_updatePos(GameObject *const obj, u16 widthLimit, u16 heighLimit);

#endif //  _GAMEOBJECT_H_

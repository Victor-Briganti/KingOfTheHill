#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <genesis.h>

typedef struct GameObject {
    Sprite *sprite;
    s8 x_pos;
    s8 y_pos;
} GameObject;

void GAMEOBJECT_init(GameObject *const obj, const SpriteDefinition *sprite, u16 palette,
                     s8 x_pos, s8 y_pos);

void GAMEOBJECT_updatePos(GameObject *const obj, u16 widthLimit, u16 heighLimit);

#endif //  __GAMEOBJECT_H__

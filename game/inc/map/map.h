#ifndef __MAP_H__
#define __MAP_H__

#include "types/collision.h"

#include <genesis.h>

#define MAP_MAX_HEIGHT 24 /* TODO: Change this */
#define MAP_MAX_WIDTH 24  /* TODO: Change this */

extern u16 map[MAP_MAX_HEIGHT][MAP_MAX_WIDTH];

typedef struct MapObject {
  u16 x, y;
  CollisionType object;
} MapObject;

void MAP_initLevel(const u16 mapHeight, const u16 mapWidth);

void MAP_initCollision(MapObject objectVector[], const u16 count);

void MAP_updateCollision(const Vect2D_s16 prev, const Vect2D_s16 cur,
                         const CollisionType colType);

#endif // __MAP_H__

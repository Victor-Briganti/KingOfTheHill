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

void MAP_initLevel(u16 mapHeight, u16 mapWidth);

void MAP_initObjects(MapObject objectVector[], u16 count);

void MAP_updateLevel();

#endif // __MAP_H__

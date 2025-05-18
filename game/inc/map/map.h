#ifndef __MAP_H__
#define __MAP_H__

#include "global.h"

#define MAP_MAX_HEIGHT MAP_LEVEL1_HEIGHT 
#define MAP_MAX_WIDTH MAP_LEVEL1_WIDTH

extern u16 map[MAP_MAX_HEIGHT][MAP_MAX_WIDTH];

typedef enum MapObjectType {
    MAP_OBJECT_EMPTY  = 0,
    MAP_OBJECT_PLAYER = 1 << 0,
    MAP_OBJECT_PAWN   = 1 << 1,
    MAP_OBJECT_TOWER  = 1 << 2,
    MAP_OBJECT_BISHOP = 1 << 3,
    MAP_OBJECT_KNIGHT = 1 << 4,
    MAP_OBJECT_QUEEN  = 1 << 5,
    MAP_OBJECT_KING   = 1 << 6
} MapObjectType;

typedef struct MapObject{
    u16 x, y;
    MapObjectType object;
} MapObject;

void MAP_initLevel(u16 mapHeight, u16 mapWidth);

void MAP_initObjects(MapObject objectVector[], u16 count);

void MAP_updateLevel();

#endif // __MAP_H__

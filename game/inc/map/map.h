#ifndef __MAP_H__
#define __MAP_H__

#include "global.h"

extern u16 map[MAP_LEVEL1_HEIGHT][MAP_LEVEL1_WIDTH];

#define MAP_MARK_EMPTY (0)
#define MAP_MARK_PLAYER (1)
#define MAP_MARK_PAWN (2)
#define MAP_MARK_TOWER (4)
#define MAP_MARK_BISHOP (8)
#define MAP_MARK_KNIGHT (16)
#define MAP_MARK_QUEEN (32)
#define MAP_MARK_KING (64)

void MAP_initLevel1();
bool MAP_updateLevel1();

#endif // __MAP_H__

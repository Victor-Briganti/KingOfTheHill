#include "map/map.h"
#include "maths.h"
#include "types/collision.h"

u16 map[MAP_MAX_HEIGHT][MAP_MAX_WIDTH] = {0};

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void MAP_initLevel(const u16 mapHeight, const u16 mapWidth) {
  if (mapHeight > MAP_MAX_HEIGHT && mapWidth > MAP_MAX_WIDTH)
    kprintf("Map is bigger than supported");

  for (u16 i = 0; i < mapHeight; i++) {
    for (u16 j = 0; j < mapWidth; j++) {
      map[i][j] = 0;
    }
  }
}

void MAP_initObjects(MapObject objectVector[], const u16 count) {
  for (u16 i = 0; i < count; i++) {
    map[objectVector[i].y][objectVector[i].x] |= objectVector[i].object;
  }
}

void MAP_updateCollision(const Vect2D_s16 prev, const Vect2D_s16 cur,
                         const CollisionType colType) {
  if (prev.y < 0 && prev.x < 0)
    return;

  map[prev.y][prev.x] &= ~colType;
  map[cur.y][cur.x] |= colType;
}

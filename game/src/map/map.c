#include "map/map.h"
#include "enemy/pawn.h"
#include "player/player.h"

u16 map[MAP_MAX_HEIGHT][MAP_MAX_WIDTH] = {0};

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

ActorNode *actors[2] = {
    &player.actor,
    &pawn.actor,
};

static inline void MAP_updateCollisions() {
  s16 x, y;
  for (u8 i = 0; i < 2; i++) {
    y = actors[i]->collisionPrevPos.y;
    x = actors[i]->collisionPrevPos.x;
    map[y][x] &= ~actors[i]->collisionType;

    y = actors[i]->collisionCurPos.y;
    x = actors[i]->collisionCurPos.x;
    map[y][x] |= actors[i]->collisionType;
  }
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void MAP_initLevel(u16 mapHeight, u16 mapWidth) {
  if (mapHeight > MAP_MAX_HEIGHT && mapWidth > MAP_MAX_WIDTH)
    kprintf("Map is bigger than supported");

  for (u8 i = 0; i < mapHeight; i++) {
    for (u8 j = 0; j < mapWidth; j++) {
      map[i][j] = 0;
    }
  }
}

void MAP_initObjects(MapObject objectVector[], u16 count) {
  for (u16 i = 0; i < count; i++) {
    map[objectVector[i].y][objectVector[i].x] |= objectVector[i].object;
  }
}

void MAP_updateLevel() { MAP_updateCollisions(); }

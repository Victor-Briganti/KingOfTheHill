#include "map/map.h"
#include "enemy/pawn.h"
#include "gameobject/gameobject.h"
#include "player/player.h"

u16 map[MAP_MAX_HEIGHT][MAP_MAX_WIDTH] = {0};

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

GameObject *objects[2] = {
    &player.object,
    &pawn.object,
};

static void MAP_updateCollisions() {
  s16 x, y;
  CollisionObjectType type;
  for (u8 i = 0; i < 2; i++) {
    type = GAMEOBJECT_getCollisionType(objects[i]);

    y = GAMEOBJECT_getPrevCollisionY(objects[i]);
    x = GAMEOBJECT_getPrevCollisionX(objects[i]);
    map[y][x] &= ~type;

    y = GAMEOBJECT_getCurCollisionY(objects[i]);
    x = GAMEOBJECT_getCurCollisionX(objects[i]);
    map[y][x] |= type;
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

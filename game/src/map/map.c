#include "map/map.h"
#include "enemy/pawn.h"
#include "player/player.h"

u16 map[MAP_MAX_HEIGHT][MAP_MAX_WIDTH] = {0};

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

bool MAP_updateLevel() {
  map[player.previousY][player.previousX] &= ~MAP_OBJECT_PLAYER;
  map[player.posY][player.posX] |= MAP_OBJECT_PLAYER;

  map[pawn.object.prev.y][pawn.object.prev.x] &= ~MAP_OBJECT_PAWN;
  map[pawn.object.cur.y][pawn.object.cur.x] |= MAP_OBJECT_PAWN;

  if (map[player.posY][player.posX] & MAP_OBJECT_PAWN &&
      map[player.posY][player.posX] & MAP_OBJECT_PLAYER) {
    player.health--;
    return FALSE;
  }

  return TRUE;
}

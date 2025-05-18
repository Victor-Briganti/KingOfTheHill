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

void MAP_updateLevel() {
  // Updates the position of the player
  map[player.object.prev.y][player.object.prev.x] &= ~MAP_OBJECT_PLAYER;
  map[player.object.cur.y][player.object.cur.x] |= MAP_OBJECT_PLAYER;
  
  // Updates the position of the pawn
  map[pawn.object.prev.y][pawn.object.prev.x] &= ~MAP_OBJECT_PAWN;
  map[pawn.object.cur.y][pawn.object.cur.x] |= MAP_OBJECT_PAWN;
}

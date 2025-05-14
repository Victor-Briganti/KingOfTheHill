#include "map/map.h"
#include "enemy/pawn.h"
#include "global.h"
#include "player/player.h"

u16 map[MAP_LEVEL1_HEIGHT][MAP_LEVEL1_WIDTH];

void MAP_initLevel1() {
  for (u8 i = 0; i < MAP_LEVEL1_HEIGHT; i++) {
    for (u8 j = 0; j < MAP_LEVEL1_WIDTH; j++) {
      map[i][j] = 0;
    }
  }

  map[PLAYER_LEVEL1_Y_POS][PLAYER_LEVEL1_X_POS] = MAP_MARK_PLAYER;
}

bool MAP_updateLevel1() {
  map[player.previousY][player.previousX] &= ~MAP_MARK_PLAYER;
  map[player.posY][player.posX] |= MAP_MARK_PLAYER;

  map[pawn.previousY][pawn.previousX] &= ~MAP_MARK_PAWN;
  map[pawn.posY][pawn.posX] |= MAP_MARK_PAWN;

  if (map[player.posY][player.posX] & MAP_MARK_PAWN &&
      map[player.posY][player.posX] & MAP_MARK_PLAYER) {
    player.health--;
    return FALSE;
  }

  return TRUE;
}

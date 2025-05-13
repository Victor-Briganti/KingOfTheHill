#include "map/map.h"
#include "player/player.h"
#include "global.h"

u16 map[MAP_LEVEL1_HEIGHT][MAP_LEVEL1_WIDTH] = {0};

void MAP_initLevel1() {
    map[PLAYER_LEVEL1_Y_POS][PLAYER_LEVEL1_X_POS] = MAP_MARK_PLAYER;
}

void MAP_updateLevel1() {
    map[player.object.y][player.object.x] = MAP_MARK_PLAYER;
}

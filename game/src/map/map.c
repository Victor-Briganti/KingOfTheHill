#include "map/map.h"
#include "player/player.h"
#include "enemy/pawn.h"
#include "global.h"

u16 map[MAP_LEVEL1_HEIGHT][MAP_LEVEL1_WIDTH] = {0};

void MAP_initLevel1() {
    map[PLAYER_LEVEL1_Y_POS][PLAYER_LEVEL1_X_POS] = MAP_MARK_PLAYER;
}

bool MAP_updateLevel1() {
    map[player.previousY][player.previousX] = MAP_MARK_EMPTY;
    map[player.object.y][player.object.x] = MAP_MARK_PLAYER;
    
    map[pawn.previousY][pawn.previousX] = MAP_MARK_EMPTY;
    map[pawn.object.y][pawn.object.x] = MAP_MARK_PAWN;
 
    return TRUE;
}

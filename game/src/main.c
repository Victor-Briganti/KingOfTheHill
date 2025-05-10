#include <genesis.h>
#include <sprite_eng.h>

#include "gfx.h"
#include "global.h"
#include "joy.h"
#include "player/player.h"
#include "sprites.h"
#include "sys.h"
#include "tilemap/tilemap.h"
#include "tools.h"
#include "vdp.h"

//===----------------------------------------------------------------------===//
// GLOBALS
//===----------------------------------------------------------------------===//

u16 mapLevelHeight;
u16 mapLevelWidth;

u16 mapLevelX;
u16 mapLevelY;

u16 playerInitX;
u16 playerInitY;

//===----------------------------------------------------------------------===//
// AUXILIARY
//===----------------------------------------------------------------------===//

static void COMMON_init() {
  // Set the resolution of the screen
  VDP_setScreenWidth320();
  VDP_setScreenHeight224();

  // Init the sprite and input engine
  SPR_init();
  JOY_init();

  // Init the player object
  PLAYER_init();

  // Initialize globals
  mapLevelHeight = MAP_LEVEL1_HEIGHT;
  mapLevelWidth = MAP_LEVEL1_WIDTH;

  mapLevelX = MAP_LEVEL1_X_POS;
  mapLevelY = MAP_LEVEL1_Y_POS;

  playerInitX = PLAYER_LEVEL1_X_POS;
  playerInitY = PLAYER_LEVEL1_Y_POS;
}

static void BACKGROUND_init() {
  // Draw background
  VDP_drawImageEx(BACKGROUND_PLANE, &background_level,
                  TILE_ATTR_FULL(BACKGROUND_PAL, 0, FALSE, FALSE, 1), 0, 0,
                  true, DMA);

  // Render level info
  VDP_setTextPalette(BACKGROUND_PAL);
  VDP_setTextPlane(BACKGROUND_PLANE);
}

static void BACKGROUND_update(u16 score) {
  char scoreText[6];
  sprintf(scoreText, "%d", score);
  VDP_drawText(scoreText, SCORE_TEXT_X, SCORE_TEXT_Y);
  VDP_drawText("LEVEL 1-1", LEVEL_TEXT_X, LEVEL_TEXT_Y);
}

//===----------------------------------------------------------------------===//
// MAIN
//===----------------------------------------------------------------------===//

int main(bool resetType) {
  if (!resetType) {
    SYS_hardReset();
  }

  COMMON_init();
  while (TRUE) {
    BACKGROUND_init();
    TILEMAP_init(&tileset);
    PLAYER_levelInit(&goblin_sprite1, PLAYER_PAL, playerInitX, playerInitY);
    SYS_doVBlankProcess();

    while (TRUE) {
      BACKGROUND_update(0);
      TILEMAP_update(&level_map1);
      PLAYER_update();
      SPR_update();
      SYS_doVBlankProcess();
    }
  }

  return 0;
}

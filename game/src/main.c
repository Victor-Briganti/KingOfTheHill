#include <genesis.h>
#include <sprite_eng.h>

#include "gameobject/gameobject.h"
#include "gfx.h"
#include "global.h"
#include "joy.h"
#include "player/player.h"
#include "sprites.h"
#include "sys.h"
#include "tilemap/tilemap.h"
#include "tools.h"
#include "vdp.h"

void SCREEN_init() {
  SYS_disableInts();

  // Set the resolution of the screen
  VDP_setScreenWidth320();
  VDP_setScreenHeight224();

  // Init the sprite and input engine
  SPR_init();
  JOY_init();

  SYS_enableInts();
}

void BACKGROUND_init() {
  // Draw background
  VDP_drawImageEx(BACKGROUND_PLANE, &background_level,
                  TILE_ATTR_FULL(BACKGROUND_PAL, 0, FALSE, FALSE, 1), 0, 0,
                  true, DMA);

  // Render level info
  VDP_setTextPalette(BACKGROUND_PAL);
  VDP_setTextPlane(BACKGROUND_PLANE);
  VDP_drawText("LEVEL 1-1", LEVEL_TEXT_X, LEVEL_TEXT_Y);
}

void BACKGROUND_update(u16 score) {
  char scoreText[6];
  sprintf(scoreText, "%d", score);
  VDP_drawText(scoreText, SCORE_TEXT_X, SCORE_TEXT_Y);
}

int main(bool resetType) {
  // Soft reset doesn't clear RAM. Can lead to bugs.
  if (!resetType) {
    SYS_hardReset();
  }

  SCREEN_init();
  BACKGROUND_init();
  TILEMAP_init(&tileset);
  PLAYER_init(&goblin_sprite1, PLAYER_PAL, GOBLIN_LEVEL1_X_POS,
              GOBLIN_LEVEL1_Y_POS);

  SYS_doVBlankProcess();

  u16 score = 0;
  while (TRUE) {
    BACKGROUND_update(score);
    TILEMAP_update(&level_map1);
    PLAYER_update();

    // update hardware sprites table
    SPR_update();

    // wait for VBLANK
    SYS_doVBlankProcess();
    score++;
  }

  return 0;
}

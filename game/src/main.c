#include <genesis.h>
#include <sprite_eng.h>

#include "global.h"
#include "resources.h"
#include "sprites.h"
#include "sys.h"
#include "tools.h"
#include "vdp.h"

void COMMON_init() {
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
  // Draw chess board
  VDP_drawImageEx(BG_B, &background, TILE_ATTR_FULL(PAL3, 0, FALSE, FALSE, 1),
                  0, 0, true, DMA);

  // Render level info
  char score[6];
  sprintf(score, "%d", 65443);
  VDP_drawText(score, 0, 1);
  VDP_drawText("LEVEL 1-1", 13, 1);
}

void SPRITE_init() {
  Sprite *goblin = SPR_addSprite(&goblin1, POS_X(0), POS_Y(0),
                                 TILE_ATTR(PAL0, FALSE, FALSE, FALSE));
  PAL_setPalette(PAL0, goblin1.palette->data, DMA);
}

int main(bool resetType) {
  // Soft reset doesn't clear RAM. Can lead to bugs.
  if (!resetType) {
    SYS_hardReset();
  }

  COMMON_init();
  BACKGROUND_init();
  SPRITE_init();

  SYS_doVBlankProcess();

  while (TRUE) {
    // update hardware sprites table
    SPR_update();

    // wait for VBLANK
    SYS_doVBlankProcess();
  }

  return 0;
}

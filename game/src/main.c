#include <genesis.h>
#include <sprite_eng.h>

#include "global.h"
#include "joy.h"
#include "resources.h"
#include "sprites.h"
#include "sys.h"
#include "tools.h"
#include "vdp.h"

void INPUT_eventHandler(u16 joy, u16 changed, u16 state);

void SCREEN_init() {
  SYS_disableInts();

  // Set the resolution of the screen
  VDP_setScreenWidth320();
  VDP_setScreenHeight224();

  // Init the sprite and input engine
  SPR_init();

  SYS_enableInts();
}

void INPUT_init() {
  JOY_init();
  JOY_setEventHandler(INPUT_eventHandler);
}

void BACKGROUND_init() {
  // Draw chess board
  VDP_drawImageEx(BACKGROUND_PLANE, &background,
                  TILE_ATTR_FULL(BACKGROUND_PAL, 0, FALSE, FALSE, 1), 0, 0,
                  true, DMA);

  // Render level info
  char score[6];
  sprintf(score, "%d", 65443);
  VDP_setTextPalette(BACKGROUND_PAL);
  VDP_setTextPlane(BACKGROUND_PLANE);
  VDP_drawText(score, 0, 1);
  VDP_drawText("LEVEL 1-1", 13, 1);
}

Sprite *SPRITE_init() {
  Sprite *goblin = SPR_addSprite(&goblin1, POS_X(0), POS_Y(0),
                                 TILE_ATTR(PLAYER_PAL, FALSE, FALSE, FALSE));
  PAL_setPalette(PLAYER_PAL, goblin1.palette->data, DMA);

  return goblin;
}

s8 x = 0;
s8 y = 0;

void SPRITE_clamp(s8 *x, s8 *y, u16 width, u16 height) {
  if (*x < 0)
    *x = 0;

  if (*x >= width)
    *x = width - 1;

  if (*y < 0)
    *y = 0;

  if (*y >= height)
    *y = height - 1;
}

void INPUT_eventHandler(u16 joy, u16 changed, u16 state) {
  if (joy != JOY_1)
    return;

  // Verify if the directionals are pressed
  u16 directional = state & BUTTON_DIR;

  // Move when the buttons are released
  if (!(directional)) {
    if (changed & BUTTON_LEFT) {
      x--;
    } else if (changed & BUTTON_RIGHT) {
      x++;
    } else if (changed & BUTTON_DOWN) {
      y++;
    } else if (changed & BUTTON_UP) {
      y--;
    }

    SPRITE_clamp(&x, &y, BOARD_WIDTH, BOARD_HEIGHT);
  }
}

int main(bool resetType) {
  // Soft reset doesn't clear RAM. Can lead to bugs.
  if (!resetType) {
    SYS_hardReset();
  }

  SCREEN_init();
  INPUT_init();
  BACKGROUND_init();
  Sprite *goblin = SPRITE_init();

  SYS_doVBlankProcess();

  while (TRUE) {
    SPR_setPosition(goblin, POS_X(x), POS_Y(y));

    // update hardware sprites table
    SPR_update();

    // wait for VBLANK
    SYS_doVBlankProcess();
  }

  return 0;
}

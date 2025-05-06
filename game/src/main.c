#include <genesis.h>
#include <sprite_eng.h>

#include "gameobject/gameobject.h"
#include "global.h"
#include "joy.h"
#include "gfx.h"
#include "sprites.h"
#include "sys.h"
#include "tools.h"
#include "vdp.h"

GameObject goblin;

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

void INPUT_eventHandler(u16 joy, u16 changed, u16 state) {
  if (joy != JOY_1)
    return;

  // Verify if the directionals are pressed
  u16 directional = state & BUTTON_DIR;

  // Move when the buttons are released
  if (!(directional)) {
    if (changed & BUTTON_LEFT) {
      goblin.x_pos--;
    } else if (changed & BUTTON_RIGHT) {
      goblin.x_pos++;
    } else if (changed & BUTTON_DOWN) {
      goblin.y_pos++;
    } else if (changed & BUTTON_UP) {
      goblin.y_pos--;
    }

    GAMEOBJECT_updatePos(&goblin, BOARD_WIDTH, BOARD_HEIGHT);
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
  GAMEOBJECT_init(&goblin, &goblin1, PLAYER_PAL, 0, 0);

  SYS_doVBlankProcess();

  while (TRUE) {
    // update hardware sprites table
    SPR_update();

    // wait for VBLANK
    SYS_doVBlankProcess();
  }

  return 0;
}

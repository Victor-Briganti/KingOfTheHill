#include <genesis.h>
#include <sprite_eng.h>

#include "gameobject/gameobject.h"
#include "gfx.h"
#include "global.h"
#include "joy.h"
#include "sprites.h"
#include "sys.h"
#include "tilemap/tilemap.h"
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

void MAP_updatePlayerTile() {
  if (goblin.x_pos < MAP_LEVEL1_WIDTH - 2)
    TILEMAP_updateRightTile(goblin.x_pos, goblin.y_pos, MAP_LEVEL1_X_POS,
                            MAP_LEVEL1_Y_POS, GREEN_TILE);

  if (goblin.x_pos != 0)
    TILEMAP_updateLeftTile(goblin.x_pos, goblin.y_pos, MAP_LEVEL1_X_POS,
                           MAP_LEVEL1_Y_POS, GREEN_TILE);

  if (goblin.y_pos != 0)
    TILEMAP_updateUpTile(goblin.x_pos, goblin.y_pos, MAP_LEVEL1_X_POS,
                         MAP_LEVEL1_Y_POS, GREEN_TILE);

  if (goblin.y_pos < MAP_LEVEL1_HEIGHT - 2)
    TILEMAP_updateBottomTile(goblin.x_pos, goblin.y_pos, MAP_LEVEL1_X_POS,
                             MAP_LEVEL1_Y_POS, GREEN_TILE);

  if (goblin.x_pos < MAP_LEVEL1_WIDTH - 2 && goblin.y_pos != 0)
    TILEMAP_updateUpRighTile(goblin.x_pos, goblin.y_pos, MAP_LEVEL1_X_POS,
                             MAP_LEVEL1_Y_POS, GREEN_TILE);

  if (goblin.x_pos != 0 && goblin.y_pos != 0)
    TILEMAP_updateUpLeftTile(goblin.x_pos, goblin.y_pos, MAP_LEVEL1_X_POS,
                             MAP_LEVEL1_Y_POS, GREEN_TILE);

  if (goblin.x_pos < MAP_LEVEL1_WIDTH - 2 &&
      goblin.y_pos < MAP_LEVEL1_HEIGHT - 2)
    TILEMAP_updateBottomRightTile(goblin.x_pos, goblin.y_pos, MAP_LEVEL1_X_POS,
                                  MAP_LEVEL1_Y_POS, GREEN_TILE);

  if (goblin.x_pos != 0 && goblin.y_pos < MAP_LEVEL1_HEIGHT - 2)
    TILEMAP_updateBottomLeftTile(goblin.x_pos, goblin.y_pos, MAP_LEVEL1_X_POS,
                                  MAP_LEVEL1_Y_POS, GREEN_TILE);
}

void INPUT_eventHandler(u16 joy, u16 changed, u16 state) {
  if (joy != JOY_1)
    return;

  // Verify if the directionals are pressed
  u16 directional = state & BUTTON_DIR;

  // Move when the buttons are released
  if (!(directional)) {
    if (changed & BUTTON_LEFT) {
      goblin.x_pos -= 2;
    } else if (changed & BUTTON_RIGHT) {
      goblin.x_pos += 2;
    } else if (changed & BUTTON_DOWN) {
      goblin.y_pos += 2;
    } else if (changed & BUTTON_UP) {
      goblin.y_pos -= 2;
    }

    GAMEOBJECT_updatePos(&goblin, MAP_LEVEL1_WIDTH - 1, MAP_LEVEL1_HEIGHT - 1);
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
  TILEMAP_init(&tileset);
  GAMEOBJECT_init(&goblin, &goblin1, PLAYER_PAL, GOBLIN_LEVEL1_X_POS,
                  GOBLIN_LEVEL1_Y_POS);

  SYS_doVBlankProcess();

  u16 score = 0;
  while (TRUE) {
    BACKGROUND_update(score);
    TILEMAP_update(&level_map1);
    MAP_updatePlayerTile();

    // update hardware sprites table
    SPR_update();

    // wait for VBLANK
    SYS_doVBlankProcess();
    score++;
  }

  return 0;
}

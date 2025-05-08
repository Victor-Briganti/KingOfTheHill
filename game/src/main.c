#include <genesis.h>
#include <sprite_eng.h>

#include "gameobject/gameobject.h"
#include "gfx.h"
#include "global.h"
#include "joy.h"
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
  VDP_drawImageEx(BACKGROUND_PLANE, &background_level,
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

u8 tile = 2;
u8 tile_diag = 1;
void MAP_updateTile() {
  // Updates the right tile
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile),
      goblin.x_pos + MAP_LEVEL1_X_POS + 2, goblin.y_pos + MAP_LEVEL1_Y_POS);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile),
      goblin.x_pos + MAP_LEVEL1_X_POS + 3, goblin.y_pos + MAP_LEVEL1_Y_POS);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile),
      goblin.x_pos + MAP_LEVEL1_X_POS + 2, goblin.y_pos + MAP_LEVEL1_Y_POS + 1);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile),
      goblin.x_pos + MAP_LEVEL1_X_POS + 3, goblin.y_pos + MAP_LEVEL1_Y_POS + 1);

  // Updates the bottom tile
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile),
      goblin.x_pos + MAP_LEVEL1_X_POS + 1, goblin.y_pos + MAP_LEVEL1_Y_POS + 2);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile),
      goblin.x_pos + MAP_LEVEL1_X_POS, goblin.y_pos + MAP_LEVEL1_Y_POS + 2);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile),
      goblin.x_pos + MAP_LEVEL1_X_POS + 1, goblin.y_pos + MAP_LEVEL1_Y_POS + 3);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile),
      goblin.x_pos + MAP_LEVEL1_X_POS, goblin.y_pos + MAP_LEVEL1_Y_POS + 3);

  // Updates the left tile
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile),
      goblin.x_pos + MAP_LEVEL1_X_POS - 1, goblin.y_pos + MAP_LEVEL1_Y_POS);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile),
      goblin.x_pos + MAP_LEVEL1_X_POS - 2, goblin.y_pos + MAP_LEVEL1_Y_POS);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile),
      goblin.x_pos + MAP_LEVEL1_X_POS - 1, goblin.y_pos + MAP_LEVEL1_Y_POS + 1);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile),
      goblin.x_pos + MAP_LEVEL1_X_POS - 2, goblin.y_pos + MAP_LEVEL1_Y_POS + 1);

  // Updates the up tile
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile),
      goblin.x_pos + MAP_LEVEL1_X_POS + 1, goblin.y_pos + MAP_LEVEL1_Y_POS - 1);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile),
      goblin.x_pos + MAP_LEVEL1_X_POS, goblin.y_pos + MAP_LEVEL1_Y_POS - 1);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile),
      goblin.x_pos + MAP_LEVEL1_X_POS + 1, goblin.y_pos + MAP_LEVEL1_Y_POS - 2);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile),
      goblin.x_pos + MAP_LEVEL1_X_POS, goblin.y_pos + MAP_LEVEL1_Y_POS - 2);

  // Updates the up right diagonal tile
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile_diag),
      goblin.x_pos + MAP_LEVEL1_X_POS + 2, goblin.y_pos + MAP_LEVEL1_Y_POS - 2);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile_diag),
      goblin.x_pos + MAP_LEVEL1_X_POS + 2, goblin.y_pos + MAP_LEVEL1_Y_POS - 1);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile_diag),
      goblin.x_pos + MAP_LEVEL1_X_POS + 3, goblin.y_pos + MAP_LEVEL1_Y_POS - 2);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile_diag),
      goblin.x_pos + MAP_LEVEL1_X_POS + 3, goblin.y_pos + MAP_LEVEL1_Y_POS - 1);

  // Updates the up left diagonal tile
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile_diag),
      goblin.x_pos + MAP_LEVEL1_X_POS - 1, goblin.y_pos + MAP_LEVEL1_Y_POS - 2);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile_diag),
      goblin.x_pos + MAP_LEVEL1_X_POS - 2, goblin.y_pos + MAP_LEVEL1_Y_POS - 1);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile_diag),
      goblin.x_pos + MAP_LEVEL1_X_POS - 2, goblin.y_pos + MAP_LEVEL1_Y_POS - 2);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile_diag),
      goblin.x_pos + MAP_LEVEL1_X_POS - 1, goblin.y_pos + MAP_LEVEL1_Y_POS - 1);

  // Updates the bottom left diagonal tile
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile_diag),
      goblin.x_pos + MAP_LEVEL1_X_POS - 1, goblin.y_pos + MAP_LEVEL1_Y_POS + 2);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile_diag),
      goblin.x_pos + MAP_LEVEL1_X_POS - 2, goblin.y_pos + MAP_LEVEL1_Y_POS + 2);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile_diag),
      goblin.x_pos + MAP_LEVEL1_X_POS - 1, goblin.y_pos + MAP_LEVEL1_Y_POS + 3);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile_diag),
      goblin.x_pos + MAP_LEVEL1_X_POS - 2, goblin.y_pos + MAP_LEVEL1_Y_POS + 3);

  // Updates the bottom right diagonal tile
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile_diag),
      goblin.x_pos + MAP_LEVEL1_X_POS + 2, goblin.y_pos + MAP_LEVEL1_Y_POS + 2);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile_diag),
      goblin.x_pos + MAP_LEVEL1_X_POS + 3, goblin.y_pos + MAP_LEVEL1_Y_POS + 2);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile_diag),
      goblin.x_pos + MAP_LEVEL1_X_POS + 2, goblin.y_pos + MAP_LEVEL1_Y_POS + 3);
  VDP_setTileMapXY(
      TILEMAP_PLANE, TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tile_diag),
      goblin.x_pos + MAP_LEVEL1_X_POS + 3, goblin.y_pos + MAP_LEVEL1_Y_POS + 3);

  if (tile == 2) {
    tile = 4;
  } else {
    tile = 2;
  }

  if (tile_diag == 1) {
    tile_diag = 4;
  } else {
    tile_diag = 1;
  }
}

void MAP_init() {
  PAL_setPalette(TILEMAP_PAL, tileset_palette.data, DMA);
  u16 baseTileIndex = TILE_USER_INDEX;
  VDP_loadTileSet(&tileset, baseTileIndex, DMA);
  VDP_setTileMapEx(TILEMAP_PLANE, &level_map1,
                   TILE_ATTR_FULL(TILEMAP_PAL, // match your palette
                                  0,           // priority
                                  FALSE,       // flip X
                                  FALSE,       // flip Y
                                  baseTileIndex),
                   // MAP_LEVEL1_X_POS, MAP_LEVEL1_Y_POS, // screen x,y in tiles
                   MAP_LEVEL1_X_POS, MAP_LEVEL1_Y_POS, // screen x,y in tiles
                   0, 0,                       // map x,y offset in tiles
                   level_map1.w, level_map1.h, // region size
                   DMA);
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

    GAMEOBJECT_updatePos(&goblin, MAP_LEVEL1_WIDTH * 2 - 1,
                         MAP_LEVEL1_HEIGHT * 2 - 1);
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
  MAP_init();
  GAMEOBJECT_init(&goblin, &goblin1, PLAYER_PAL, 3 * 2, 5 * 2);

  SYS_doVBlankProcess();

  u16 frame = 0;
  while (TRUE) {
    if (frame % 120 == 0) {
      MAP_updateTile();
      frame = 0;
    }

    // update hardware sprites table
    SPR_update();

    // wait for VBLANK
    SYS_doVBlankProcess();
    frame++;
  }

  return 0;
}

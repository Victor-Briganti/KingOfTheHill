#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include <genesis.h>
#include <gfx.h>
#include <global.h>

inline void BACKGROUND_initTransition(const Image *image) {
  // Draw background
  VDP_drawImageEx(
      BACKGROUND_PLANE, image,
      TILE_ATTR_FULL(BACKGROUND_PAL, 0, FALSE, FALSE, TILE_USER_INDEX), 0, 0,
      true, DMA);
}

inline void BACKGROUND_initImage(const Image *image) {
  // Draw background
  VDP_drawImageEx(
      BACKGROUND_PLANE, image,
      TILE_ATTR_FULL(BACKGROUND_PAL, 0, FALSE, FALSE, TILE_USER_INDEX), 0, 0,
      true, DMA);
  tile_index += background_level.tileset->numTile;
}

inline void BACKGROUND_init() {
  // Draw background
  VDP_drawImageEx(BACKGROUND_PLANE, &background_level,
                  TILE_ATTR_FULL(BACKGROUND_PAL, 0, FALSE, FALSE, tile_index),
                  0, 0, true, DMA);

  tile_index += background_level.tileset->numTile;

  // Render level info
  VDP_setTextPalette(BACKGROUND_PAL);
  VDP_setTextPlane(BACKGROUND_PLANE);
}

inline void BACKGROUND_setText(const char *const string) {
  VDP_drawText(string, LEVEL_TEXT_X, LEVEL_TEXT_Y);
}

inline void BACKGROUND_release() { tile_index = TILE_USER_INDEX; }

#endif // __BACKGROUND_H__

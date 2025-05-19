#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include <genesis.h>
#include <gfx.h>
#include <global.h>

inline void BACKGROUND_init() {
  // Draw background
  VDP_drawImageEx(BACKGROUND_PLANE, &background_level,
                  TILE_ATTR_FULL(BACKGROUND_PAL, 0, FALSE, FALSE, 1), 0, 0,
                  true, DMA);

  // Render level info
  VDP_setTextPalette(BACKGROUND_PAL);
  VDP_setTextPlane(BACKGROUND_PLANE);
}

inline void BACKGROUND_setScore(const u16 score) {
  char scoreText[6];
  sprintf(scoreText, "%d", score);
  VDP_drawText(scoreText, SCORE_TEXT_X, SCORE_TEXT_Y);
}

inline void BACKGROUND_setText(const char *const string) {
  VDP_drawText(string, LEVEL_TEXT_X, LEVEL_TEXT_Y);
}

#endif // __BACKGROUND_H__

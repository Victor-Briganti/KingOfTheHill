#include "scene/game_over.h"
#include "background/background.h"
#include "gfx.h"
#include "global.h"
#include "maths.h"
#include "scene/scene_manager.h"
#include "sys.h"

//===----------------------------------------------------------------------===//
// GLOBALS
//===----------------------------------------------------------------------===//

Scene gameOver = {GAME_OVER_init, GAME_OVER_update, GAME_OVER_hit,
                  GAME_OVER_destroy};

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

static inline void initBackground() {
  VDP_init();
  BACKGROUND_initImage(&background_gameover);
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void GAME_OVER_init() {
  initBackground();
  SYS_doVBlankProcess();
}

u16 count = 0;
SceneId GAME_OVER_update() {
  if (frame % FRAME_ANIMATION == 0) {
    if (count == 4096) {
      count = 0;
      return SCENE_ID_LEVEL01;
    }

    count++;
  }

  return SCENE_ID_GAME_OVER;
}

void GAME_OVER_hit(const Vect2D_s16 hitPos) {}

void GAME_OVER_destroy() {
  SYS_reset();
}

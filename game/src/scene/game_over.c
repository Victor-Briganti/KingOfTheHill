#include "scene/game_over.h"
#include "background/background.h"
#include "global.h"
#include "scene/scene_manager.h"

#include <gfx.h>
#include <maths.h>
#include <sfx.h>
#include <snd/xgm.h>
#include <sys.h>

//===----------------------------------------------------------------------===//
// GLOBALS
//===----------------------------------------------------------------------===//

Scene gameOver = {GAME_OVER_init, GAME_OVER_update, GAME_OVER_hit,
                  GAME_OVER_destroy};

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

static inline void initBackground() {
  XGM_stopPlay();
  XGM_setLoopNumber(0);
  XGM_startPlay(&death_music);
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
  JOY_update();

  if (JOY_readJoypad(JOY_1) & BUTTON_START) {
    return SCENE_ID_LEVEL01;
  }

  if (frame % 32 == 0) {
    if (count == 4096) {
      count = 0;
      return SCENE_ID_LEVEL01;
    }

    count++;
  }

  return SCENE_ID_GAME_OVER;
}

void GAME_OVER_hit(const Vect2D_s16 hitPos) {}

void GAME_OVER_destroy() { SYS_reset(); }

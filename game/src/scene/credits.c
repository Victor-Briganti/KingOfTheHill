#include "scene/credits.h"
#include "background/background.h"
#include "gfx.h"
#include "maths.h"
#include "scene/scene_manager.h"
#include "sys.h"

//===----------------------------------------------------------------------===//
// GLOBALSs
//===----------------------------------------------------------------------===//

Scene credits = {CREDITS_init, CREDITS_update, CREDITS_hit, CREDITS_destroy};

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

static inline void initBackground() {
  VDP_init();
  BACKGROUND_initImage(&background_credits);
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void CREDITS_init() {
  XGM_stopPlay();
  initBackground();
  SYS_doVBlankProcess();
}

SceneId CREDITS_update() {
  JOY_update();

  if (JOY_readJoypad(JOY_1) != 0) {
    return SCENE_ID_INIT;
  }

  return SCENE_ID_CREDITS;
}

void CREDITS_hit(const Vect2D_s16 hitPos) {}

void CREDITS_destroy() { SYS_reset(); }

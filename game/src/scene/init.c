#include "scene/init.h"
#include "background/background.h"
#include "gfx.h"
#include "maths.h"
#include "scene/scene_manager.h"
#include "sys.h"

//===----------------------------------------------------------------------===//
// GLOBALS
//===----------------------------------------------------------------------===//

Scene init = {INIT_init, INIT_update, INIT_hit, INIT_destroy};

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

static inline void initBackground() {
  VDP_init();
  BACKGROUND_initImage(&background_init);
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void INIT_init() {
  initBackground();
  SYS_doVBlankProcess();
}

SceneId INIT_update() {
  JOY_update();

  if (JOY_readJoypad(JOY_1) & BUTTON_START) {
    return SCENE_ID_LEVEL01;
  }

  return SCENE_ID_INIT;
}

void INIT_hit(const Vect2D_s16 hitPos) {}

void INIT_destroy() { BACKGROUND_release(); }

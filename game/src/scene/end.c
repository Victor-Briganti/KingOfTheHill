#include "scene/end.h"
#include "background/background.h"
#include "gfx.h"
#include "maths.h"
#include "scene/scene_manager.h"
#include "sys.h"

//===----------------------------------------------------------------------===//
// GLOBALS
//===----------------------------------------------------------------------===//

Scene end = {END_init, END_update, END_hit, END_destroy};

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

static inline void initBackground() {
  VDP_init();
  BACKGROUND_initImage(&background_end);
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void END_init() {
  initBackground();
  SYS_doVBlankProcess();
}

SceneId END_update() {
  JOY_update();

  if (JOY_readJoypad(JOY_1) != 0) {
    return SCENE_ID_INIT;
  }

  return SCENE_ID_END;
}

void END_hit(const Vect2D_s16 hitPos) {}

void END_destroy() { SYS_reset(); }

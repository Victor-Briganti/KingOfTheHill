#include "scene/movements.h"
#include "background/background.h"
#include "gfx.h"
#include "maths.h"
#include "scene/scene_manager.h"
#include "sys.h"

//===----------------------------------------------------------------------===//
// GLOBALSs
//===----------------------------------------------------------------------===//

Scene movements = {MOVEMENTS_init, MOVEMENTS_update, MOVEMENTS_hit, MOVEMENTS_destroy};

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

static inline void initBackground() {
  VDP_init();
  BACKGROUND_initImage(&background_movements);
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void MOVEMENTS_init() {
  XGM_stopPlay();
  initBackground();
  SYS_doVBlankProcess();
}

SceneId MOVEMENTS_update() {
  JOY_update();

  if (JOY_readJoypad(JOY_1) != 0) {
    return SCENE_ID_INIT;
  }

  return SCENE_ID_MOVEMENTS;
}

void MOVEMENTS_hit(const Vect2D_s16 hitPos) {}

void MOVEMENTS_destroy() { SYS_reset(); }

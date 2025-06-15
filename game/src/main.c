#include "global.h"
#include "hud/heart.h"
#include "player/player.h"
#include "scene/scene_manager.h"

#include <genesis.h>
#include <gfx.h>
#include <joy.h>
#include <sprite_eng.h>
#include <sprites.h>
#include <sys.h>
#include <tools.h>
#include <vdp.h>

//===----------------------------------------------------------------------===//
// GLOBALS
//===----------------------------------------------------------------------===//

u16 frame = 0;
u16 tile_index = TILE_USER_INDEX;

//===----------------------------------------------------------------------===//
// AUXILIARY
//===----------------------------------------------------------------------===//

static void COMMON_init() {
  // Set the resolution of the screen
  VDP_setScreenWidth320();
  VDP_setScreenHeight224();

  // Init the sprite and input engine
  SPR_init();
  JOY_init();

  // Init the player object
  PLAYER_init();
  HEART_init();
}

//===----------------------------------------------------------------------===//
// MAIN
//===----------------------------------------------------------------------===//

int main(const bool resetType) {
  if (!resetType) {
    SYS_hardReset();
  }

  COMMON_init();
  // SYS_showFrameLoad(true);
  while (TRUE) {
    SYS_doVBlankProcess();

    sceneManager[sceneIndex]->init();
    while (TRUE) {
      SceneId result = sceneManager[sceneIndex]->update();
      if (result != sceneIndex) {
        sceneManager[sceneIndex]->destroy();
        sceneIndex = result;
        break;
      }

      frame++;
    }

    frame = 0;
  }
  return 0;
}

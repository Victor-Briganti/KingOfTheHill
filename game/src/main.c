#include "enemy/pawn.h"
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

u16 mapLevelHeight;
u16 mapLevelWidth;

u16 mapLevelX;
u16 mapLevelY;

s16 playerInitX;
s16 playerInitY;

GameTurn turn;

u16 frame = 0;

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
  PAWN_init();
}

//===----------------------------------------------------------------------===//
// MAIN
//===----------------------------------------------------------------------===//

int main(const bool resetType) {
  if (!resetType) {
    SYS_hardReset();
  }

  COMMON_init();
  while (TRUE) {
    SYS_doVBlankProcess();

    if (!player.health)
      continue;

    scene.init();
    while (TRUE) {
      s8 result = scene.update();
      if (result < 0)
        break;
      
      frame++;
    }

    scene.destroy();
    frame = 0;
  }
  return 0;
}

#include "global.h"
#include "hud/heart.h"
#include "player/player.h"
#include "scene/scene_manager.h"

#include <genesis.h>
#include <gfx.h>
#include <joy.h>
#include <sfx.h>
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

  // Init the sounds
  XGM_setPCM(ID_MOVE_SOUND, &move_sound, LEN_MOVE_SOUND);
  XGM_setPCM(ID_DEATH_SOUND, &death_sound, LEN_DEATH_SOUND);
  XGM_setPCM(ID_ATTACK_SOUND, &attack_sound, LEN_ATTACK_SOUND);
  XGM_setPCM(ID_TRANSFORM_SOUND, &transform_sound, LEN_TRANFORM_SOUND);
  XGM_setPCM(ID_RESURRECT_SOUND, &resurrect_sound, LEN_RESURRECT_SOUND);
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

    sceneManager[sceneIndex]->init();
    while (TRUE) {
      SceneId result = sceneManager[sceneIndex]->update();
      if (result != sceneIndex) {
        sceneManager[sceneIndex]->destroy();
        sceneIndex = result;
        break;
      }

      if (!XGM_isPlaying()) {
        switch (sceneIndex) {
        case SCENE_ID_GAME_OVER:
          XGM_startPlay(&death_music);
          break;
        case SCENE_ID_END:
          XGM_startPlay(&win_music);
          break;
        default:
          XGM_startPlay(&background_music);
        }
      }

      frame++;
    }

    frame = 0;
  }
  return 0;
}

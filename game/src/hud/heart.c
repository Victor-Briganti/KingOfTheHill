#include "hud/heart.h"
#include "gameobject/gameobject.h"
#include "global.h"
#include "player/player.h"
#include "sprites.h"

Heart heart;

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void HEART_init() {
  heart.status[0] = FULL;
  heart.status[1] = FULL;
  heart.status[2] = FULL;

  GAMEOBJECT2_init(&heart.object[0], &heart_full, ENEMY_PAL, 300, 1);
  GAMEOBJECT2_init(&heart.object[1], &heart_full, ENEMY_PAL, 280, 1);
  GAMEOBJECT2_init(&heart.object[2], &heart_full, ENEMY_PAL, 260, 1);
}

void HEART_update() {
  GAMEOBJECT_releaseSprite(&heart.object[0]);
  GAMEOBJECT_releaseSprite(&heart.object[1]);
  GAMEOBJECT_releaseSprite(&heart.object[2]);

  if (player.health == 5) {
    GAMEOBJECT2_init(&heart.object[0], &heart_half, ENEMY_PAL, 300, 1);
    GAMEOBJECT2_init(&heart.object[1], &heart_full, ENEMY_PAL, 280, 1);
    GAMEOBJECT2_init(&heart.object[2], &heart_full, ENEMY_PAL, 260, 1);
  } else if (player.health == 4) {
    GAMEOBJECT2_init(&heart.object[0], &heart_empty, ENEMY_PAL, 300, 1);
    GAMEOBJECT2_init(&heart.object[1], &heart_full, ENEMY_PAL, 280, 1);
    GAMEOBJECT2_init(&heart.object[2], &heart_full, ENEMY_PAL, 260, 1);
  } else if (player.health == 3) {
    GAMEOBJECT2_init(&heart.object[0], &heart_empty, ENEMY_PAL, 300, 1);
    GAMEOBJECT2_init(&heart.object[1], &heart_half, ENEMY_PAL, 280, 1);
    GAMEOBJECT2_init(&heart.object[2], &heart_full, ENEMY_PAL, 260, 1);
  } else if (player.health == 2) {
    GAMEOBJECT2_init(&heart.object[0], &heart_empty, ENEMY_PAL, 300, 1);
    GAMEOBJECT2_init(&heart.object[1], &heart_empty, ENEMY_PAL, 280, 1);
    GAMEOBJECT2_init(&heart.object[2], &heart_full, ENEMY_PAL, 260, 1);
  } else if (player.health == 1) {
    GAMEOBJECT2_init(&heart.object[0], &heart_empty, ENEMY_PAL, 300, 1);
    GAMEOBJECT2_init(&heart.object[1], &heart_empty, ENEMY_PAL, 280, 1);
    GAMEOBJECT2_init(&heart.object[2], &heart_half, ENEMY_PAL, 260, 1);
  } else if (player.health == 0) {
    GAMEOBJECT2_init(&heart.object[0], &heart_empty, ENEMY_PAL, 300, 1);
    GAMEOBJECT2_init(&heart.object[1], &heart_empty, ENEMY_PAL, 280, 1);
    GAMEOBJECT2_init(&heart.object[2], &heart_empty, ENEMY_PAL, 260, 1);
  }
}

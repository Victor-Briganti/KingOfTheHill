#include "hud/heart.h"
#include "genesis.h"
#include "global.h"
#include "node/overlay.h"
#include "player/player.h"
#include "sprites.h"

Heart heart;

#define MAX_HEARTS 3

static const s16 heartX[MAX_HEARTS] = {300, 280, 260};
static const s16 heartY = 1;

//===----------------------------------------------------------------------===//
// PRIVATE
//===----------------------------------------------------------------------===//

static const SpriteDefinition *getHeartSprite(const s8 heartIndex) {
  const u8 remaining = player.totalHealth - player.health;

  if (remaining - (heartIndex * 2) <= 0)
    return &heart_full;

  if (remaining - (heartIndex * 2) <= 1)
    return &heart_half;

  return &heart_empty;
}

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void HEART_init() {
  for (u8 i = 0; i < MAX_HEARTS; i++) {
    heart.status[i] = FULL;

    OVERLAY_init(&heart.node[i], &heart_full, ENEMY_PAL, heartX[i], heartY);
  }
}

void HEART_update() {
  for (u8 i = 0; i < MAX_HEARTS; i++) {
    OVERLAY_releaseSprite(&heart.node[i]);
    OVERLAY_init(&heart.node[i], getHeartSprite(i), ENEMY_PAL, heartX[i],
                 heartY);
  }
}

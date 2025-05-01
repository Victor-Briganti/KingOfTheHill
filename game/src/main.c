#include <genesis.h>
#include <sprite_eng.h>

#include "resources.h"
#include "tools.h"

void background_init() {
    VDP_setScreenWidth320();
    SPR_init();

    VDP_drawImageEx(BG_B, &background, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 0, 0,
                    true, DMA);
}

void text_init() {
    // Text rendering
    char score[6];
    sprintf(score, "%d", 65443);
    VDP_drawText(score, 0, 1);
    VDP_drawText("LEVEL 1-1", 13, 1);
}

Sprite *heart_full_init() {
    PAL_setPalette(PAL1, heart_full.palette->data, DMA);
    return SPR_addSprite(&heart_full, 0, 0,
                         TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, 1));
}

Sprite *heart_half_init() {
    PAL_setPalette(PAL1, heart_half.palette->data, DMA);
    return SPR_addSprite(&heart_half, 0, 0,
                         TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, 1));
}

Sprite *heart_empty_init() {
    PAL_setPalette(PAL1, heart_empty.palette->data, DMA);
    return SPR_addSprite(&heart_empty, 0, 0,
                         TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, 1));
}

int main(bool resetType) {
    // Soft reset doesn't clear RAM. Can lead to bugs.
    if (!resetType) {
        SYS_hardReset();
    }

    background_init();
    text_init();
    Sprite *sprite1 = heart_full_init();
    Sprite *sprite2 = heart_half_init();
    Sprite *sprite3 = heart_empty_init();
    SYS_doVBlankProcess();

    while (TRUE) {
        SPR_setPosition(sprite1, 300, 5);
        SPR_setPosition(sprite2, 282, 5);
        SPR_setPosition(sprite3, 264, 5);

        // update hardware sprites table
        SPR_update();

        // wait for VBLANK
        SYS_doVBlankProcess();
    }

    return 0;
}

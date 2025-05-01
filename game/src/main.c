#include <genesis.h>
#include <sprite_eng.h>

#include "resources.h"

void background_init() {
    VDP_setScreenWidth320();
    SPR_init();

    VDP_drawImageEx(BG_A, &background, TILE_ATTR_FULL(PAL3, 0, 0, 0, 1), 0, 0,
                    true, DMA);
}

int main(bool resetType) {
    // Soft reset doesn't clear RAM. Can lead to bugs.
    if (!resetType) {
        SYS_hardReset();
    }

    background_init();
    SYS_doVBlankProcess();

    while (TRUE) {
        // update hardware sprites table
        SPR_update();

        // wait for VBLANK
        SYS_doVBlankProcess();
    }

    return 0;
}

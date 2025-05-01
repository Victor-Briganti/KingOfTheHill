#include <genesis.h>
#include <sprite_eng.h>

#include "resources.h"
#include "tools.h"

void background_init() {
    VDP_setScreenWidth320();
    SPR_init();

    VDP_drawImageEx(BG_B, &background, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), 0, 0,
                    true, DMA);
}

int main(bool resetType) {
    // Soft reset doesn't clear RAM. Can lead to bugs.
    if (!resetType) {
        SYS_hardReset();
    }

    VDP_drawText("000000", 0, 1);
    VDP_drawText("LEVEL 1-1", 13, 1);
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

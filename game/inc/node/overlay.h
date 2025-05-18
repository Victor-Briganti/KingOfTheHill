#ifndef __OVERLAY_H__
#define __OVERLAY_H__

#include <genesis.h>
#include <maths.h>

typedef struct OverlayNode {
    Sprite *sprite; // Current node position in tiles (for animations)
    Vect2D_s16 pixel; // Current node position in tiles (for animations)
} OverlayNode;

inline void OVERLAY_init(OverlayNode *const node,
                         const SpriteDefinition *sprite, u16 const palette, const s16 x,
                         const s16 y) {
    PAL_setPalette(palette, sprite->palette->data, DMA);
    node->sprite =
            SPR_addSprite(sprite, x, y, TILE_ATTR(palette, FALSE, FALSE, FALSE));

    node->pixel = (Vect2D_s16){x, y};
}

inline void OVERLAY_releaseSprite(const OverlayNode *const node) {
    SPR_releaseSprite(node->sprite);
}

#endif // __OVERLAY_H__

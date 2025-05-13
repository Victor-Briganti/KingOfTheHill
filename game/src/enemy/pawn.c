#include "enemy/pawn.h"

Pawn pawn;

void PAWN_init() {
    pawn.previousX = 0;
    pawn.previousY = 0;
}

void PAWN_update() {
}

void PAWN_levelInit(const SpriteDefinition *sprite, u16 palette, s16 x, s16 y) {
    GAMEOBJECT_init(&pawn.object, sprite, palette, x, y);
}

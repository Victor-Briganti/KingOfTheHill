#include "enemy/pawn.h"

Pawn pawn;

void PAWN_init() {
    pawn.previousX = 0;
    pawn.previousY = 0;
}

void PAWN_update() {
    if (turn == PLAYER) 
        return;

    pawn.previousX = pawn.object.x;
    pawn.previousY = pawn.object.y;
    pawn.object.y += 2;
    pawn.object.y = clamp(pawn.object.y, 0, mapLevelHeight - 2);
    turn = PLAYER;

    GAMEOBJECT_updatePos(&pawn.object);
}

void PAWN_levelInit(const SpriteDefinition *sprite, u16 palette, s16 x, s16 y) {
    GAMEOBJECT_init(&pawn.object, sprite, palette, x, y);
}

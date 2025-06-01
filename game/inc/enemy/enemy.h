#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <genesis.h>

#include "node/actor.h"

typedef enum EnemyType {
    PAWN_TYPE,
    QUEEN_TYPE,
    TOWER_TYPE,
    BISHOP_TYPE,
} EnemyType;

typedef enum EnemyState {
    ENEMY_IDLE,
    ENEMY_MOVING,
    ENEMY_ANIMATING,
    ENEMY_DEAD,
    ENEMY_DESTROYED,
} EnemyState;

typedef struct Enemy Enemy;

typedef void (*EnemyDealloc)(const Enemy *const );
typedef void (*EnemyDestroy)(Enemy *const enemy);
typedef s8 (*EnemyUpdate)(Enemy*);

typedef struct Enemy {
    ActorNode actor;
    EnemyType type;
    EnemyState state;
    u8 indexSprite;
    EnemyDealloc dealloc;
    EnemyDestroy destroy;
    EnemyUpdate update;
} Enemy;

void ENEMY_init(Enemy *enemy, const EnemyType type, const s16 x, const s16 y);

#endif // __ENEMY_H__

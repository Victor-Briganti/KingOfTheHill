#ifndef __MAP_H__
#define __MAP_H__

#include "types/collision.h"

#include <genesis.h>

#define MAP_MAX_HEIGHT 24 /* TODO: Change this */
#define MAP_MAX_WIDTH 24  /* TODO: Change this */

extern u16 map[MAP_MAX_HEIGHT][MAP_MAX_WIDTH];

typedef struct MapObject {
  u16 x, y;
  CollisionType object;
} MapObject;

inline void MAP_initLevel(const u16 mapHeight, const u16 mapWidth) {
  if (mapHeight > MAP_MAX_HEIGHT && mapWidth > MAP_MAX_WIDTH)
    kprintf("Map is bigger than supported");

  for (u16 i = 0; i < mapHeight; i++) {
    for (u16 j = 0; j < mapWidth; j++) {
      map[i][j] = 0;
    }
  }
}

inline Vect2D_s16 MAP_checkVertical(const Vect2D_s16 origin,
                                    const Vect2D_s16 dest) {
  if (origin.y < dest.y) {
    // Verify every position below
    for (s16 i = origin.y + 1; i <= dest.y; i++) {
      if (map[i][origin.x] != COLLISION_TYPE_EMPTY) {
        return (Vect2D_s16){origin.x, i};
      }
    }
  } else {
    // Verify every position above
    for (s16 i = origin.y - 1; i >= dest.y; i--) {
      if (map[i][origin.x] != COLLISION_TYPE_EMPTY) {
        return (Vect2D_s16){origin.x, i};
      }
    }
  }

  return (Vect2D_s16){-1, -1};
}

inline Vect2D_s16 MAP_checkHorizontal(const Vect2D_s16 origin,
                                      const Vect2D_s16 dest) {
  if (origin.x < dest.x) {
    // Verify every position below
    for (s16 i = origin.x + 1; i <= dest.x; i++) {
      if (map[origin.y][i] != COLLISION_TYPE_EMPTY) {
        return (Vect2D_s16){i, origin.y};
      }
    }
  } else {
    // Verify every position above
    for (s16 i = origin.x - 1; i >= dest.x; i--) {
      if (map[origin.y][i] != COLLISION_TYPE_EMPTY) {
        return (Vect2D_s16){i, origin.y};
      }
    }
  }

  return (Vect2D_s16){-1, -1};
}

inline Vect2D_s16 MAP_checkDiagonal(const Vect2D_s16 origin,
                                    const Vect2D_s16 dest) {
  if (origin.x > dest.x && origin.y > dest.y) {
    // Verify up left diagonal
    for (s16 x = origin.x - 1, y = origin.y - 1; x >= dest.x && y >= dest.y;
         x--, y--) {
      if (map[y][x] != COLLISION_TYPE_EMPTY) {
        return (Vect2D_s16){x, y};
      }
    }
  }

  if (origin.x < dest.x && origin.y > dest.y) {
    // Verify up right diagonal
    for (s16 x = origin.x + 1, y = origin.y - 1; x <= dest.x && y >= dest.y;
         x++, y--) {
      if (map[y][x] != COLLISION_TYPE_EMPTY) {
        return (Vect2D_s16){x, y};
      }
    }
  }

  if (origin.x > dest.x && origin.y < dest.y) {
    // Verify down left diagonal
    for (s16 x = origin.x - 1, y = origin.y + 1; x >= dest.x && y <= dest.y;
         x--, y++) {
      if (map[y][x] != COLLISION_TYPE_EMPTY) {
        return (Vect2D_s16){x, y};
      }
    }
  }

  if (origin.x < dest.x && origin.y < dest.y) {
    // Verify down right diagonal
    for (s16 x = origin.x + 1, y = origin.y + 1; x <= dest.x && y <= dest.y;
         x++, y++) {
      if (map[y][x] != COLLISION_TYPE_EMPTY) {
        return (Vect2D_s16){x, y};
      }
    }
  }

  return (Vect2D_s16){-1, -1};
}

inline void MAP_updateCollision(const Vect2D_s16 prev, const Vect2D_s16 cur,
                                const CollisionType colType) {
  if (prev.y < 0 && prev.x < 0)
    return;

  map[prev.y][prev.x] &= ~colType;
  map[cur.y][cur.x] |= colType;
}

inline void MAP_removeCollision(const Vect2D_s16 vec,
                                const CollisionType colType) {
  if (vec.y < 0 && vec.x < 0)
    return;

  map[vec.y][vec.x] &= ~colType;
}

#endif // __MAP_H__

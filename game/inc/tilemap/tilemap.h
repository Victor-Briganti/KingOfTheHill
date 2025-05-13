#ifndef __TILEMAP_H__
#define __TILEMAP_H__

#include "global.h"

#include <genesis.h>
#include <gfx.h>

//===----------------------------------------------------------------------===//
// TILESET
//===----------------------------------------------------------------------===//

// Position of each tile color
#define BLACK_TILE (0)
#define WHITE_TILE (1)
#define PURPLE_TILE (2)
#define GREEN_TILE (3)
#define RED_TILE (4)
#define BLUE_TILE (5)

//===----------------------------------------------------------------------===//
// TILEMAP
//===----------------------------------------------------------------------===//

inline void TILEMAP_init(const TileSet *tileset) {
  PAL_setPalette(TILEMAP_PAL, tileset_palette.data, DMA);
  VDP_loadTileSet(tileset, TILE_USER_INDEX, DMA);
}

inline void TILEMAP_update(const TileMap *const tilemap) {
  VDP_setTileMapEx(
      TILEMAP_PLANE, tilemap,
      TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, TILE_USER_INDEX), mapLevelX,
      mapLevelY, 0, 0, tilemap->w, tilemap->h, DMA);
}

inline void TILEMAP_updateRightTile(s16 x, s16 y, s16 mapPosX, s16 mapPosY,
                                    u16 tileIndex) {
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX + 2, y + mapPosY);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX + 3, y + mapPosY);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX + 2, y + mapPosY + 1);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX + 3, y + mapPosY + 1);
}

inline void TILEMAP_updateLeftTile(s16 x, s16 y, s16 mapPosX, s16 mapPosY,
                                   u16 tileIndex) {
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX - 1, y + mapPosY);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX - 2, y + mapPosY);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX - 1, y + mapPosY + 1);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX - 2, y + mapPosY + 1);
}

inline void TILEMAP_updateUpTile(s16 x, s16 y, s16 mapPosX, s16 mapPosY,
                                 u16 tileIndex) {
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX + 1, y + mapPosY - 1);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX, y + mapPosY - 1);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX + 1, y + mapPosY - 2);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX, y + mapPosY - 2);
}

inline void TILEMAP_updateBottomTile(s16 x, s16 y, s16 mapPosX, s16 mapPosY,
                                     u16 tileIndex) {
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX + 1, y + mapPosY + 2);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX, y + mapPosY + 2);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX + 1, y + mapPosY + 3);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX, y + mapPosY + 3);
}

inline void TILEMAP_updateUpRighTile(s16 x, s16 y, s16 mapPosX, s16 mapPosY,
                                     u16 tileIndex) {
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX + 2, y + mapPosY - 2);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX + 2, y + mapPosY - 1);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX + 3, y + mapPosY - 2);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX + 3, y + mapPosY - 1);
}

inline void TILEMAP_updateUpLeftTile(s16 x, s16 y, s16 mapPosX, s16 mapPosY,
                                     u16 tileIndex) {
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX - 1, y + mapPosY - 2);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX - 2, y + mapPosY - 1);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX - 2, y + mapPosY - 2);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX - 1, y + mapPosY - 1);
}

inline void TILEMAP_updateBottomRightTile(s16 x, s16 y, s16 mapPosX, s16 mapPosY,
                                          u16 tileIndex) {
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX + 2, y + mapPosY + 2);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX + 3, y + mapPosY + 2);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX + 2, y + mapPosY + 3);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapPosX + 3, y + mapPosY + 3);
}

inline void TILEMAP_updateBottomLeftTile(s16 x, s16 y, s16 mapPosX, s16 mapPosY,
                                         u16 tileIndex) {
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, GREEN_TILE),
                   x + mapPosX - 1, y + mapPosY + 2);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, GREEN_TILE),
                   x + mapPosX - 2, y + mapPosY + 2);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, GREEN_TILE),
                   x + mapPosX - 1, y + mapPosY + 3);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, GREEN_TILE),
                   x + mapPosX - 2, y + mapPosY + 3);
}

inline void TILEMAP_setTile(s16 x, s16 y, s16 mapPosX, s16 mapPosY,
                            u16 tileIndex) {
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapLevelX, y + mapLevelY);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapLevelX + 1, y + mapLevelY);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapLevelX, y + mapLevelY + 1);
  VDP_setTileMapXY(TILEMAP_PLANE,
                   TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex),
                   x + mapLevelX + 1, y + mapLevelY + 1);
}

#endif // __TILEMAP_H__

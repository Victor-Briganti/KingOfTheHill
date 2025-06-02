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

static const s16 TILE_OFFSET_RIGHT[4][2] = {{2, 0}, {3, 0}, {2, 1}, {3, 1}};

static const s16 TILE_OFFSET_LEFT[4][2] = {{-1, 0}, {-2, 0}, {-1, 1}, {-2, 1}};

static const s16 TILE_OFFSET_UP[4][2] = {{1, -1}, {0, -1}, {1, -2}, {0, -2}};

static const s16 TILE_OFFSET_DOWN[4][2] = {{1, 2}, {0, 2}, {1, 3}, {0, 3}};

static const s16 TILE_OFFSET_UP_RIGHT[4][2] = {
    {2, -2}, {2, -1}, {3, -2}, {3, -1}};

static const s16 TILE_OFFSET_UP_LEFT[4][2] = {
    {-1, -2}, {-2, -1}, {-2, -2}, {-1, -1}};

static const s16 TILE_OFFSET_DOWN_RIGHT[4][2] = {
    {2, 2}, {3, 2}, {2, 3}, {3, 3}};

static const s16 TILE_OFFSET_DOWN_LEFT[4][2] = {
    {-1, 2}, {-2, 2}, {-1, 3}, {-2, 3}};

static const s16 TILE_OFFSET_CENTER[4][2] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};

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

inline void TILEMAP_placeTiles(const s16 x, const s16 y, const s16 mapPosX,
                               const s16 mapPosY, const u16 tileIndex,
                               const s16 offsets[][2], const u16 count) {
  u16 attr = TILE_ATTR_FULL(TILEMAP_PAL, 0, FALSE, FALSE, tileIndex);
  for (u16 i = 0; i < count; i++)
    VDP_setTileMapXY(TILEMAP_PLANE, attr, x + mapPosX + offsets[i][0],
                     y + mapPosY + offsets[i][1]);
}

#define TILEMAP_updateRightTile(x, y, mapPosX, mapPosY, tileIndex)             \
  TILEMAP_placeTiles((x), (y), (mapPosX), (mapPosY), (tileIndex),              \
                     TILE_OFFSET_RIGHT, 4)

#define TILEMAP_updateLeftTile(x, y, mapPosX, mapPosY, tileIndex)              \
  TILEMAP_placeTiles((x), (y), (mapPosX), (mapPosY), (tileIndex),              \
                     TILE_OFFSET_LEFT, 4)

#define TILEMAP_updateUpTile(x, y, mapPosX, mapPosY, tileIndex)                \
  TILEMAP_placeTiles((x), (y), (mapPosX), (mapPosY), (tileIndex),              \
                     TILE_OFFSET_UP, 4)

#define TILEMAP_updateBottomTile(x, y, mapPosX, mapPosY, tileIndex)            \
  TILEMAP_placeTiles((x), (y), (mapPosX), (mapPosY), (tileIndex),              \
                     TILE_OFFSET_DOWN, 4)

#define TILEMAP_updateUpRightTile(x, y, mapPosX, mapPosY, tileIndex)           \
  TILEMAP_placeTiles((x), (y), (mapPosX), (mapPosY), (tileIndex),              \
                     TILE_OFFSET_UP_RIGHT, 4)

#define TILEMAP_updateUpLeftTile(x, y, mapPosX, mapPosY, tileIndex)            \
  TILEMAP_placeTiles((x), (y), (mapPosX), (mapPosY), (tileIndex),              \
                     TILE_OFFSET_UP_LEFT, 4)

#define TILEMAP_updateBottomRightTile(x, y, mapPosX, mapPosY, tileIndex)       \
  TILEMAP_placeTiles((x), (y), (mapPosX), (mapPosY), (tileIndex),              \
                     TILE_OFFSET_DOWN_RIGHT, 4)

#define TILEMAP_updateBottomLeftTile(x, y, mapPosX, mapPosY, tileIndex)        \
  TILEMAP_placeTiles((x), (y), (mapPosX), (mapPosY), (tileIndex),              \
                     TILE_OFFSET_DOWN_LEFT, 4)

#define TILEMAP_setTile(x, y, mapPosX, mapPosY, tileIndex)                     \
  TILEMAP_placeTiles((x), (y), (mapPosX), (mapPosY), (tileIndex),              \
                     TILE_OFFSET_CENTER, 4)

#endif // __TILEMAP_H__

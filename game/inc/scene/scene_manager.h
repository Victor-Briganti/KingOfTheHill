#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include <genesis.h>

#define MAX_SCENE 20

//===----------------------------------------------------------------------===//
// TYPE DEFINITION
//===----------------------------------------------------------------------===//

typedef enum SceneId {
  SCENE_ID_INIT = 0,
  SCENE_ID_GAME_OVER = 1,
  SCENE_ID_END = 2,
  SCENE_ID_CREDITS = 3,
  SCENE_ID_MOVEMENTS = 4,
  SCENE_ID_LEVEL01 = 5,
  SCENE_ID_LEVEL02 = 6,
  SCENE_ID_LEVEL03 = 7,
  SCENE_ID_LEVEL04 = 8,
  SCENE_ID_LEVEL05 = 9,
  SCENE_ID_LEVEL06 = 10,
  SCENE_ID_LEVEL07 = 11,
  SCENE_ID_LEVEL08 = 12,
  SCENE_ID_LEVEL09 = 13,
  SCENE_ID_LEVEL10 = 14,
  SCENE_ID_LEVEL11 = 15,
  SCENE_ID_LEVEL12 = 16,
  SCENE_ID_LEVEL13 = 17,
  SCENE_ID_LEVEL14 = 18,
  SCENE_ID_LEVEL15 = 19,
} SceneId;

typedef void (*SceneInitFunction)(void);
typedef SceneId (*SceneUpdateFunction)(void);
typedef void (*SceneHitFunction)(Vect2D_s16);
typedef void (*SceneDestroyFunction)(void);

typedef struct Scene {
  SceneInitFunction init;
  SceneUpdateFunction update;
  SceneHitFunction hit;
  SceneDestroyFunction destroy;
} Scene;

//===----------------------------------------------------------------------===//
// EXTERN
//===----------------------------------------------------------------------===//

// Array with all the possible scenes of the game
extern Scene *sceneManager[MAX_SCENE];

// Index of the current scene on the sceneManager
extern SceneId sceneIndex;

// Current Level Size Information
extern u16 mapLevelHeight;
extern u16 mapLevelWidth;

// Current Map Level Size Information
extern u16 mapLevelX;
extern u16 mapLevelY;

// Current Level Player Initial Position
extern s16 playerInitX;
extern s16 playerInitY;

#endif // __SCENE_MANAGER_H__

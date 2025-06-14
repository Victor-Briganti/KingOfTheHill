#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include <genesis.h>

#define MAX_SCENE 17

//===----------------------------------------------------------------------===//
// TYPE DEFINITION
//===----------------------------------------------------------------------===//

typedef enum SceneId {
  SCENE_ID_INIT = 0,
  SCENE_ID_GAME_OVER = 1,
  SCENE_ID_LEVEL01 = 2,
  SCENE_ID_LEVEL02 = 3,
  SCENE_ID_LEVEL03 = 4,
  SCENE_ID_LEVEL04 = 5,
  SCENE_ID_LEVEL05 = 6,
  SCENE_ID_LEVEL06 = 7,
  SCENE_ID_LEVEL07 = 8,
  SCENE_ID_LEVEL08 = 9,
  SCENE_ID_LEVEL09 = 10,
  SCENE_ID_LEVEL10 = 11,
  SCENE_ID_LEVEL11 = 12,
  SCENE_ID_LEVEL12 = 13,
  SCENE_ID_LEVEL13 = 14,
  SCENE_ID_LEVEL14 = 15,
  SCENE_ID_LEVEL15 = 16,
  SCENE_ID_PASSED = 999,
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

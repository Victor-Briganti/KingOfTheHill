#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include <genesis.h>

#define MAX_SCENE 1

//===----------------------------------------------------------------------===//
// TYPE DEFINITION
//===----------------------------------------------------------------------===//

typedef void (*SceneInitFunction)(void);
typedef s8 (*SceneUpdateFunction)(void);
typedef void (*SceneHitFunction)(void);
typedef void (*SceneDestroyFunction)(void);

typedef struct Scene {
  SceneInitFunction init;
  SceneUpdateFunction update;
  SceneHitFunction hit;
  SceneDestroyFunction destroy;
} Scene;

typedef enum SceneId {
  SCENE_ID_LEVEL01 = 0,
} SceneId;

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

#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include <genesis.h>

#define MAX_SCENE 1

typedef void (*SceneInitFunction)(void);
typedef s8 (*SceneUpdateFunction)(void);
typedef void (*SceneDestroyFunction)(void);

typedef struct Scene {
  SceneInitFunction init;
  SceneUpdateFunction update;
  SceneDestroyFunction destroy;
} Scene;

void SCENE1_init();
s8 SCENE1_update();
void SCENE1_destroy();

extern Scene scene;

#endif // __SCENE_MANAGER_H__

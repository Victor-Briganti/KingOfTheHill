#include "scene/scene_manager.h"
#include "scene/scene1.h"
#include "scene/scene2.h"
#include "scene/scene3.h"

Scene *sceneManager[MAX_SCENE] = {
    &scene1,
    &scene2,
    &scene3,
};

SceneId sceneIndex = SCENE_ID_LEVEL03;

u16 mapLevelHeight;
u16 mapLevelWidth;

u16 mapLevelX;
u16 mapLevelY;

s16 playerInitX;
s16 playerInitY;

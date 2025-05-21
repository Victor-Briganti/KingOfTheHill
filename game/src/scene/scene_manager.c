#include "scene/scene_manager.h"
#include "scene/scene1.h"

Scene *sceneManager[MAX_SCENE] = {
    &scene1,
};

SceneId sceneIndex = SCENE_ID_LEVEL01;

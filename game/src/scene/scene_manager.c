#include "scene/scene_manager.h"
#include "scene/scene1.h"
#include "scene/scene2.h"
#include "scene/scene3.h"
#include "scene/scene4.h"
#include "scene/scene5.h"
#include "scene/scene6.h"
#include "scene/scene7.h"
#include "scene/scene8.h"
#include "scene/scene9.h"
#include "scene/scene10.h"
#include "scene/scene11.h"

Scene *sceneManager[MAX_SCENE] = {&scene1, &scene2, &scene3, &scene4, &scene5,
                                  &scene6, &scene7, &scene8, &scene9, &scene10, &scene11};

SceneId sceneIndex = SCENE_ID_LEVEL11;

u16 mapLevelHeight;
u16 mapLevelWidth;

u16 mapLevelX;
u16 mapLevelY;

s16 playerInitX;
s16 playerInitY;

#include "scene/scene_manager.h"
#include "scene/end.h"
#include "scene/credits.h"
#include "scene/game_over.h"
#include "scene/init.h"
#include "scene/scene1.h"
#include "scene/scene10.h"
#include "scene/scene11.h"
#include "scene/scene12.h"
#include "scene/scene13.h"
#include "scene/scene14.h"
#include "scene/scene15.h"
#include "scene/scene2.h"
#include "scene/scene3.h"
#include "scene/scene4.h"
#include "scene/scene5.h"
#include "scene/scene6.h"
#include "scene/scene7.h"
#include "scene/scene8.h"
#include "scene/scene9.h"

Scene *sceneManager[MAX_SCENE] = {
    &init,    &gameOver, &end, &credits,    &scene1,  &scene2,  &scene3,
    &scene4,  &scene5,   &scene6,  &scene7,  &scene8,  &scene9,
    &scene10, &scene11,  &scene12, &scene13, &scene14, &scene15};

SceneId sceneIndex = SCENE_ID_INIT;

u16 mapLevelHeight;
u16 mapLevelWidth;

u16 mapLevelX;
u16 mapLevelY;

s16 playerInitX;
s16 playerInitY;

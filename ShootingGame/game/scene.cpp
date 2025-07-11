/**
 * @file scene.cpp
 * @brief 画面遷移制御
 * @author KOOLER FAN
 * @date 2025/07/10
 */

#include "scene.h"

#include "game.h"
#include "title.h"

static auto g_Scene = Scene::TITLE;
static Scene g_SceneNext = g_Scene;

void Scene_Initialize()
{
    switch (g_Scene)
    {
    case Scene::TITLE:
        Title_Initialize();
        break;
    case Scene::GAME:
        Game_Initialize();
        break;
    case Scene::RESULT:
        break;
    default:
        break;
    }
}

void Scene_Finalize()
{
    switch (g_Scene)
    {
    case Scene::TITLE:
        Title_Finalize();
        break;
    case Scene::GAME:
        Game_Finalize();
        break;
    case Scene::RESULT:
        break;
    default:
        break;
    }
}

void Scene_Update(double elapsed_time)
{
    switch (g_Scene)
    {
    case Scene::TITLE:
        Title_Update(elapsed_time);
        break;
    case Scene::GAME:
        Game_Update(elapsed_time);
        break;
    case Scene::RESULT:
        break;
    default:
        break;
    }
}

void Scene_Draw()
{
    switch (g_Scene)
    {
    case Scene::TITLE:
        Title_Draw();
        break;
    case Scene::GAME:
        Game_Draw();
        break;
    case Scene::RESULT:
        break;
    default:
        break;
    }
}

void Scene_Refresh()
{
    if (g_Scene != g_SceneNext)
    {
        Scene_Finalize();

        g_Scene = g_SceneNext;

        Scene_Initialize();
    }
}

void Scene_Change(Scene scene)
{
    g_SceneNext = scene;
}

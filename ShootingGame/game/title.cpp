/**
 * @file title.h
 * @brief タイトル制御
 * @author KOOLER FAN
 * @date 2025/07/11
 */

#include "title.h"

#include "direct3d.h"
#include "fade.h"
#include "key_logger.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"

static int g_TitleBgTexId = -1;

void Title_Initialize()
{
    g_TitleBgTexId = Texture_Load(L"assets/title.png");
}

void Title_Finalize()
{
    // Texture_AllRelease();
}

void Title_Update(double elapsed_time)
{
    if (KeyLogger_IsTrigger(KK_ENTER))
    {
        Fade_Start(1.0, true);
    }
    if (Fade_GetState() == FADE_STATE::FINISHED_OUT)
    {
        Scene_Change(Scene::GAME);
    }
}

void Title_Draw()
{
    Sprite_Draw(g_TitleBgTexId, 0.0f, 0.0f, Direct3D_GetBackBufferWidth(), Direct3D_GetBackBufferHeight());
}

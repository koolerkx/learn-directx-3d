/**
 * @file sprite_anim.cpp
 * @brief スプライトアニメーション描画
 * @author KOOLER FAN
 * @date 2025/06/17
 */
#include "sprite_anim.h"
#include "sprite.h"
#include "texture.h"

static int g_PatternMax = 8;
static int g_PatternNum = 0;
static int g_TextureId = 1;

void SpriteAnim_Initialize()
{
    g_TextureId = Texture_Load(L"kokosozai.png");
}

void SpriteAnim_Finalize(void)
{
}

void SpriteAnim_Update()
{
    g_PatternNum = (g_PatternNum + 1) % g_PatternMax;
}

void SpriteAnim_Draw()
{
    Sprite_Draw(g_TextureId, 512.0f, 512.0f, 32.0f * g_PatternNum, 32 * 3, 32.0f, 32.0f, 256.0f, 256.0f);
}

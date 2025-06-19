/**
* @file sprite_anim.cpp
 * @brief スプライトアニメーション描画
 * @author KOOLER FAN
 * @date 2025/06/17
 */
#include "sprite_anim.h"
#include "sprite.h"
#include "texture.h"
#include <DirectXMath.h>

#include "debug_ostream.h"
using namespace DirectX;

struct AnimPatternData
{
    int m_TextureId{1}; // テクスチャID
    int m_PatternMax{0}; // パターン数
    XMUINT2 m_StartPosition{0, 0}; // アニメーションのスタート座標
    XMUINT2 m_PatternSize{0, 0}; // 1パターンサイズ
    bool m_IsLooped{false}; // ループするか
};

struct AnimPlayData
{
    int m_PatternId{-1}; // アニメーションパターンID
    int m_PatternNum{0}; // 現在再生中のパターン番号
    double m_accumulated_time{0}; // 累積時間
};

static constexpr int ANIM_PATTERN_MAX = 128;
static AnimPatternData g_AnimPattern[ANIM_PATTERN_MAX];
static constexpr int ANIM_PLAY_MAX = 256;
static AnimPlayData g_AnimPlay[ANIM_PLAY_MAX];

void SpriteAnim_Initialize()
{
    // g_AnimPattern[0].m_TextureId = Texture_Load(L"kokosozai.png");;
    // g_AnimPattern[0].m_PatternMax = 13;
    // g_AnimPattern[0].m_StartPosition = {0, 96};
    // g_AnimPattern[0].m_PatternSize = {32, 32};
    //
    // g_AnimPlay[0].m_PatternId = 0;

    g_AnimPattern[0].m_TextureId = Texture_Load(L"kokosozai.png");
    g_AnimPattern[0].m_PatternMax = 13;
    g_AnimPattern[0].m_PatternSize = {32, 32};
    g_AnimPattern[0].m_StartPosition = {0, 0 * 32};
    g_AnimPattern[0].m_IsLooped = true;
    g_AnimPlay[0].m_PatternId = 0;

    g_AnimPattern[1].m_TextureId = Texture_Load(L"kokosozai.png");
    g_AnimPattern[1].m_PatternMax = 13;
    g_AnimPattern[1].m_PatternSize = {32, 32};
    g_AnimPattern[1].m_StartPosition = {0, 32};
    g_AnimPattern[1].m_IsLooped = true;
    g_AnimPlay[1].m_PatternId = 1;

    g_AnimPattern[2].m_TextureId = Texture_Load(L"kokosozai.png");
    g_AnimPattern[2].m_PatternMax = 4;
    g_AnimPattern[2].m_PatternSize = {32, 32};
    g_AnimPattern[2].m_StartPosition = {2 * 32, 5 * 32};
    g_AnimPattern[2].m_IsLooped = false;
    g_AnimPlay[2].m_PatternId = 2;
}

void SpriteAnim_Finalize(void)
{
}

void SpriteAnim_Update(double elapsed_time)
{
    for (int i = 0; i < 3; i++)
    {
        if (g_AnimPlay[i].m_accumulated_time >= 0.1)
        {
            const int pattern_id = g_AnimPlay[i].m_PatternId;
            AnimPatternData* pPatternData = &g_AnimPattern[pattern_id];

            g_AnimPlay[i].m_PatternNum++;

            if (g_AnimPlay[i].m_PatternNum >= pPatternData->m_PatternMax)
            {
                if (pPatternData->m_IsLooped)
                {
                    g_AnimPlay[i].m_PatternNum = 0;
                }
                else
                {
                    g_AnimPlay[i].m_PatternNum = pPatternData->m_PatternMax - 1;
                }
            }

            g_AnimPlay[i].m_accumulated_time -= 0.1;
        }
        g_AnimPlay[i].m_accumulated_time += elapsed_time;
    }
}

void SpriteAnim_Draw(int playid, float dx, float dy, float dw, float dh)
{
    const int pattern_id = g_AnimPlay[playid].m_PatternId;
    const int pattern_num = g_AnimPlay[playid].m_PatternNum;
    AnimPatternData* pPatternData = &g_AnimPattern[pattern_id];

    Sprite_Draw(
        pPatternData->m_TextureId,
        dx, dy,
        pPatternData->m_StartPosition.x + pattern_num * pPatternData->m_PatternSize.x,
        pPatternData->m_StartPosition.y,
        pPatternData->m_PatternSize.x,
        pPatternData->m_PatternSize.y,
        dw, dh
    );
}

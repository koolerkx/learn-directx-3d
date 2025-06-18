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
    g_AnimPattern[0].m_PatternMax = 8;
    g_AnimPattern[0].m_PatternSize = { 32, 32 };
    g_AnimPattern[0].m_StartPosition = { 0, 96 };
    g_AnimPlay[0].m_PatternId = 0;

    g_AnimPattern[1].m_TextureId = Texture_Load(L"kokosozai.png");
    g_AnimPattern[1].m_PatternMax = 13;
    g_AnimPattern[1].m_PatternSize = { 32, 32 };
    g_AnimPattern[1].m_StartPosition = { 0, 32 };
    g_AnimPlay[1].m_PatternId = 1;

}

void SpriteAnim_Finalize(void)
{
}

void SpriteAnim_Update(double elapsed_time)
{
    for (int i = 0; i < 2; i++)
    {
        if (g_AnimPlay[i].m_accumulated_time >= 0.1)
        {
            g_AnimPlay[i].m_PatternNum = (g_AnimPlay[i].m_PatternNum + 1) % g_AnimPattern[g_AnimPlay[i].m_PatternId].m_PatternMax;
            g_AnimPlay[i].m_accumulated_time = 0;
            hal::dout << i << " " << g_AnimPlay[i].m_PatternNum << std::endl;
        }
    g_AnimPlay[i].m_accumulated_time += elapsed_time;
    }
}

void SpriteAnim_Draw(int playid, float dx, float dy, float dw, float dh)
{
    int i = playid;

    Sprite_Draw(
        g_AnimPattern[g_AnimPlay[i].m_PatternId].m_TextureId,
        dx, dy,
        g_AnimPattern[g_AnimPlay[i].m_PatternId].m_StartPosition.x + g_AnimPlay[i].m_PatternNum * g_AnimPattern[g_AnimPlay[i].m_PatternId].m_PatternSize.x,
        g_AnimPattern[g_AnimPlay[i].m_PatternId].m_StartPosition.y,
        g_AnimPattern[g_AnimPlay[i].m_PatternId].m_PatternSize.x,
        g_AnimPattern[g_AnimPlay[i].m_PatternId].m_PatternSize.y,
        dw, dh
    );
}

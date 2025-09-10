/**
　* @file score.cpp
 * @brief スコアの管理
 * @author KOOLER FAN
 * @date 2025/07/02
 */
#include "score.h"

#include "sprite.h"
#include "texture.h"

static unsigned int g_Score = 0;
static unsigned int g_ViewScore = 0;

static int g_Digit = 0;
static int g_CounterStop = 1;

static float g_x = 0.0f;
static float g_y = 0.0f;

static int g_ScoreTexId = -1;

constexpr float SCORE_FONT_WIDTH = 32.0f;
constexpr float SCORE_FONT_HEIGHT = 32.0f;
constexpr float SCORE_FONT_DISPLAY_WIDTH = 64.0f;
constexpr float SCORE_FONT_DISPLAY_HEIGHT = 64.0f;
static void drawNumber(float x, float y, int number);

void Score_Initialize(float x, float y, int digit)
{
    g_Score = 0;
    g_Digit = digit;
    g_x = x;
    g_y = y;

    // カンストの得点を作る
    for (int i = 0; i < digit; i++)
    {
        g_CounterStop *= 10;
    }
    g_CounterStop -= 1;

    g_ScoreTexId = Texture_Load(L"assets/consolab_ascii_512.png");
}

void Score_Finalize()
{
}

void Score_Update()
{
    g_ViewScore = min(g_Score, g_ViewScore + 1);
}

void Score_Draw()
{
    int score = min(g_ViewScore, static_cast<unsigned int>(g_CounterStop));

    for (int i = 0; i < g_Digit; i++)
    {
        int n = score % 10;
        score /= 10;

        drawNumber(g_x + SCORE_FONT_WIDTH * (g_Digit - 1 - i), g_y, n);
    }
}

unsigned int Score_GetScore()
{
    return g_Score;
}

void Score_AddScore(int score)
{
    g_ViewScore = g_Score;
    g_Score += score;
}

void Score_Reset()
{
    g_Score = 0;
}

void drawNumber(float x, float y, int number)
{
    Sprite_Draw(g_ScoreTexId, x, y,
                SCORE_FONT_WIDTH * number, SCORE_FONT_HEIGHT,
                SCORE_FONT_WIDTH, SCORE_FONT_HEIGHT,
                SCORE_FONT_DISPLAY_WIDTH, SCORE_FONT_DISPLAY_HEIGHT);
}

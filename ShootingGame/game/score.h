/**
 * @file score.h
 * @brief スコアの管理
 * @author KOOLER FAN
 * @date 2025/07/02
 */

#ifndef SCORE_H
#define SCORE_H

void Score_Initialize(float x, float y, int digit);
void Score_Finalize();
void Score_Update();
void Score_Draw();

unsigned int Score_GetScore();
void Score_AddScore(int score);
void Score_Reset();

#endif // SCORE_H

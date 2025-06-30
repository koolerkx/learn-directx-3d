/**
 * @file game.h
 * @brief ゲームの処理
 * @author KOOLER FAN
 * @date 2025/07/01
 */
#ifndef GAME_H
#define GAME_H

void Game_Initialize();
void Game_Finalize();

void Game_Update(double elapsed_time);
void Game_Draw();

#endif

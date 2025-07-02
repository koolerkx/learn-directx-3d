/**
 * @file enemy_spawner.h
 * @brief 敵機の生成
 * @author KOOLER FAN
 * @date 2025/07/02
 */

#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include "enemy.h"

void EnemySpawner_Initialize(void);
void EnemySpawner_Finalize(void);

void EnemySpawner_Update(double elapsed_time);

void EnemySpawner_Create(const DirectX::XMFLOAT2& position, EnemyTypeID id, double spawn_time, double spawn_rate, int spawn_count);


#endif
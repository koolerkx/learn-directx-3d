/**
 * @file enemy.h
 * @brief 敵機の管理
 * @author KOOLER FAN
 * @date 2025/07/02
 */

#ifndef ENEMY_H
#define ENEMY_H

#include "DirectXTex.h"

void Enemy_Initialize(void);
void Enemy_Finalize(void);

void Enemy_Update(double elapsed_time);
void Enemy_Draw(void);

enum class EnemyTypeID : int {
    RED,
    GREEN,
    End
};
void Enemy_Create(const DirectX::XMFLOAT2& position, EnemyTypeID enemyTypeId);

#endif
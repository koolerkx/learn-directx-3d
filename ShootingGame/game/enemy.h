/**
 * @file enemy.h
 * @brief 敵機の管理
 * @author KOOLER FAN
 * @date 2025/07/02
 */

#ifndef ENEMY_H
#define ENEMY_H

#include "collision.h"
#include "DirectXTex.h"

static constexpr unsigned int ENEMY_MAX = 256;

void Enemy_Initialize(void);
void Enemy_Finalize(void);

void Enemy_Update(double elapsed_time);
void Enemy_Draw(void);

enum class EnemyTypeID : int
{
    RED,
    GREEN,
    End
};

void Enemy_Create(const DirectX::XMFLOAT2& position, EnemyTypeID enemyTypeId);

bool Enemy_IsEnable(int index);
Circle Enemy_GetCollision(int index);

void Enemy_Destroy(int index);

#endif

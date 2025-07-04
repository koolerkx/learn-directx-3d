/**
* @file bullet.h
 * @brief 弾の処理
 * @author KOOLER FAN
 * @date 2025/07/01
 */

#ifndef BULLET_H
#define BULLET_H

#include <DirectXMath.h>

#include "collision.h"

static constexpr unsigned int BULLET_MAX = 1024;

void Bullet_Initialize(void);
void Bullet_Finalize(void);

void Bullet_Update(double elapsed_time);
void Bullet_Draw(void);

void Bullet_Create(const DirectX::XMFLOAT2& position);

bool Bullet_IsEnable(int index);
Circle Bullet_GetCollision(int index);

void Bullet_Destroy(int index);

#endif

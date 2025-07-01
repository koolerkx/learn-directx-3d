/**
* @file bullet.h
 * @brief 弾の処理
 * @author KOOLER FAN
 * @date 2025/07/01
 */

#ifndef BULLET_H
#define BULLET_H

#include <DirectXMath.h>

void Bullet_Initialize(void);
void Bullet_Finalize(void);

void Bullet_Update(double elapsed_time);
void Bullet_Draw(void);

void Bullet_Create(DirectX::XMFLOAT2& position);

#endif
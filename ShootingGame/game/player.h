/**
* @file player.h
 * @brief プレイヤーの処理
 * @author KOOLER FAN
 * @date 2025/07/01
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <DirectXMath.h>

#include "collision.h"

void Player_Initialize(const DirectX::XMFLOAT2& position);
void Player_Finalize();

void Player_Update(double elapsed_time);
void Player_Draw();

bool Player_IsEnable();
Circle Player_GetCollision();
void Player_Destroy();

#endif // PLAYER_H

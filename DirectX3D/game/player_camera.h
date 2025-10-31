/**
 * @file player_camera.h
 * @brief プレイヤーカメラ制御
 * 
 * @author KOOLER FAN
 * @date 2025/10/31
 */

#ifndef PLAYER_CAMERA_H
#define PLAYER_CAMERA_H

#include <DirectXMath.h>

void Player_Camera_Initialize();

void Player_Camera_Finalize(void);
void Player_Camera_Update(double elapsed_time);

const DirectX::XMFLOAT4X4& Player_Camera_GetMatrix();
const DirectX::XMFLOAT4X4& Player_Camera_GetPerspectiveMatrix();

const DirectX::XMFLOAT3& Player_Camera_GetFront();
const DirectX::XMFLOAT3& Player_Camera_GetRight();
const DirectX::XMFLOAT3& Player_Camera_GetUp();

float Player_Camera_GetFov();
void Player_Camera_SetFov(float fov);

const DirectX::XMFLOAT3& Player_Camera_GetPosition();

#endif

/**
 * @file camera.h
 * @brief カメラ制御
 * 
 * @author KOOLER FAN
 * @date 2025/09/11
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>

void Camera_Initialize(void);
void Camera_Finalize(void);
void Camera_Update(double elapsed_time);

const DirectX::XMFLOAT4X4& Camera_GetMatrix();
const DirectX::XMFLOAT4X4& Camera_GetPerspectiveMatrix();

const DirectX::XMFLOAT3& Camera_GetFront();
const DirectX::XMFLOAT3& Camera_GetRight();
const DirectX::XMFLOAT3& Camera_GetUp();

const DirectX::XMFLOAT3& Camera_GetPosition();

#endif // CAMERA_H

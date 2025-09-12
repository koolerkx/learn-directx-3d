/**
 * @file camera.cpp
 * @brief カメラ制御
 * 
 * @author KOOLER FAN
 * @date 2025/09/11
 */


#include "camera.h"


#include <DirectXMath.h>
#include "direct3d.h"
#include "shader3d.h"

using namespace DirectX;

void Camera_Initialize() {}
void Camera_Finalize() {}
void Camera_Update()
{
    // 頂点シェーダーに変換行列を設定

    // パースペクティブ行列
    float fovAngleY = XMConvertToRadians(45.0f);
    float aspectRatio = static_cast<float>(Direct3D_GetBackBufferWidth()) / static_cast<float>(Direct3D_GetBackBufferHeight());
    float nearZ = 0.1f;
    float farZ = 10000.0f;

    XMMATRIX mtxPerspective = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
    Shader3D_SetProjectionMatrix(mtxPerspective);

    // ビュー座標変換行列
     FXMVECTOR cameraPosition = { 10.0f, 10.0f, 10.0f };
    constexpr FXMVECTOR cameraFocusPosition = { 0.0f, 0.0f, 0.0f };
    constexpr FXMVECTOR cameraUpDirection = { 0.0f, 1.0f, 0.0f };

    XMMATRIX mtxView = XMMatrixLookAtLH(cameraPosition, cameraFocusPosition, cameraUpDirection);
    Shader3D_SetViewMatrix(mtxView);
}

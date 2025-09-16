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
#include "key_logger.h"

using namespace DirectX;

static XMFLOAT3 g_CameraPosition{ 10.0f, 10.0f, 10.0f };
static XMFLOAT3 g_CameraFront{ 0.0f, 0.0f, 1.0f };
static XMFLOAT3 g_CameraUp{ 0.0f, 1.0f, 0.0f };
static XMFLOAT3 g_CameraRight{ 1.0f, 0.0f, 0.0f };

constexpr float CAMERA_MOVE_SPEED = 0.5f; // per seconds
constexpr float CAMERA_ROTATION_SPEED = XMConvertToRadians(45.0f); // per seconds

void Camera_Initialize()
{
    g_CameraPosition = { 10.0f, 10.0f, 10.0f };
    g_CameraFront = { 0.0f, 0.0f, 1.0f };
    g_CameraUp = { 0.0f, 1.0f, 0.0f };
    g_CameraRight = { 1.0f, 0.0f, 0.0f };
}

void Camera_Finalize() {}

void Camera_Update(double elapsed_time)
{
    XMVECTOR cameraPosition = XMLoadFloat3(&g_CameraPosition);
    XMVECTOR cameraFront = XMLoadFloat3(&g_CameraFront);
    XMVECTOR cameraUp = XMLoadFloat3(&g_CameraUp);
    XMVECTOR cameraRight = XMLoadFloat3(&g_CameraRight);
    
    // 移動
    if (KeyLogger_IsPressed(KK_W))
    {
        cameraPosition = cameraFront * CAMERA_MOVE_SPEED * elapsed_time;
    }
    if (KeyLogger_IsPressed(KK_A))
    {
        cameraPosition = -cameraRight * CAMERA_MOVE_SPEED * elapsed_time;
    }
    if (KeyLogger_IsPressed(KK_S))
    {
        cameraPosition = -cameraFront * CAMERA_MOVE_SPEED * elapsed_time;
    }
    if (KeyLogger_IsPressed(KK_D))
    {
        cameraPosition = cameraRight * CAMERA_MOVE_SPEED * elapsed_time;
    }
    
    XMStoreFloat3(&g_CameraPosition, cameraPosition);
    XMStoreFloat3(&g_CameraFront, cameraFront);
    XMStoreFloat3(&g_CameraUp, cameraUp);
    XMStoreFloat3(&g_CameraRight, cameraRight);
    
    // 頂点シェーダーに変換行列を設定
    // ビュー座標変換行列
    XMMATRIX mtxView = XMMatrixLookAtLH(
        cameraPosition,
        cameraPosition + cameraFront,
        cameraUp
        );
    Shader3D_SetViewMatrix(mtxView);

    // パースペクティブ行列
    float fovAngleY = XMConvertToRadians(45.0f);
    float aspectRatio = static_cast<float>(Direct3D_GetBackBufferWidth()) / static_cast<float>(Direct3D_GetBackBufferHeight());
    float nearZ = 0.1f;
    float farZ = 10000.0f;
    
    XMMATRIX mtxPerspective = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
    Shader3D_SetProjectionMatrix(mtxPerspective);
}

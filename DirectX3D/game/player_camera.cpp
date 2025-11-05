/**
 * @file player_camera.cpp
 * @brief プレイヤーカメラ制御
 * 
 * @author KOOLER FAN
 * @date 2025/10/31
 */

#include "player_camera.h"

#include <DirectXMath.h>

#include "direct3d.h"
#include "player.h"
#include "shader3d.h"
#include "shader_field.h"
using namespace DirectX;

static XMFLOAT3 g_CameraPosition = { 0.0f, 0.0f, 0.0f };
static XMFLOAT3 g_CameraFront = { 0.0f, 0.0f, 1.0f };
static XMFLOAT3 g_CameraUp = { 0.0f, 1.0f, 0.0f };
static XMFLOAT3 g_CameraRight = { 1.0f, 0.0f, 0.0f };

static float g_CameraFov = 60.0f; // in degree

static XMFLOAT4X4 g_CameraMatrix;
static XMFLOAT4X4 g_PerspectiveMatrix;

void Player_Camera_Initialize()
{}

void Player_Camera_Finalize() {}

void Player_Camera_Update(double elapsed_time)
{
    XMVECTOR target = XMLoadFloat3(&Player_GetPosition());
    XMVECTOR player_front = XMLoadFloat3(&Player_GetFront());

    constexpr float camera_distance = 5.0f;
    // XMVECTOR camera_position = target - (player_front * camera_distance) + XMVectorSetY({}, 2);

    // fixed direction
    XMVECTOR camera_position = XMLoadFloat3(&Player_GetPosition());
    camera_position += { 0.0f, 4.0f, -12.0f };

    // fixed position
    // XMVECTOR camera_position = XMVECTOR{0.0f, 5.0f, 8.0f};
    XMVECTOR camera_front = XMVector3Normalize(target - camera_position);
    XMVECTOR camera_up = XMLoadFloat3(&g_CameraUp);

    XMStoreFloat3(&g_CameraPosition, camera_position);
    XMStoreFloat3(&g_CameraFront, camera_front);

    // 頂点シェーダーに変換行列を設定
    // ビュー座標変換行列
    XMMATRIX mtxView = XMMatrixLookAtLH(
        camera_position,
        target,
        camera_up
        );

    // XMStoreFloat4x4(&g_CameraMatrix, mtxView);
    Shader3D_SetViewMatrix(mtxView); // TODO: deprecate
    Shader_Field_SetViewMatrix(mtxView);

    // パースペクティブ行列
    // float fovAngleY = XMConvertToRadians(g_CameraFov);
    float fovAngleY = 1.0f;
    float aspectRatio = static_cast<float>(Direct3D_GetBackBufferWidth()) / static_cast<float>(Direct3D_GetBackBufferHeight());
    float nearZ = 0.01f;
    float farZ = 10000.0f;

    XMMATRIX mtxPerspective = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);

    // XMStoreFloat4x4(&g_PerspectiveMatrix, mtxPerspective);
    Shader3D_SetProjectionMatrix(mtxPerspective); // TODO: deprecate
    Shader_Field_SetProjectionMatrix(mtxPerspective);
}

const XMFLOAT4X4& Player_Camera_GetMatrix()
{
    return g_CameraMatrix;
}

const XMFLOAT4X4& Player_Camera_GetPerspectiveMatrix()
{
    return g_PerspectiveMatrix;
}

const XMFLOAT3& Player_Camera_GetFront()
{
    return g_CameraFront;
}

const XMFLOAT3& Player_Camera_GetRight()
{
    return g_CameraRight;
}

const XMFLOAT3& Player_Camera_GetUp()
{
    return g_CameraUp;
}

float Player_Camera_GetFov()
{
    return g_CameraFov;
}

void Player_Camera_SetFov(float fov)
{
    g_CameraFov = fov;
}

const XMFLOAT3& Player_Camera_GetPosition()
{
    return g_CameraPosition;
}

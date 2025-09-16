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

static XMFLOAT3 g_CameraPosition = { 10.0f, 0.0f, 10.0f };
static XMFLOAT3 g_CameraFront = { 0.0f, 0.0f, 1.0f };
static XMFLOAT3 g_CameraUp = { 0.0f, 1.0f, 0.0f };
static XMFLOAT3 g_CameraRight = { 1.0f, 0.0f, 0.0f };

constexpr float CAMERA_MOVE_SPEED = 10.0f; // per seconds
constexpr float CAMERA_ROTATION_SPEED = XMConvertToRadians(45.0f); // per seconds

static XMFLOAT4X4 g_CameraMatrix;
static XMFLOAT4X4 g_PerspectiveMatrix;

void Camera_Initialize()
{
    g_CameraPosition = { 0.0f, 1.0f, -10.0f };
    g_CameraFront = { 0.0f, 0.0f, 1.0f };
    g_CameraUp = { 0.0f, 1.0f, 0.0f };
    g_CameraRight = { 1.0f, 0.0f, 0.0f };

    XMStoreFloat4x4(&g_CameraMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&g_PerspectiveMatrix, XMMatrixIdentity());
}

void Camera_Finalize() {}

void Camera_Update(double elapsed_time)
{
    const float _elapsed_time = static_cast<float>(elapsed_time);

    XMVECTOR cameraPosition = XMLoadFloat3(&g_CameraPosition);
    XMVECTOR cameraFront = XMLoadFloat3(&g_CameraFront);
    XMVECTOR cameraUp = XMLoadFloat3(&g_CameraUp);
    XMVECTOR cameraRight = XMLoadFloat3(&g_CameraRight);

    // 移動
    if (KeyLogger_IsPressed(KK_W))
    {
        cameraPosition += cameraFront * CAMERA_MOVE_SPEED * _elapsed_time;
    }
    if (KeyLogger_IsPressed(KK_A))
    {
        cameraPosition += -cameraRight * CAMERA_MOVE_SPEED * _elapsed_time;
    }
    if (KeyLogger_IsPressed(KK_S))
    {
        cameraPosition += -cameraFront * CAMERA_MOVE_SPEED * _elapsed_time;
    }
    if (KeyLogger_IsPressed(KK_D))
    {
        cameraPosition += cameraRight * CAMERA_MOVE_SPEED * _elapsed_time;
    }

    if (KeyLogger_IsPressed(KK_SPACE))
    {
        cameraPosition += cameraUp * CAMERA_MOVE_SPEED * _elapsed_time;
    }
    if (KeyLogger_IsPressed(KK_LEFTSHIFT))
    {
        cameraPosition += -cameraUp * CAMERA_MOVE_SPEED * _elapsed_time;
    }

    if (KeyLogger_IsPressed(KK_LEFTSHIFT))
    {
        cameraPosition += -cameraUp * CAMERA_MOVE_SPEED * _elapsed_time;
    }

    //回転
    if (KeyLogger_IsPressed(KK_DOWN))
    {
        XMMATRIX rotation = XMMatrixRotationAxis(cameraRight, CAMERA_ROTATION_SPEED * _elapsed_time);
        cameraFront = XMVector3TransformNormal(cameraFront, rotation);
        cameraFront = XMVector3Normalize(cameraFront);
        cameraUp = XMVector3Cross(cameraFront, cameraRight);
    }
    if (KeyLogger_IsPressed(KK_UP))
    {
        XMMATRIX rotation = XMMatrixRotationAxis(cameraRight, -CAMERA_ROTATION_SPEED * _elapsed_time);
        cameraFront = XMVector3TransformNormal(cameraFront, rotation);
        cameraFront = XMVector3Normalize(cameraFront);
        cameraUp = XMVector3Cross(cameraFront, cameraRight);
    }
    if (KeyLogger_IsPressed(KK_LEFT))
    {
        XMMATRIX rotation = XMMatrixRotationAxis(cameraUp, -CAMERA_ROTATION_SPEED * _elapsed_time);
        cameraFront = XMVector3TransformNormal(cameraFront, rotation);
        cameraFront = XMVector3Normalize(cameraFront);
        cameraRight = XMVector3Cross(cameraUp, cameraFront);
    }
    if (KeyLogger_IsPressed(KK_RIGHT))
    {
        XMMATRIX rotation = XMMatrixRotationAxis(cameraUp, CAMERA_ROTATION_SPEED * _elapsed_time);
        cameraFront = XMVector3TransformNormal(cameraFront, rotation);
        cameraFront = XMVector3Normalize(cameraFront);
        cameraRight = XMVector3Cross(cameraUp, cameraFront);
    }

    if (KeyLogger_IsPressed(KK_Q))
    {
        XMMATRIX rotation = XMMatrixRotationAxis(cameraFront, CAMERA_ROTATION_SPEED * _elapsed_time);
        cameraRight = XMVector3TransformNormal(cameraRight, rotation);
        cameraRight = XMVector3Normalize(cameraRight);
        cameraUp = XMVector3Cross(cameraFront, cameraRight);
    }
    if (KeyLogger_IsPressed(KK_E))
    {
        XMMATRIX rotation = XMMatrixRotationAxis(cameraFront, -CAMERA_ROTATION_SPEED * _elapsed_time);
        cameraRight = XMVector3TransformNormal(cameraRight, rotation);
        cameraRight = XMVector3Normalize(cameraRight);
        cameraUp = XMVector3Cross(cameraFront, cameraRight);
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

    XMStoreFloat4x4(&g_CameraMatrix, mtxView);
    Shader3D_SetViewMatrix(mtxView); // TODO: deprecate

    // パースペクティブ行列
    float fovAngleY = XMConvertToRadians(45.0f);
    float aspectRatio = static_cast<float>(Direct3D_GetBackBufferWidth()) / static_cast<float>(Direct3D_GetBackBufferHeight());
    float nearZ = 0.01f;
    float farZ = 10000.0f;

    XMMATRIX mtxPerspective = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);

    XMStoreFloat4x4(&g_PerspectiveMatrix, mtxPerspective);
    Shader3D_SetProjectionMatrix(mtxPerspective); // TODO: deprecate
}

const XMFLOAT4X4& Camera_GetMatrix()
{
    return g_CameraMatrix;
}

const XMFLOAT4X4& Camera_GetPerspectiveMatrix()
{
    return g_PerspectiveMatrix;
}

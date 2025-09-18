/**
 * @file camera.cpp
 * @brief カメラ制御
 * 
 * @author KOOLER FAN
 * @date 2025/09/11
 */


#include "camera.h"

#include <memory>
#include <sstream>
#include <DirectXMath.h>
#include <iomanip>

#include "color.h"
#include "debug_imgui_camera.h"
#include "direct3d.h"
#include "shader3d.h"
#include "key_logger.h"
#include "debug_text.h"

using namespace DirectX;

static XMFLOAT3 g_CameraPosition = { 0.0f, 0.0f, 0.0f };
static XMFLOAT3 g_CameraFront = { 0.0f, 0.0f, 1.0f };
static XMFLOAT3 g_CameraUp = { 0.0f, 1.0f, 0.0f };
static XMFLOAT3 g_CameraRight = { 1.0f, 0.0f, 0.0f };

constexpr float CAMERA_MOVE_SPEED = 7.5f; // per seconds
constexpr float CAMERA_ROTATION_SPEED = XMConvertToRadians(60.0f); // per seconds

static XMFLOAT4X4 g_CameraMatrix;
static XMFLOAT4X4 g_PerspectiveMatrix;

static std::unique_ptr<hal::DebugText> g_pDebugText = nullptr;

void Camera_Initialize(
    const DirectX::XMFLOAT3& position,
    const DirectX::XMFLOAT3& front,
    const DirectX::XMFLOAT3& up
    )
{
    Camera_Initialize();

    XMVECTOR _front = XMLoadFloat3(&front);
    XMVECTOR _up = XMLoadFloat3(&up);

    _front = XMVector3Normalize(_front);
    _up = XMVector3Normalize(_up);

    XMVECTOR _right = XMVector3Cross(_up, _front);
    _right = XMVector3Normalize(_right);

    XMStoreFloat3(&g_CameraFront, _front);
    XMStoreFloat3(&g_CameraUp, _up);
    XMStoreFloat3(&g_CameraRight, _right);

    g_CameraPosition = position;
}

void Camera_Initialize()
{
    g_CameraPosition = { 0.0f, 1.0f, -10.0f };
    g_CameraFront = { 0.0f, 0.0f, 1.0f };
    g_CameraUp = { 0.0f, 1.0f, 0.0f };
    g_CameraRight = { 1.0f, 0.0f, 0.0f };

    XMStoreFloat4x4(&g_CameraMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&g_PerspectiveMatrix, XMMatrixIdentity());

    g_pDebugText = std::make_unique<hal::DebugText>(Direct3D_GetDevice(), Direct3D_GetContext(),
                                                    L"assets/consolab_ascii_512.png",
                                                    Direct3D_GetBackBufferWidth(), Direct3D_GetBackBufferHeight(),
                                                    0.0f, 32.0f, 0, 0, 0.0f, 16.0f);

    DebugImGui_SetOnCameraPositionChanged(Camera_SetPosition);
    DebugImGui_SetOnCameraFrontChanged(Camera_SetFrontVec);
    DebugImGui_SetOnCameraUpChanged(Camera_SetUpVec);
    DebugImGui_SetOnCameraRightChanged(Camera_SetRightVec);
}

void Camera_Finalize()
{
    g_pDebugText.reset();
}

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
        // cameraPosition += cameraFront * CAMERA_MOVE_SPEED * _elapsed_time;
        XMVECTOR front = XMVector3Normalize(cameraFront * XMVECTOR{ 1.0f, 0.0f, 1.0f });
        cameraPosition += front * CAMERA_MOVE_SPEED * _elapsed_time;
    }
    if (KeyLogger_IsPressed(KK_A))
    {
        cameraPosition += -cameraRight * CAMERA_MOVE_SPEED * _elapsed_time;
    }
    if (KeyLogger_IsPressed(KK_S))
    {
        // cameraPosition += -cameraFront * CAMERA_MOVE_SPEED * _elapsed_time;
        XMVECTOR front = XMVector3Normalize(cameraFront * XMVECTOR{ 1.0f, 0.0f, 1.0f });
        cameraPosition += -front * CAMERA_MOVE_SPEED * _elapsed_time;
    }
    if (KeyLogger_IsPressed(KK_D))
    {
        cameraPosition += cameraRight * CAMERA_MOVE_SPEED * _elapsed_time;
    }

    if (KeyLogger_IsPressed(KK_E))
    {
        cameraPosition += cameraUp * CAMERA_MOVE_SPEED * _elapsed_time;
        cameraPosition += XMVECTOR{ 0.0f, 1.0f, 0.0f } * CAMERA_MOVE_SPEED * _elapsed_time;
    }

    if (KeyLogger_IsPressed(KK_Q))
    {
        cameraPosition += -cameraUp * CAMERA_MOVE_SPEED * _elapsed_time;
        cameraPosition += XMVECTOR{ 0.0f, -1.0f, 0.0f } * CAMERA_MOVE_SPEED * _elapsed_time;
    }

    //回転
    if (KeyLogger_IsPressed(KK_DOWN))
    {
        XMMATRIX rotation = XMMatrixRotationAxis(cameraRight, CAMERA_ROTATION_SPEED * _elapsed_time);
        cameraFront = XMVector3TransformNormal(cameraFront, rotation);
        cameraFront = XMVector3Normalize(cameraFront);
        cameraUp = XMVector3Cross(cameraFront, cameraRight);
        cameraUp = XMVector3Normalize(cameraUp);
    }
    if (KeyLogger_IsPressed(KK_UP))
    {
        XMMATRIX rotation = XMMatrixRotationAxis(cameraRight, -CAMERA_ROTATION_SPEED * _elapsed_time);
        cameraFront = XMVector3TransformNormal(cameraFront, rotation);
        cameraFront = XMVector3Normalize(cameraFront);
        cameraUp = XMVector3Cross(cameraFront, cameraRight);
        cameraUp = XMVector3Normalize(cameraUp);
    }
    if (KeyLogger_IsPressed(KK_LEFT))
    {
        // XMMATRIX rotation = XMMatrixRotationAxis(cameraUp, -CAMERA_ROTATION_SPEED * _elapsed_time);
        XMMATRIX rotation = XMMatrixRotationY(-CAMERA_ROTATION_SPEED * _elapsed_time);
        cameraUp = XMVector3TransformNormal(cameraUp, rotation);
        cameraUp = XMVector3Normalize(cameraUp);
        cameraFront = XMVector3TransformNormal(cameraFront, rotation);
        cameraFront = XMVector3Normalize(cameraFront);
        cameraRight = XMVector3Cross(cameraUp, cameraFront);
        cameraRight = XMVector3Normalize(cameraRight);
    }
    if (KeyLogger_IsPressed(KK_RIGHT))
    {
        // XMMATRIX rotation = XMMatrixRotationAxis(cameraUp, CAMERA_ROTATION_SPEED * _elapsed_time);
        XMMATRIX rotation = XMMatrixRotationY(CAMERA_ROTATION_SPEED * _elapsed_time);
        cameraUp = XMVector3TransformNormal(cameraUp, rotation);
        cameraUp = XMVector3Normalize(cameraUp);
        cameraFront = XMVector3TransformNormal(cameraFront, rotation);
        cameraFront = XMVector3Normalize(cameraFront);
        cameraRight = XMVector3Cross(cameraUp, cameraFront);
        cameraRight = XMVector3Normalize(cameraRight);
    }

    // if (KeyLogger_IsPressed(KK_Q))
    // {
    //     XMMATRIX rotation = XMMatrixRotationAxis(cameraFront, CAMERA_ROTATION_SPEED * _elapsed_time);
    //     cameraRight = XMVector3TransformNormal(cameraRight, rotation);
    //     cameraRight = XMVector3Normalize(cameraRight);
    //     cameraUp = XMVector3Cross(cameraFront, cameraRight);
    // }
    // if (KeyLogger_IsPressed(KK_E))
    // {
    //     XMMATRIX rotation = XMMatrixRotationAxis(cameraFront, -CAMERA_ROTATION_SPEED * _elapsed_time);
    //     cameraRight = XMVector3TransformNormal(cameraRight, rotation);
    //     cameraRight = XMVector3Normalize(cameraRight);
    //     cameraUp = XMVector3Cross(cameraFront, cameraRight);
    // }


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

const XMFLOAT3& Camera_GetFront()
{
    return g_CameraFront;
}

const XMFLOAT3& Camera_GetRight()
{
    return g_CameraRight;
}

const XMFLOAT3& Camera_GetUp()
{
    return g_CameraUp;
}

const XMFLOAT3& Camera_GetPosition()
{
    return g_CameraPosition;
}

void Camera_SetPosition(const XMFLOAT3& position)
{
    g_CameraPosition = position;
}

void Camera_SetFrontVec(const XMFLOAT3& front)
{
    XMVECTOR front_ = XMVector3Normalize(XMLoadFloat3(&front));
    XMVECTOR up_ = XMLoadFloat3(&g_CameraUp);
    XMVECTOR right_ = XMLoadFloat3(&g_CameraRight);

    right_ = XMVector3Normalize(XMVector3Cross(up_, front_));
    up_ = XMVector3Normalize(XMVector3Cross(front_, right_));

    XMStoreFloat3(&g_CameraFront, front_);
    XMStoreFloat3(&g_CameraUp, up_);
    XMStoreFloat3(&g_CameraRight, right_);
}

void Camera_SetUpVec(const XMFLOAT3& up)
{
    XMVECTOR front_ = XMLoadFloat3(&g_CameraFront);
    XMVECTOR up_ = XMVector3Normalize(XMLoadFloat3(&up));
    XMVECTOR right_ = XMLoadFloat3(&g_CameraRight);

    front_ = XMVector3Normalize(XMVector3Cross(right_, up_));
    right_ = XMVector3Normalize(XMVector3Cross(up_, front_));

    XMStoreFloat3(&g_CameraFront, front_);
    XMStoreFloat3(&g_CameraUp, up_);
    XMStoreFloat3(&g_CameraRight, right_);
}

void Camera_SetRightVec(const XMFLOAT3& right)
{
    XMVECTOR front_ = XMLoadFloat3(&g_CameraFront);
    XMVECTOR up_ = XMLoadFloat3(&g_CameraUp);
    XMVECTOR right_ = XMVector3Normalize(XMLoadFloat3(&right));

    up_ = XMVector3Normalize(XMVector3Cross(front_, right_));
    front_ = XMVector3Normalize(XMVector3Cross(right_, up_));

    XMStoreFloat3(&g_CameraFront, front_);
    XMStoreFloat3(&g_CameraUp, up_);
    XMStoreFloat3(&g_CameraRight, right_);
}

void Camera_DebugDraw()
{
    // std::stringstream ss;
    //
    // ss << std::showpos << std::fixed << std::setprecision(4);
    // ss << "Camera Front   : " << std::setw(8) << g_CameraFront.x << " " << std::setw(8) << g_CameraFront.y << " " << std::setw(8) << g_CameraFront.z << "\n";
    // ss << "Camera Right: " << std::setw(8) << g_CameraRight.x << " " << std::setw(8) << g_CameraRight.y << " " << std::setw(8) << g_CameraRight.z << "\n";
    // ss << "Camera Up: " << std::setw(8) << g_CameraUp.x << " " << std::setw(8) << g_CameraUp.y << " " << std::setw(8) << g_CameraUp.z << "\n";
    // ss << "Camera Position: " << std::setw(8) << g_CameraPosition.x << " " << std::setw(8) << g_CameraPosition.y << " " << std::setw(8) << g_CameraPosition.z << "\n";
    //
    // g_pDebugText->SetText(ss.str().c_str(), Color::YELLOW);
    // g_pDebugText->Draw();
    // g_pDebugText->Clear();

    DebugImGui_UpdateCameraData(g_CameraFront, g_CameraUp, g_CameraRight, g_CameraPosition);
}

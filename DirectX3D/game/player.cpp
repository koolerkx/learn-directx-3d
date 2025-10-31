/**
 * @file player.cpp
 * @brief プレイヤー制御
 * @author KOOLER FAN
 * @date 2025/10/31
 */

#include "player.h"

#include <memory>

#include <DirectXMath.h>

#include "camera.h"
#include "key_logger.h"
#include "light.h"
using namespace DirectX;

#include "model.h"

static XMFLOAT3 g_PlayerPosition = { 0.0f, 0.0f, 0.0f };
static XMFLOAT3 g_PlayerFront = { 0.0f, 0.0f, 1.0f };
static XMFLOAT3 g_PlayerVelocity = { 0.0f, 0.0f, 0.0f };

static std::unique_ptr<MODEL> g_PlayerModel = nullptr;
static const std::string MODEL_PATH = "assets/slime_re.fbx";

static bool g_IsJump = false;

static constexpr XMVECTOR jump_direction = { 0.0f, 1.0f, 0.0f };
static constexpr float jump_force = 5.0f;

static constexpr XMVECTOR gravity_direction = { 0.0f, -1.0f, 0.0f };
static constexpr float gravity_force = 9.8f;
static constexpr float gravity_scale = 1.0f;

void Player_Initialize(const XMFLOAT3& position, const XMFLOAT3& front)
{
    g_PlayerPosition = position;
    g_PlayerVelocity = { 0.0f, 0.0f, 0.0f };
    XMStoreFloat3(&g_PlayerFront, XMVector3Normalize(XMLoadFloat3(&front)));

    g_IsJump = false;

    g_PlayerModel.reset(ModelLoad(MODEL_PATH.c_str(), 0.25f));
}

void Player_Finalize()
{
    g_PlayerModel.reset();
}

void Player_Update(double elapsed_time)
{
    XMVECTOR position = XMLoadFloat3(&g_PlayerPosition);
    XMVECTOR velocity = XMLoadFloat3(&g_PlayerVelocity);

    if (KeyLogger_IsTrigger(KK_SPACE) && !g_IsJump)
    {
        velocity += jump_direction * jump_force;
        g_IsJump = true;
    }

    velocity += gravity_direction * gravity_force * gravity_scale * static_cast<float>(elapsed_time);
    position += velocity * static_cast<float>(elapsed_time);
    
    if (XMVectorGetY(position) < 0.0f)
    {
        position -= velocity * static_cast<float>(elapsed_time);
        velocity *= { 1.0f, 0.0f, 1.0f };
        g_IsJump = false;
    }

    XMStoreFloat3(&g_PlayerVelocity, velocity);
    XMStoreFloat3(&g_PlayerPosition, position);
}

void Player_Draw()
{
    Light_SetSpecular(Camera_GetPosition(), 32.0f, { 0.5f, 0.5f, 0.5f });

    XMMATRIX mtxWorld = XMMatrixIdentity();
    mtxWorld *= XMMatrixTranslation(g_PlayerPosition.x, g_PlayerPosition.y, g_PlayerPosition.z);
    ModelDraw(g_PlayerModel.get(), mtxWorld);
}

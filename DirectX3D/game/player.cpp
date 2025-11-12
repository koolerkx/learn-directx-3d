/**
 * @file player.cpp
 * @brief プレイヤー制御
 * @author KOOLER FAN
 * @date 2025/10/31
 */

#include "player.h"

#include <memory>

#include <DirectXMath.h>

#include "cube.h"
#include "key_logger.h"
#include "light.h"
#include "map.h"
#include "player_camera.h"
using namespace DirectX;

#include "model.h"

static XMFLOAT3 g_PlayerPosition = { 0.0f, 0.0f, 0.0f };
static XMFLOAT3 g_PlayerFront = { 0.0f, 0.0f, 1.0f };
static XMFLOAT3 g_PlayerVelocity = { 0.0f, 0.0f, 0.0f };

static std::unique_ptr<MODEL> g_PlayerModel = nullptr;
static const std::string MODEL_PATH = "assets/slime_re.fbx";

static bool g_IsJump = false;

static constexpr XMVECTOR JUMP_DIRECTION = { 0.0f, 1.0f, 0.0f };
static constexpr float JUMP_FORCE = 5.0f;

static constexpr XMVECTOR GRAVITY_DIRECTION = { 0.0f, -1.0f, 0.0f };
static constexpr float GRAVITY_FORCE = 9.8f;
static constexpr float GRAVITY_SCALE = 1.0f;
static const XMVECTOR GRAVITY_ACC =
    GRAVITY_DIRECTION * GRAVITY_FORCE * GRAVITY_SCALE;

static constexpr float speed = 0.5;

void Player_Initialize(const XMFLOAT3& position, const XMFLOAT3& front)
{
    g_PlayerPosition = position;
    g_PlayerVelocity = { 0.0f, 0.0f, 0.0f };
    XMStoreFloat3(&g_PlayerFront, XMVector3Normalize(XMLoadFloat3(&front)));

    g_IsJump = false;

    g_PlayerModel.reset(ModelLoad(MODEL_PATH.c_str(), 0.25f));
}

void Player_Finalize() { g_PlayerModel.reset(); }

void Player_Update(double elapsed_time)
{
    XMVECTOR position = XMLoadFloat3(&g_PlayerPosition);
    XMVECTOR velocity = XMLoadFloat3(&g_PlayerVelocity);

    if (KeyLogger_IsTrigger(KK_SPACE) && !g_IsJump)
    {
        velocity += JUMP_DIRECTION * JUMP_FORCE;
        g_IsJump = true;
    }

    velocity += GRAVITY_ACC * static_cast<float>(elapsed_time);

    position += velocity * static_cast<float>(elapsed_time);
    XMStoreFloat3(&g_PlayerPosition, position);

    for (int i = 0; i < Map_GetObjectsCount(); i++)
    {
        AABB player = Player_ConvertPositionToAABB(position);
        AABB object = Map_GetObject(i)->aabb;

        Hit hit = Collision_IsHitAABB(object, player);
        // object collision due to gravity
        if (hit.isHit)
        {
            if (hit.normal.y > 0.0f)
            {
                // position -= gravity_velocity;
                position = XMVectorSetY(position, object.max.y);

                velocity *= { 1.0f, 0.0f, 1.0f };
                g_IsJump = false;
            }
        }
    }

    XMVECTOR movement_direction = { 0.0f, 0.0f, 0.0f };
    XMVECTOR camera_front =
        XMVectorSetY(XMLoadFloat3(&Player_Camera_GetFront()), 0);
    XMVECTOR camera_right = XMVector3Cross(
        XMLoadFloat3(&Player_Camera_GetUp()),
        XMLoadFloat3(&Player_Camera_GetFront())
    );

    if (KeyLogger_IsPressed(KK_W))
    {
        movement_direction += camera_front;
    }
    if (KeyLogger_IsPressed(KK_S))
    {
        movement_direction -= camera_front;
    }
    if (KeyLogger_IsPressed(KK_A))
    {
        movement_direction -= camera_right;
    }
    if (KeyLogger_IsPressed(KK_D))
    {
        movement_direction += camera_right;
    }

    // Normalize movement to prevent faster diagonal movement
    if (XMVectorGetX(XMVector3LengthSq(movement_direction)) > 0.0f)
    {
        movement_direction = XMVector3Normalize(movement_direction);

        XMVECTOR dot =
            XMVector3Dot(XMLoadFloat3(&g_PlayerFront), movement_direction);
        float angle = acosf(XMVectorGetX(dot));

        const float rotation_speed = XMConvertToRadians(720.0f * elapsed_time);

        // XMStoreFloat3(&g_PlayerFront, movement_direction);
        XMVECTOR front;
        if (angle < rotation_speed)
        {
            front = movement_direction;
        }
        else
        {
            XMMATRIX r = XMMatrixIdentity();
            if (XMVectorGetY(XMVector3Cross(
                    movement_direction, XMLoadFloat3(&g_PlayerFront)
                )) < 0.0f)
            {
                r = XMMatrixRotationY(rotation_speed);
            }
            else
            {
                r = XMMatrixRotationY(-rotation_speed);
            }
            front = XMVector3TransformNormal(XMLoadFloat3(&g_PlayerFront), r);
        }

        XMStoreFloat3(&g_PlayerFront, front);

        velocity += front * static_cast<float>(800.0 / 50.0 * elapsed_time);
    }

    // Apply movement acceleration
    velocity += -velocity * static_cast<float>(4.0 * elapsed_time); // drag
    position += velocity * static_cast<float>(elapsed_time);

    XMStoreFloat3(&g_PlayerVelocity, velocity);
    XMStoreFloat3(&g_PlayerPosition, position);

    {
        AABB player = Player_GetAABB();
        for (int i = 0; i < Map_GetObjectsCount(); i++)
        {
            AABB object = Map_GetObject(i)->aabb;

            Hit hit = Collision_IsHitAABB(object, player);
            if (hit.isHit)
            {
                if (hit.normal.x > 0.0f)
                {
                    position = XMVectorSetX(position, object.max.x + 0.5f);
                    velocity *= { 0.0f, 1.0f, 1.0f };
                }
                else if (hit.normal.x < 0.0f)
                {
                    position = XMVectorSetX(position, object.min.x - 0.5f);
                    velocity *= { 0.0f, 1.0f, 1.0f };
                }
                else if (hit.normal.y < 0.0f)
                {
                    position = XMVectorSetY(position, object.min.y - 1.0f);
                    velocity *= { 1.0f, 0.0f, 1.0f };
                }
                else if (hit.normal.z > 0.0f)
                {
                    position = XMVectorSetZ(position, object.max.z + 0.5f);
                    velocity *= { 1.0f, 1.0f, 0.0f };
                }
                else if (hit.normal.z < 0.0f)
                {
                    position = XMVectorSetZ(position, object.min.z - 0.5f);
                    velocity *= { 1.0f, 1.0f, 0.0f };
                }
            }
        }
    }

    XMStoreFloat3(&g_PlayerPosition, position);
    XMStoreFloat3(&g_PlayerVelocity, velocity);
}

void Player_Draw()
{
    Light_SetSpecular(Player_Camera_GetPosition(), 10.0f, { 0.5f, 0.5f, 0.5f });

    XMMATRIX mtxWorld = XMMatrixIdentity();

    float angle =
        atan2f(g_PlayerFront.x, g_PlayerFront.z) + XMConvertToRadians(180.0f);

    mtxWorld *= XMMatrixRotationY(angle);
    mtxWorld *= XMMatrixTranslation(
        g_PlayerPosition.x, g_PlayerPosition.y, g_PlayerPosition.z
    );
    ModelDraw(g_PlayerModel.get(), mtxWorld);
}

const XMFLOAT3& Player_GetPosition() { return g_PlayerPosition; }

const XMFLOAT3& Player_GetFront() { return g_PlayerFront; }

AABB Player_GetAABB()
{
    return { { g_PlayerPosition.x - 0.5f, g_PlayerPosition.y,
               g_PlayerPosition.z - 0.5f },
             { g_PlayerPosition.x + 0.5f, g_PlayerPosition.y + 1.0f,
               g_PlayerPosition.z + 0.5f } };
}

AABB Player_ConvertPositionToAABB(const DirectX::XMVECTOR& position)
{
    constexpr float size = 1.0f;

    AABB aabb;
    XMStoreFloat3(
        &aabb.min, position - XMVECTOR{ size / 2.0f, 0.0f, size / 2.0f }
    );
    XMStoreFloat3(
        &aabb.max, position + XMVECTOR{ size / 2.0f, size, size / 2.0f }
    );
    return aabb;
}

#include "game.h"

#include "cube.h"
#include "grid.h"
// #include "camera.h"
#include "key_logger.h"

#include <DirectXMath.h>

#include "bullet.h"
#include "color.h"
#include "debug_frame.h"
#include "light.h"
#include "map.h"
#include "mesh_field.h"
#include "model.h"
#include "player.h"
#include "player_camera.h"
#include "sampler.h"

using namespace DirectX;

static double acc_time = 0;
static float g_angle = 0;

void Game_Initialize()
{
    Map_Initialize();
    Bullet_Initialize();
    // constexpr XMFLOAT3 DEFAULT_CAMERA_POSITION = { -10.0f, 10.0f, -10.0f };
    // constexpr XMFLOAT3 DEFAULT_CAMERA_FRONT = { 0.5f, -0.5f, 0.5f };
    // constexpr XMFLOAT3 DEFAULT_CAMERA_UP = { 0.5, 0.5, 0.5 };

    // Camera_Initialize(
    //     DEFAULT_CAMERA_POSITION,
    //     DEFAULT_CAMERA_FRONT,
    //     DEFAULT_CAMERA_UP
    //     );
    Player_Camera_Initialize();

    Player_Initialize({ 0.0f, 5.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
}

void Game_Finalize()
{
    // Camera_Finalize();
    Player_Camera_Finalize();

    Player_Finalize();
    Bullet_Finalize();
    Map_Finalize();
}

void Game_Update(double elapsed_time)
{
    // Cube_Update(elapsed_time);
    // Camera_Update(elapsed_time);
    Player_Camera_Update(elapsed_time);

    MeshField_Update(elapsed_time);
    Player_Update(elapsed_time);
    Bullet_Update(elapsed_time);

    // if (KeyLogger_IsTrigger(KK_Z))
    // {
    //     g_CubePosition = Camera_GetPosition();
    //     XMStoreFloat3(&g_CubeVelocity, XMLoadFloat3(&Camera_GetFront())
    //     * 10.0f);
    // }
    // XMVECTOR cube_position = XMLoadFloat3(&g_CubePosition);
    // cube_position += XMLoadFloat3(&g_CubeVelocity) *
    // static_cast<float>(elapsed_time); XMStoreFloat3(&g_CubePosition,
    // cube_position);
    acc_time += elapsed_time;

    for (int j = 0; j < Map_GetObjectsCount(); j++)
    {
        for (int i = 0; i < Bullet_GetObjectsCount(); i++)
        {
            AABB bullet = Bullet_GetAABB(i);
            AABB object = Map_GetObject(j)->aabb;
            if (Collision_IsOverlapAABB(bullet, object))
            {
                Bullet_Destroy(i);
            }
        }
    }

    g_angle += static_cast<float>(elapsed_time) * 120.0f;
}

void Game_Draw()
{
    Grid_Draw();

    Light_SetAmbient({ 0.3f, 0.3f, 0.3f });
    Light_SetDirectional({ 1.0f, 0.0f, 0.0f, 0.0f }, { 0.3f, 0.3f, 0.3f });
    Light_SetPointCount(3);

    XMFLOAT3 pp0, pp1, pp2;
    XMStoreFloat3(
        &pp0,
        XMVector3Transform(
            { 10.0f, 5.0f, 0.0f },
            XMMatrixRotationY(XMConvertToRadians(g_angle))
        )
    );
    XMStoreFloat3(
        &pp1,
        XMVector3Transform(
            { 10.0f, 5.0f, 0.0f },
            XMMatrixRotationY(XMConvertToRadians(g_angle + 120))
        )
    );

    XMStoreFloat3(
        &pp2,
        XMVector3Transform(
            { 10.0f, 5.0f, 0.0f },
            XMMatrixRotationY(XMConvertToRadians(g_angle + 240))
        )
    );

    Light_SetPointLight(0, pp0, 30, { 1.0f, 0.0f, 0.0f });
    Light_SetPointLight(1, pp1, 30, { 0.0f, 1.0f, 0.0f });
    Light_SetPointLight(2, pp2, 30, { 0.0f, 0.0f, 1.0f });

    Sampler_SetFilter(FILTER::ANISOTROPIC);

    // Light_SetSpecular(Camera_GetPosition(), 16.0f, { 0.7f, 0.7f, 0.7f });
    // Light_SetSpecular(Player_Camera_GetPosition(), 16.0f, { 0.7f, 0.7f, 0.7f
    // }); MeshField_Draw();

    // XMMATRIX mtxWorld = XMMatrixIdentity();
    // mtxWorld *= XMMatrixRotationY(static_cast<float>(acc_time));
    // mtxWorld *= XMMatrixTranslation(0.0f, 2.0f, 0.0f);

    Map_Draw();
    Bullet_Draw();

    Player_Draw();
    // XMMATRIX cube_pos = XMMatrixTranslation(3.0f, 0.5f, 2.0f);

    // Cube_Draw(cube_pos);
    //
    // DebugFrame_AABB_Draw(Cube_GetAABB({ 3.0f, 0.5f, 2.0f }));
    // DebugFrame_AABB_Draw(Player_GetAABB());
    // Camera_DebugDraw();
}

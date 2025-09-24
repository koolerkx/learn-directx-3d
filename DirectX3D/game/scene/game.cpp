#include "game.h"

#include "cube.h"
#include "grid.h"
#include "camera.h"
#include "key_logger.h"

#include <DirectXMath.h>

#include "mesh_field.h"

using namespace DirectX;

static XMFLOAT3 g_CubePosition{};
static XMFLOAT3 g_CubeVelocity{};

void Game_Initialize()
{
    constexpr XMFLOAT3 DEFAULT_CAMERA_POSITION = { -10.0f, 10.0f, -10.0f };
    constexpr XMFLOAT3 DEFAULT_CAMERA_FRONT = { 0.5f, -0.5f, 0.5f };
    constexpr XMFLOAT3 DEFAULT_CAMERA_UP = { 0.5, 0.5, 0.5 };

    Camera_Initialize(
        DEFAULT_CAMERA_POSITION,
        DEFAULT_CAMERA_FRONT,
        DEFAULT_CAMERA_UP
        );
}

void Game_Finalize()
{
    Camera_Finalize();
}

void Game_Update(double elapsed_time)
{
    // Cube_Update(elapsed_time);
    Camera_Update(elapsed_time);
    MeshField_Update(elapsed_time);

    // if (KeyLogger_IsTrigger(KK_Z))
    // {
    //     g_CubePosition = Camera_GetPosition();
    //     XMStoreFloat3(&g_CubeVelocity, XMLoadFloat3(&Camera_GetFront()) * 10.0f);
    // }
    // XMVECTOR cube_position = XMLoadFloat3(&g_CubePosition);
    // cube_position += XMLoadFloat3(&g_CubeVelocity) * static_cast<float>(elapsed_time);
    // XMStoreFloat3(&g_CubePosition, cube_position);
}

void Game_Draw()
{
    Grid_Draw();

    // XMMATRIX mtxWorld = XMMatrixIdentity();
    // mtxWorld *= XMMatrixTranslationFromVector(XMLoadFloat3(&g_CubePosition));

    Cube_Draw();
    MeshField_Draw();

    Camera_DebugDraw();
}

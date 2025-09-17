#include "game.h"

#include "cube.h"
#include "grid.h"
#include "camera.h"
#include "key_logger.h"

#include <DirectXMath.h>
using namespace DirectX;

static XMFLOAT3 g_CubePosition{};
static XMFLOAT3 g_CubeVelocity{};

void Game_Initialize()
{
    Camera_Initialize();
}

void Game_Finalize()
{
    Camera_Finalize();
}

void Game_Update(double elapsed_time)
{
    Cube_Update(elapsed_time);
    Camera_Update(elapsed_time);

    if (KeyLogger_IsTrigger(KK_Z))
    {
        g_CubePosition = Camera_GetPosition();
        XMStoreFloat3(&g_CubeVelocity, XMLoadFloat3(&Camera_GetFront()) * 10.0f);
    }
    XMVECTOR cube_position = XMLoadFloat3(&g_CubePosition);
    cube_position += XMLoadFloat3(&g_CubeVelocity) * elapsed_time;
    XMStoreFloat3(&g_CubePosition, cube_position);
}

void Game_Draw()
{
    Grid_Draw();

    XMMATRIX mtxWorld = XMMatrixIdentity();
    mtxWorld *= XMMatrixTranslationFromVector(XMLoadFloat3(&g_CubePosition));
    Cube_Draw(mtxWorld);
}

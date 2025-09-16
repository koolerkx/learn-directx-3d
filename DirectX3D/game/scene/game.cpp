#include "game.h"

#include "cube.h"
#include "grid.h"
#include "camera.h"
#include <DirectXMath.h>

static float g_x = 4.5f;
static float g_z = 4.5f;
static float g_angle = 0.0f;
static float g_scale = 5.0f;
static double g_AccumulatedTime = 0.0f;

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

    g_AccumulatedTime += elapsed_time;

    g_x = static_cast<float>(sin(g_AccumulatedTime * 10)) * 3.0f;
    g_z = static_cast<float>(cos(g_AccumulatedTime * 10)) * 3.0f;

    g_angle = DirectX::XMConvertToRadians(static_cast<float>(g_AccumulatedTime) * 2048.0f);
    // g_angle = XMConvertToRadians(g_AccumulatedTime * 720.0f);
    g_scale = static_cast<float>(sin(g_AccumulatedTime * 5.0f) + 1.0f) * 0.5f * 2.0f;
    // g_scale = 0.5f;

    Camera_Update(elapsed_time);
    Cube_Update(elapsed_time);
}

void Game_Draw()
{
    Grid_Draw();

    DirectX::XMMATRIX mtxWorld = DirectX::XMMatrixIdentity();
    Cube_Draw(mtxWorld);
}

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

    Cube_Update(elapsed_time);
}

void Game_Draw()
{
    Camera_Update();
    Grid_Draw();
    
    float count = 7;

    for (int y = 0; y < count / 2; y++)
    {
        for (int x = 0; x < count - y * 2; x++)
        {
            for (int z = 0; z < count - y * 2; z++)
            {
                using namespace DirectX;
                XMMATRIX mtxWorld = XMMatrixIdentity();
                mtxWorld *= XMMatrixTranslation(
                    x - (count / 2 - 0.5f) + y,
                    0.5f + y,
                    z - (count / 2 - 0.5f) + y
                    );
                mtxWorld *= XMMatrixRotationY(XMConvertToRadians(g_angle * 10));
                mtxWorld *= XMMatrixTranslation(-5, 0, -5);
                mtxWorld *= XMMatrixRotationY(XMConvertToRadians(g_angle));
                mtxWorld *= XMMatrixScaling(g_scale, g_scale, g_scale);

                Cube_Draw(mtxWorld);
            }
        }
    }
}

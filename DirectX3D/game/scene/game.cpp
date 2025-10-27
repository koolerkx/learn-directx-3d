#include "game.h"

#include "cube.h"
#include "grid.h"
#include "camera.h"
#include "key_logger.h"

#include <DirectXMath.h>

#include "color.h"
#include "light.h"
#include "mesh_field.h"
#include "model.h"

using namespace DirectX;

static double acc_time = 0;

MODEL* pModel_slime;
MODEL* pModel_shinamon;
MODEL* pModel_densha;
MODEL* pModel_robot;
MODEL* pModel_kriby;

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

    pModel_slime = ModelLoad("assets/slime_re.fbx", 0.25f);
    pModel_densha = ModelLoad("assets/densha.fbx", 2.0f, true);
    pModel_shinamon = ModelLoad("assets/shinamon.fbx", 0.25f);
    pModel_robot = ModelLoad("assets/robot.fbx", 0.25f);
    pModel_kriby = ModelLoad("assets/test.fbx", 0.1f);
}

void Game_Finalize()
{
    Camera_Finalize();
    ModelRelease(pModel_slime);
    ModelRelease(pModel_shinamon);
    ModelRelease(pModel_densha);
    ModelRelease(pModel_robot);
    ModelRelease(pModel_kriby);
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
    
    acc_time += elapsed_time;
}

void Game_Draw()
{
    Grid_Draw();
    
    Light_SetAmbient({ 0.5f, 0.5f, 0.5f });
    Light_SetDirectional({ 1.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f});
    Light_SetSpecular(Camera_GetPosition(), 64.0f, {0.3f, 0.3f, 1.0f});
    
    XMMATRIX mtxWorld = XMMatrixIdentity();
    mtxWorld *= XMMatrixRotationY(static_cast<float>(acc_time));
    mtxWorld *= XMMatrixTranslation(0.0f, 2.0f, 0.0f);
    // Cube_Draw(mtxWorld);
    ModelDraw(pModel_shinamon, mtxWorld);
    
    mtxWorld *= XMMatrixTranslation(2.0f, 0.0f, 0.0f);
    ModelDraw(pModel_slime, mtxWorld);
    mtxWorld *= XMMatrixTranslation(4.0f, 0.0f, 0.0f);
    ModelDraw(pModel_densha, mtxWorld);

    mtxWorld *= XMMatrixTranslation(-8.0f, 0.0f, 0.0f);
    ModelDraw(pModel_robot, mtxWorld);

    Light_SetSpecular(Camera_GetPosition(), 64.0f, {0.1f, 0.1f, 0.1f});
    mtxWorld *= XMMatrixTranslation(-2.0f, 0.0f, 0.0f);

    Light_SetSpecular(Camera_GetPosition(), 1.0f, {0.1f, 0.1f, 0.1f});
    ModelDraw(pModel_kriby, mtxWorld);

    MeshField_Draw();

    Camera_DebugDraw();
}

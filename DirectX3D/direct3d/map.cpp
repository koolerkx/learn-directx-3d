/**
 * @file map.cpp
 * @brief マップの管理
 * @author KOOLER FAN
 * @date 2025/11/10
 */

#include "map.h"

#include <DirectXMath.h>

#include "cube.h"
#include "debug_frame.h"
#include "light.h"
#include "mesh_field.h"
#include "model.h"
#include "player_camera.h"

static MapObject g_MapObjects[]{
    { ObjectKind::FIELD,
      { 0.0f, 0.0f, 0.0f },
      { { -5.0f, -1.0f, -5.0f }, { 5.0f, 0.0f, 5.0f } } },
    { ObjectKind::BLOCK, { -1.0f, 0.5f, 2.0f } },
    { ObjectKind::BLOCK, { 0.0f, 0.5f, 2.0f } },
    { ObjectKind::BLOCK, { 1.0f, 0.5f, 2.0f } },
    { ObjectKind::BLOCK, { 2.0f, 0.5f, 2.0f } },
    { ObjectKind::BLOCK, { -1.0f, 0.5f, 3.0f } },
    { ObjectKind::BLOCK, { 0.0f, 0.5f, 3.0f } },
    { ObjectKind::BLOCK, { 1.0f, 0.5f, 3.0f } },
    { ObjectKind::BLOCK, { 2.0f, 0.5f, 3.0f } },
    { ObjectKind::BLOCK, { -1.0f, 0.5f, 4.0f } },
    { ObjectKind::BLOCK, { 0.0f, 0.5f, 4.0f } },
    { ObjectKind::BLOCK, { 1.0f, 0.5f, 4.0f } },
    { ObjectKind::BLOCK, { 2.0f, 0.5f, 4.0f } },
    { ObjectKind::BLOCK, { 0.0f, 1.5f, 3.0f } },
    { ObjectKind::BLOCK, { 1.0f, 1.5f, 3.0f } },
    { ObjectKind::ROCK, { -3.0f, 0.0f, -3.0f } },
};

static MODEL* g_Rock01{};

void Map_Initialize()
{
    g_Rock01 = ModelLoad("assets/slime_re.fbx", 0.25f);

    for (MapObject& o : g_MapObjects)
    {
        switch (o.kind_id)
        {
        case ObjectKind::BLOCK:
            o.aabb = Cube_GetAABB(o.position);
            break;
        case ObjectKind::ROCK:
            o.aabb = Model_GetAABB(g_Rock01, o.position);
            break;
        case ObjectKind::FIELD:
        default:
            break;
        }

        if (o.kind_id == ObjectKind::FIELD)
        {
            continue;
        }
    }
}

void Map_Finalize() { ModelRelease(g_Rock01); }

void Map_Draw()
{

    for (const MapObject& o : g_MapObjects)
    {
        DirectX::XMMATRIX mtxWorld = DirectX::XMMatrixIdentity();
        mtxWorld *= DirectX::XMMatrixTranslation(
            o.position.x, o.position.y, o.position.z
        );

        switch (o.kind_id)
        {
        case ObjectKind::FIELD:
            Light_SetSpecular(
                Player_Camera_GetPosition(), 50.0f, { 0.5f, 0.5f, 0.5f }
            );
            MeshField_Draw();
            DebugFrame_AABB_Draw(o.aabb);
            break;
        case ObjectKind::BLOCK:
            Cube_Draw(mtxWorld);
            DebugFrame_AABB_Draw(o.aabb);
            break;
        case ObjectKind::ROCK:
            ModelDraw(g_Rock01, mtxWorld);
            DebugFrame_AABB_Draw(o.aabb);
        default:
            break;
        }
    }
}
int Map_GetObjectsCount()
{
    return sizeof(g_MapObjects) / sizeof(g_MapObjects[0]);
}

const MapObject* Map_GetObject(int index) { return &g_MapObjects[index]; }

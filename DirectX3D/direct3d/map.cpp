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
#include "player_camera.h"

static MapObject g_MapObjects[]{
    { 0,
      { 0.0f, 0.0f, 0.0f },
      { { -5.0f, -1.0f, -5.0f }, { 5.0f, 0.0f, 5.0f } } },
    { 1, { -1.0f, 0.5f, 2.0f } },
    { 1, { 0.0f, 0.5f, 2.0f } },
    { 1, { 1.0f, 0.5f, 2.0f } },
    { 1, { 2.0f, 0.5f, 2.0f } },
    { 1, { -1.0f, 0.5f, 3.0f } },
    { 1, { 0.0f, 0.5f, 3.0f } },
    { 1, { 1.0f, 0.5f, 3.0f } },
    { 1, { 2.0f, 0.5f, 3.0f } },
    { 1, { -1.0f, 0.5f, 4.0f } },
    { 1, { 0.0f, 0.5f, 4.0f } },
    { 1, { 1.0f, 0.5f, 4.0f } },
    { 1, { 2.0f, 0.5f, 4.0f } },

    { 1, { 0.0f, 1.5f, 3.0f } },
    { 1, { 1.0f, 1.5f, 3.0f } },
};

void Map_Initialize()
{
    for (MapObject& o : g_MapObjects)
    {
        if (o.kind_id == 0)
        {
            continue;
        }
        o.aabb = Cube_GetAABB(o.position);
    }
}

void Map_Finalize() {}

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
        case 0:
            Light_SetSpecular(
                Player_Camera_GetPosition(), 50.0f, { 0.5f, 0.5f, 0.5f }
            );
            MeshField_Draw();
            break;
        case 1:
            Cube_Draw(mtxWorld);
            DebugFrame_AABB_Draw(Cube_GetAABB(o.position));
            break;
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

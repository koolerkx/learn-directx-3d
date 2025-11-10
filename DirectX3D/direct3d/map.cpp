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

static MapObject g_MapObjects[]{
    { 1, { -1.0f, 0.5f, 2.0f } }, { 1, { 0.0f, 0.5f, 2.0f } },
    { 1, { 1.0f, 0.5f, 2.0f } },  { 1, { 2.0f, 0.5f, 2.0f } },
    { 1, { -1.0f, 0.5f, 3.0f } }, { 1, { 0.0f, 0.5f, 3.0f } },
    { 1, { 1.0f, 0.5f, 3.0f } },  { 1, { 2.0f, 0.5f, 3.0f } },
    { 1, { -1.0f, 0.5f, 4.0f } }, { 1, { 0.0f, 0.5f, 4.0f } },
    { 1, { 1.0f, 0.5f, 4.0f } },  { 1, { 2.0f, 0.5f, 4.0f } },

    { 1, { 0.0f, 1.5f, 3.0f } },  { 1, { 1.0f, 1.5f, 3.0f } },
};

void Map_Initialize() {}

void Map_Finalize() {}

void Map_Draw()
{

    for (const MapObject& o : g_MapObjects)
    {
        DirectX::XMMATRIX mtxWorld = DirectX::XMMatrixIdentity();
        mtxWorld *= DirectX::XMMatrixTranslation(
            o.Position.x, o.Position.y, o.Position.z
        );

        switch (o.KindId)
        {
        case 1:
            Cube_Draw(mtxWorld);
            DebugFrame_AABB_Draw(Cube_GetAABB(o.Position));
            break;
        }
    }
}
int Map_GetObjectsCount()
{
    return sizeof(g_MapObjects) / sizeof(g_MapObjects[0]);
}

const MapObject* Map_GetObject(int index) { return &g_MapObjects[index]; }

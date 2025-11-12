/**
 * @file map.h
 * @brief マップの管理
 * @author KOOLER FAN
 * @date 2025/11/10
 */

#ifndef MAP_H
#define MAP_H
#include <DirectXMath.h>

#include "collision.h"

void Map_Initialize();
void Map_Finalize();

void Map_Draw();

int Map_GetObjectsCount();

struct MapObject
{
    int kind_id;
    DirectX::XMFLOAT3 position;
    AABB aabb;
};

const MapObject* Map_GetObject(int index);

#endif // MAP_H

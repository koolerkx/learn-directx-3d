/**
 * @file collision.h
 * @brief コリジョン判定
 * @author KOOLER FAN
 * @date 2025/07/03
 */

#ifndef COLLISION_H
#define COLLISION_H

#include <DirectXMath.h>

struct Circle
{
    DirectX::XMFLOAT2 center;
    float radius;
};

struct Box
{
    DirectX::XMFLOAT2 center;
    float half_width;
    float half_height;
};

struct AABB
{
    DirectX::XMFLOAT3 min;
    DirectX::XMFLOAT3 max;

    DirectX::XMFLOAT3 GetCenter() const { return DirectX::XMFLOAT3((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f); }
};

struct Hit
{
    bool isHit{ false };
    DirectX::XMFLOAT3 normal{};
};

bool Collision_IsOverlapCircle(const Circle& a, const Circle& b);
bool Collision_IsOverlapBox(const Box& a, const Box& b);

bool Collision_IsOverlapAABB(const AABB& a, const AABB& b);
Hit Collision_IsHitAABB(const AABB& a, const AABB& b);

#endif

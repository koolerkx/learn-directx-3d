/**
 * @file collision.cpp
 * @brief コリジョン判定
 * @author KOOLER FAN
 * @date 2025/07/03
 */

#include "collision.h"

#include <algorithm>

using namespace DirectX;

bool Collision_IsOverlapCircle(const Circle& a, const Circle& b)
{
    float x1 = b.center.x - a.center.x;
    float y1 = b.center.y - a.center.y;

    return (a.radius + b.radius) * (a.radius + b.radius) > (x1 * x1 + y1 * y1);

    // XMVECTOR ac = XMLoadFloat2(&a.center);
    // XMVECTOR bc = XMLoadFloat2(&b.center);
    // XMVECTOR lsq = XMVector2LengthSq(bc - ac);
    //
    // return (a.radius + b.radius) * (a.radius + b.radius) > XMVectorGetX(lsq);
}

bool Collision_IsOverlapBox(const Box& a, const Box& b)
{
    float at = a.center.y - a.half_height;
    float ab = a.center.y + a.half_height;
    float al = a.center.x - a.half_width;
    float ar = a.center.x + a.half_width;

    float bt = b.center.y - b.half_height;
    float bb = b.center.y + b.half_height;
    float bl = b.center.x - b.half_width;
    float br = b.center.x + b.half_width;

    return at < bb && ab > bt && al < br && ar > bl;
}

bool Collision_IsOverlapAABB(const AABB& a, const AABB& b) { return a.min.x < b.max.x && a.max.x > b.min.x && a.min.y < b.max.y && a.max.y > b.min.y && a.min.z < b.max.z && a.max.z > b.min.z; }

Hit Collision_IsHitAABB(const AABB& a, const AABB& b)
{
    Hit result{};
    result.isHit = Collision_IsOverlapAABB(a, b);
    if (!result.isHit)
        return result;

    float xdepth = std::min(a.max.x, b.max.x) - std::max(a.min.x, b.min.x);
    float ydepth = std::min(a.max.y, b.max.y) - std::max(a.min.y, b.min.y);
    float zdepth = std::min(a.max.z, b.max.z) - std::max(a.min.z, b.min.z);

    enum Axis
    {
        AXIS_X,
        AXIS_Y,
        AXIS_Z
    };
    Axis axis = AXIS_X;
    float mindepth = xdepth;

    if (ydepth < mindepth)
    {
        mindepth = ydepth;
        axis = AXIS_Y;
    }
    if (zdepth < mindepth)
    {
        mindepth = zdepth;
        axis = AXIS_Z;
    }

    XMFLOAT3 a_center = a.GetCenter();
    XMFLOAT3 b_center = b.GetCenter();
    XMVECTOR dir = XMLoadFloat3(&b_center) - XMLoadFloat3(&a_center); // a -> b

    XMVECTOR normal = XMVectorZero();
    switch (axis)
    {
    case AXIS_X:
        normal = XMVectorSet((XMVectorGetX(dir) >= 0.0f) ? 1.0f : -1.0f, 0.0f, 0.0f, 0.0f);
        break;
    case AXIS_Y:
        normal = XMVectorSet(0.0f, (XMVectorGetY(dir) >= 0.0f) ? 1.0f : -1.0f, 0.0f, 0.0f);
        break;
    case AXIS_Z:
        normal = XMVectorSet(0.0f, 0.0f, (XMVectorGetZ(dir) >= 0.0f) ? 1.0f : -1.0f, 0.0f);
        break;
    }

    XMStoreFloat3(&result.normal, normal);

    return result;
}

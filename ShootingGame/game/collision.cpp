/**
 * @file collision.cpp
 * @brief コリジョン判定
 * @author KOOLER FAN
 * @date 2025/07/03
 */

#include "collision.h"

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

    return false;
}

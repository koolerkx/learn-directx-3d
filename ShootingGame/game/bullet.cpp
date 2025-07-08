/**
* @file bullet.h
 * @brief 弾の処理
 * @author KOOLER FAN
 * @date 2025/07/01
 */

#include "bullet.h"

#include <DirectXMath.h>

#include "collision.h"
#include "color.h"
#include "sprite.h"
#include "texture.h"
using namespace DirectX;

#include "direct3d.h"

constexpr XMFLOAT2 BULLET_SIZE = {64.0f, 64.0f};
constexpr XMFLOAT2 BULLET_SPEED = {200.0f, 0.0f};
constexpr double BULLET_LIFETIME = 5.0;

struct Bullet
{
    XMFLOAT2 position;
    XMFLOAT2 velocity;
    double lifeTime;
    bool isEnable;
    Circle collision;
};

static Bullet g_Bullets[BULLET_MAX]{};
static int g_Bullet_TexId = -1;

void Bullet_Initialize()
{
    g_Bullet_TexId = Texture_Load(L"assets/star_tiny.png");

    for (Bullet& bullet : g_Bullets)
    {
        bullet.isEnable = false;
    }
}

void Bullet_Finalize()
{
}

void Bullet_Update(double elapsed_time)
{
    for (Bullet& bullet : g_Bullets)
    {
        if (!bullet.isEnable) continue;

        // 位置管理
        XMVECTOR position = XMLoadFloat2(&bullet.position);
        XMVECTOR velocity = XMLoadFloat2(&bullet.velocity);

        position += velocity * elapsed_time;

        XMStoreFloat2(&bullet.position, position);
        XMStoreFloat2(&bullet.velocity, velocity);

        // ライフサイクル管理
        bullet.lifeTime += elapsed_time;

        if (bullet.lifeTime >= BULLET_LIFETIME)
        {
            bullet.isEnable = false;
        }

        if (bullet.position.x > Direct3D_GetBackBufferWidth())
        {
            bullet.isEnable = false;
        }
    }
}

void Bullet_Draw()
{
    for (Bullet& bullet : g_Bullets)
    {
        if (!bullet.isEnable) continue;

        Sprite_Draw(g_Bullet_TexId,
                    bullet.position.x, bullet.position.y,
                    BULLET_SIZE.x, BULLET_SIZE.y, 0, Color::BLUE
        );
    }
}

void Bullet_Create(const XMFLOAT2& position)
{
    for (Bullet& bullet : g_Bullets)
    {
        if (bullet.isEnable) continue;

        // 空き領域発見
        bullet.isEnable = true;
        bullet.lifeTime = 0.0;
        bullet.position = position;
        bullet.velocity = BULLET_SPEED;
        bullet.collision = {{32.0f, 32.0f}, 16.0f};
        break;
    }
}

bool Bullet_IsEnable(int index)
{
    return g_Bullets[index].isEnable;
}

Circle Bullet_GetCollision(int index)
{
    float cx = g_Bullets[index].collision.center.x + g_Bullets[index].position.x;
    float cy = g_Bullets[index].collision.center.y + g_Bullets[index].position.y;

    return {{cx, cy}, g_Bullets[index].collision.radius};
}

void Bullet_Destroy(int index)
{
    g_Bullets[index].isEnable = false;
}

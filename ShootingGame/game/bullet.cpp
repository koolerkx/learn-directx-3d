/**
* @file bullet.h
 * @brief 弾の処理
 * @author KOOLER FAN
 * @date 2025/07/01
 */

#include "bullet.h"

#include <DirectXMath.h>

#include "color.h"
#include "sprite.h"
#include "texture.h"
using namespace DirectX;

#include "direct3d.h"

struct Bullet
{
    XMFLOAT2 position;
    XMFLOAT2 velocity;
    double lifeTime;
    bool isEnable;
};

static constexpr unsigned int BULLET_MAX = 1024;
static Bullet g_Bullets[BULLET_MAX]{};
static int g_Bullet_TexId = -1;

void Bullet_Initialize()
{
    g_Bullet_TexId = Texture_Load(L"assets/white.png");

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

        if (bullet.lifeTime >= 5.0f)
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
                    32.0f, 16.0f, 0, Color::BLUE
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
        bullet.velocity = {200.0f, 0.0f};
        break;
    }
}

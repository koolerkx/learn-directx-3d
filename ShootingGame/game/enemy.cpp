/**
 * @file enemy.cpp
 * @brief 敵機の管理
 * @author KOOLER FAN
 * @date 2025/07/02
 */

#include "enemy.h"

#include <DirectXMath.h>

#include "color.h"
#include "sprite.h"
#include "texture.h"
using namespace DirectX;

struct Enemy
{
    XMFLOAT2 position;
    XMFLOAT2 velocity;
    bool isEnable;
};

static constexpr unsigned int ENEMY_MAX = 256;
static Enemy g_Enemys[ENEMY_MAX]{};
static int g_Enemy_Texid = -1;

static constexpr XMFLOAT2 ENEMY_SIZE = {32.0f, 48.0f};
static constexpr XMFLOAT2 ENEMY_SPEED = {-100.0f, 0.0f};

void Enemy_Initialize()
{
    for (Enemy& enemy : g_Enemys)
    {
        enemy.isEnable = false;
    }

    g_Enemy_Texid = Texture_Load(L"assets/white.png");
}

void Enemy_Finalize()
{
}

void Enemy_Update(double elapsed_time)
{
    for (Enemy& enemy : g_Enemys)
    {
        if (!enemy.isEnable) continue;

        // 位置管理
        XMVECTOR position = XMLoadFloat2(&enemy.position);
        XMVECTOR velocity = XMLoadFloat2(&enemy.velocity);

        position += velocity * elapsed_time;

        XMStoreFloat2(&enemy.position, position);
        XMStoreFloat2(&enemy.velocity, velocity);


        if (enemy.position.x + ENEMY_SIZE.x < 0.0f)
        {
            enemy.isEnable = false;
        }
    }
}

void Enemy_Draw()
{
    for (Enemy& enemy : g_Enemys)
    {
        if (!enemy.isEnable) continue;

        Sprite_Draw(g_Enemy_Texid,
                    enemy.position.x, enemy.position.y,
                    ENEMY_SIZE.x, ENEMY_SIZE.y, 0, Color::RED
        );
    }
}

void Enemy_Create(const XMFLOAT2& position)
{
    for (Enemy& enemy : g_Enemys)
    {
        if (enemy.isEnable) continue;

        enemy.isEnable = true;
        enemy.position = position;
        enemy.velocity = ENEMY_SPEED;

        break;
    }
}

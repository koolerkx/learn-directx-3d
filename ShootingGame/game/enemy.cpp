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

struct EnemyType
{
    int texId = -1;
    // int tx, ty, tw, th; // スプライトシート UV Cut情報
    Color::COLOR color = Color::WHITE;
};

struct Enemy
{
    XMFLOAT2 position;
    XMFLOAT2 velocity;
    int typeId;
    float offsetY;
    double lifeTime;
    bool isEnable;
};

static constexpr unsigned int ENEMY_MAX = 256;
static Enemy g_Enemys[ENEMY_MAX]{};

static constexpr XMFLOAT2 ENEMY_SIZE = {32.0f, 48.0f};
static constexpr XMFLOAT2 ENEMY_SPEED = {-100.0f, 0.0f};

static EnemyType g_EnemyType[]
{
    {-1, Color::RED},
    {-1, Color::GREEN},
};

void Enemy_Initialize()
{
    for (Enemy& enemy : g_Enemys)
    {
        enemy.isEnable = false;
        enemy.offsetY = 0.0f;
    }

    g_EnemyType[0].texId = Texture_Load(L"assets/white.png");
    g_EnemyType[1].texId = Texture_Load(L"assets/white.png");
}

void Enemy_Finalize()
{
}

void Enemy_Update(double elapsed_time)
{
    for (Enemy& enemy : g_Enemys)
    {
        if (!enemy.isEnable) continue;

        // 敵機の挙動
        switch (static_cast<EnemyTypeID>(enemy.typeId))
        {
        case EnemyTypeID::RED:
            // 位置管理
            XMVECTOR position = XMLoadFloat2(&enemy.position);
            XMVECTOR velocity = XMLoadFloat2(&enemy.velocity);

            position += velocity * static_cast<float>(elapsed_time);

            XMStoreFloat2(&enemy.position, position);
            XMStoreFloat2(&enemy.velocity, velocity);

            break;
        case EnemyTypeID::GREEN:
            enemy.position.x += enemy.velocity.x * static_cast<float>(elapsed_time);
        // frequency and amplitude
            enemy.position.y += enemy.offsetY + static_cast<float>(cos(enemy.lifeTime)) * 3.5f * 20.0f;

            break;
        default: break;
        }

        enemy.lifeTime += elapsed_time;

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

        Sprite_Draw(g_EnemyType[enemy.typeId].texId,
                    enemy.position.x, enemy.position.y,
                    ENEMY_SIZE.x, ENEMY_SIZE.y,
                    0, g_EnemyType[enemy.typeId].color
        );
    }
}

void Enemy_Create(const XMFLOAT2& position, EnemyTypeID enemyTypeId)
{
    for (Enemy& enemy : g_Enemys)
    {
        if (enemy.isEnable) continue;

        enemy.isEnable = true;
        enemy.position = position;
        enemy.velocity = ENEMY_SPEED;
        enemy.typeId = static_cast<int>(enemyTypeId); // fixme

        break;
    }
}

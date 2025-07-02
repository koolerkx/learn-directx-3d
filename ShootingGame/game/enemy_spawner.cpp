/**
 * @file enemy_spawner.cpp
 * @brief 敵機の生成
 * @author KOOLER FAN
 * @date 2025/07/02
 */

#include "enemy_spawner.h"

#include <DirectXMath.h>

#include "debug_ostream.h"

using namespace DirectX;

struct EnemySpawn
{
    XMFLOAT2 position;
    EnemyTypeID id;
    double time; // 生成時間
    double rate; // 生成頻度
    int count; // 最大生成数
    double spawnTime; // 前回生成時間
    int spawnCount; // 生成した数
    bool isEnd;
};

static constexpr unsigned int ENEMY_SPAWNER_MAX = 256;
static EnemySpawn g_EnemySpawners[ENEMY_SPAWNER_MAX]{};
static int g_SpawnerCount = 0;
static double g_Time = 0.0;

void EnemySpawner_Initialize()
{
    Enemy_Initialize();
}

void EnemySpawner_Finalize()
{
    Enemy_Finalize();
}

void EnemySpawner_Update(double elapsed_time)
{
    g_Time += elapsed_time;

    for (int i = 0; i < g_SpawnerCount; i++)
    {
        if (g_EnemySpawners[i].isEnd) continue;
        if (g_Time < g_EnemySpawners[i].time) break;


        if (g_EnemySpawners[i].spawnCount == 0)
        {
            g_EnemySpawners[i].spawnTime = g_Time - g_EnemySpawners[i].rate;
        }

        if (g_Time - g_EnemySpawners[i].spawnTime >= g_EnemySpawners[i].rate)
        {
            Enemy_Create(g_EnemySpawners[i].position, g_EnemySpawners[i].id);
            g_EnemySpawners[i].spawnCount++;

            if (g_EnemySpawners[i].spawnCount >= g_EnemySpawners[i].count)
            {
                g_EnemySpawners[i].isEnd = true;
            }
            g_EnemySpawners[i].spawnTime = g_Time;
        }
    }
}

void EnemySpawner_Create(const XMFLOAT2& position, EnemyTypeID id, double spawn_time, double spawn_rate,
                         int spawn_count)
{
    if (g_SpawnerCount >= ENEMY_SPAWNER_MAX) return;

    EnemySpawn* pEnemySpawn = &g_EnemySpawners[g_SpawnerCount];
    pEnemySpawn->position = position;
    pEnemySpawn->id = id;
    pEnemySpawn->time = spawn_time;
    pEnemySpawn->rate = spawn_rate;
    pEnemySpawn->count = spawn_count;
    pEnemySpawn->isEnd = false;

    pEnemySpawn->spawnCount = 0;
    pEnemySpawn->spawnTime = 0;
    g_SpawnerCount++;
}

#include "game.h"

#include "bullet.h"
#include "direct3d.h"
#include "enemy.h"
#include "player.h"

static float fromLastEnemySpawnTime = 0;
static constexpr float ENEMY_SPAWN_INTERVAL = 1.0f;

void Game_Initialize()
{
    Player_Initialize({64.0f, 64.0f});
    Bullet_Initialize();
    Enemy_Initialize();
}

void Game_Finalize()
{
}

void Game_Update(double elapsed_time)
{

    fromLastEnemySpawnTime += elapsed_time;
    if (fromLastEnemySpawnTime > ENEMY_SPAWN_INTERVAL)
    {
        fromLastEnemySpawnTime = 0;
        
        Enemy_Create({
            static_cast<float>(Direct3D_GetBackBufferWidth()) - 100.0f,
            static_cast<float>(Direct3D_GetBackBufferHeight()) / 2.0f
        });
    }
    
    Player_Update(elapsed_time);
    Bullet_Update(elapsed_time);
    Enemy_Update(elapsed_time);
}

void Game_Draw()
{
    Enemy_Draw();
    Bullet_Draw();
    Player_Draw();
}

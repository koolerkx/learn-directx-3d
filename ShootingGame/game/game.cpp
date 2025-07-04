#include "game.h"

#include "bullet.h"
#include "direct3d.h"
#include "enemy_spawner.h"
#include "player.h"

void hitJudgementBilletVSEnemy();

void Game_Initialize()
{
    Player_Initialize({64.0f, 64.0f});
    Bullet_Initialize();
    EnemySpawner_Initialize();

    EnemySpawner_Create({
                            static_cast<float>(Direct3D_GetBackBufferWidth()) - 100.0f,
                            static_cast<float>(Direct3D_GetBackBufferHeight()) / 2.0f
                        }, EnemyTypeID::GREEN,
                        3.0f, 0.5, 5);


    EnemySpawner_Create({
                            static_cast<float>(Direct3D_GetBackBufferWidth()) - 100.0f,
                            static_cast<float>(Direct3D_GetBackBufferHeight()) / 2.0f
                        }, EnemyTypeID::RED,
                        4.0f, 0.5, 5);
}

void Game_Finalize()
{
    Player_Finalize();
    Bullet_Finalize();
    EnemySpawner_Finalize();
}

void Game_Update(double elapsed_time)
{
    EnemySpawner_Update(elapsed_time);
    Player_Update(elapsed_time);
    Bullet_Update(elapsed_time);
    Enemy_Update(elapsed_time);

    hitJudgementBilletVSEnemy();
}

void Game_Draw()
{
    Enemy_Draw();
    Bullet_Draw();
    Player_Draw();
}

void hitJudgementBilletVSEnemy()
{
    for (int bi = 0; bi < BULLET_MAX; bi++)
    {
        if (!Bullet_IsEnable(bi)) continue;

        for (int ei = 0; ei < ENEMY_MAX; ei++)
        {
            if (!Enemy_IsEnable(ei)) continue;

            if (Collision_IsOverlapCircle(Bullet_GetCollision(bi), Enemy_GetCollision(ei)))
            {
                Bullet_Destroy(bi);
                Enemy_Destroy(ei);
            }
        }
    }
}

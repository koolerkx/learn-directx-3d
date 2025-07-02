#include "game.h"

#include "bullet.h"
#include "direct3d.h"
#include "enemy_spawner.h"
#include "player.h"

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
}

void Game_Draw()
{
    Enemy_Draw();
    Bullet_Draw();
    Player_Draw();
}

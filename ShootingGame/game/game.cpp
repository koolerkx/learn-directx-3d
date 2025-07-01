#include "game.h"

#include "bullet.h"
#include "player.h"

void Game_Initialize()
{
    Player_Initialize({64.0f, 64.0f});
    Bullet_Initialize();
}

void Game_Finalize()
{
}

void Game_Update(double elapsed_time)
{
    Player_Update(elapsed_time);
    Bullet_Update(elapsed_time);
}

void Game_Draw()
{
    Bullet_Draw();
    Player_Draw();
}

#include "game.h"

#include "cube.h"
#include "grid.h"
#include "camera.h"

void Game_Initialize()
{
    Camera_Initialize();
}

void Game_Finalize()
{
    Camera_Finalize();
}

void Game_Update(double)
{}

void Game_Draw()
{
    Camera_Update();
    Grid_Draw();
    Cube_Draw();
}

#include "game.h"

#include "cube.h"
#include "grid.h"

void Game_Initialize()
{}

void Game_Finalize()
{}

void Game_Update(double)
{}

void Game_Draw()
{
    Grid_Draw();
    Cube_Draw();
}

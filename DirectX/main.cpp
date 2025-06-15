#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "game_window.h"

#include "direct3d.h"
#include "sprite.h"
#include "shader.h"

int APIENTRY WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE,
    _In_ LPSTR,
    _In_ int nCmdShow)
{
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    
    HWND hWnd = GameWindow_Create(hInstance);

    Direct3D_Initialize(hWnd);
    Shader_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
    Sprite_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // メッセージループ
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        
        Direct3D_Clear();
        
        for (int i = 0; i < 4; i++) {
            Sprite_Draw(32.0f + i * 384, 32.0f);
        }

        Direct3D_Present();
    }

    Sprite_Finalize();
    Shader_Finalize();
    Direct3D_Finalize();

    return (int)msg.wParam;
}

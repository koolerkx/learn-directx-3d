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


    // ゲームループ＆メッセージループ
    MSG msg;

    float x = 0.0f;

    do
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // ウィンドウメッセージが来ていたら 
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // ゲームの処理 
            Direct3D_Clear();
            for (int i = 0; i < 4; i++)
            {
                Sprite_Draw(x, 32.0f, 128.0f, 128.0f);

                Sprite_Draw(x, 265.0f, 512.0f, 512.0f);

                // x += 0.3f;
            }
            Direct3D_Present();
        }
    }
    while (msg.message != WM_QUIT);

    Sprite_Finalize();
    Shader_Finalize();
    Direct3D_Finalize();

    return (int)msg.wParam;
}

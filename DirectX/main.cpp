#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "game_window.h"

#include "direct3d.h"
#include "sprite.h"
#include "shader.h"
#include "texture.h"

#include "DirectXMath.h"
#include "sprite_anim.h"

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
    Texture_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
    SpriteAnim_Initialize();

    // テクスチャ読み込み
    int texid_knight_winter = Texture_Load(L"knight_3.png");
    // int texid_knight_summer = Texture_Load(L"knight_3_summer.png");
    int texid_kokosozai = Texture_Load(L"kokosozai.png");


    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);


    // ゲームループ＆メッセージループ
    MSG msg;

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
            
            Sprite_Begin();

            SpriteAnim_Draw();
            SpriteAnim_Update();

            DirectX::XMFLOAT4 color = {1.0f , 1.0f, 1.0f, 1.0f};
            
            Sprite_Draw(texid_knight_winter, 32.0f, 32.0f, color);
            Sprite_Draw(texid_knight_winter, 512.0f, 32.0f, 256, 256, color);
            Sprite_Draw(texid_kokosozai, 32.0f, 32.0f, 32.0f, 64.0f, 32.0f, 32.0f, color);
            Sprite_Draw(texid_kokosozai, 32.0f, 32.0f, 32.0f, 32.0f * 2, 32.0f, 32.0f, 256.0f, 256.0f, color);

            Direct3D_Present();
        }
    }
    while (msg.message != WM_QUIT);

    SpriteAnim_Finalize();
    Texture_Finalize();
    Sprite_Finalize();
    Shader_Finalize();
    Direct3D_Finalize();

    return (int)msg.wParam;
}

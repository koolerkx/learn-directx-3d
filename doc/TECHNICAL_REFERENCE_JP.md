# DirectX11 テンプレート - 技術リファレンス

このドキュメントは、DirectX11 テンプレートプロジェクトの詳細な技術情報、コード例、実装詳細を提供します。

**言語切り替え**: [English](TECHNICAL_REFERENCE.md) | 日本語

## アーキテクチャ概要

```
DirectXアプリケーションフロー:

    WinMain() エントリポイント
           │
           ▼
    ┌─────────────────┐    ┌─────────────────┐
    │  ゲームウィンドウ │    │   DirectX11     │
    │     作成        │?──?│   初期化        │
    └─────────────────┘    └─────────────────┘
           │                        │
           ▼                        ▼
    ┌─────────────────┐    ┌─────────────────┐
    │ 入力システム    │    │ 描画ループ      │
    │   初期化        │    │   開始          │
    └─────────────────┘    └─────────────────┘
           │                        │
           ▼                        ▼
    ┌─────────────────────────────────────────┐
    │            ゲームループ                  │
    │  ┌─────────┬─────────┬─────────────────┐│
    │  │ 更新    │  描画   │    画面表示     ││
    │  │ ロジック │ スプライト│                ││
    │  └─────────┴─────────┴─────────────────┘│
    └─────────────────────────────────────────┘
```

## コアシステム API

### DirectX11 デバイス管理（`direct3d.h/.cpp`）

**目的**: コア DirectX11 デバイスとコンテキスト初期化

```cpp
// DirectX11デバイスとスワップチェーンの初期化
bool Direct3D_Initialize(HWND hWnd);
void Direct3D_Clear();    // バックバッファークリア
void Direct3D_Present();  // 画面表示

// 他のシステム用デバイスとコンテキスト取得
ID3D11Device* Direct3D_GetDevice();
ID3D11DeviceContext* Direct3D_GetContext();

// バックバッファー寸法取得
unsigned int Direct3D_GetBackBufferWidth();
unsigned int Direct3D_GetBackBufferHeight();

// 使用例:
Direct3D_Initialize(hWnd);
Direct3D_Clear();
// 描画操作をここに...
Direct3D_Present();
```

### HLSL シェーダー管理（`shader.h/.cpp`）

**目的**: 頂点・ピクセルシェーダーのコンパイルと管理

```cpp
// シェーダーシステムがHLSLコンパイルを処理
void Shader_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Shader_Finalize();

// シェーダーファイルは自動でコンパイルされる:
// - shader_vertex_2d.hlsl → 頂点変換
// - shader_pixel_2d.hlsl → ピクセル/テクスチャ操作

// 使用方法:
Shader_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
```

### 2D スプライト描画エンジン（`sprite.h/.cpp`）

**目的**: 変換対応ハードウェアアクセラレート 2D スプライト描画

```cpp
// スプライトシステム初期化
void Sprite_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Sprite_Finalize();

// スプライトバッチ開始
void Sprite_Begin();

// 基本スプライト描画
void Sprite_Draw(int textureId, float x, float y, float width, float height);

// 回転とUV座標対応高度スプライト
void Sprite_Draw(int textureId, float x, float y,
                float srcX, float srcY, float srcW, float srcH,
                float dstW, float dstH, float angle);

// 色合い付きスプライト
void Sprite_Draw(int textureId, float x, float y, float width, float height,
                DirectX::XMFLOAT4 color);

// ゲームループでの使用例:
Sprite_Begin();
Sprite_Draw(knightTexture, 100, 100, 128, 128);
Sprite_Draw(backgroundTexture, 0, 0, screenWidth, screenHeight);
// 更なるスプライト描画...
```

### アニメーションシステム（`sprite_anim.h/.cpp`）

**目的**: フレームベーススプライトシートアニメーション管理

```cpp
// アニメーションシステム初期化
void SpriteAnim_Initialize();
void SpriteAnim_Finalize();

// スプライトシートからアニメーションパターン登録
int SpriteAnim_RegisterPattern(
    int textureId,        // スプライトシートテクスチャ
    int frameCount,       // フレーム数
    int totalFrames,      // 一行フレーム数
    float frameDuration,  // フレーム時間
    DirectX::XMINT2 frameSize,    // フレーム寸法 {幅, 高さ}
    DirectX::XMINT2 startPos,     // 開始位置 {x, y}
    bool loop = true      // ループアニメーション
);

// アニメーションプレイヤーインスタンス作成
int SpriteAnim_CreatePlayer(int patternId);

// ゲームループでの更新と描画
void SpriteAnim_Update(double deltaTime);
void SpriteAnim_Draw(int playerId, float x, float y, float width, float height);

// 使用例:
int walkPattern = SpriteAnim_RegisterPattern(
    characterTexture,  // 512x256スプライトシート
    8,                 // 8フレームアニメーション
    4,                // 一行フレーム数
    0.1f,              // フレーム100ms
    {32, 32},          // 32x32ピクセルフレーム
    {0, 0},            // 左上開始
    true               // ループアニメーション
);

int playerId = SpriteAnim_CreatePlayer(walkPattern);

// ゲームループ内:
SpriteAnim_Update(deltaTime);
SpriteAnim_Draw(playerId, playerX, playerY, 64, 64);
```

### テクスチャリソース管理（`texture.h/.cpp`）

**目的**: テクスチャリソースの読み込みと管理

```cpp
// テクスチャシステム初期化
void Texture_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Texture_Finalize();

// ファイルからテクスチャ読み込み
int Texture_Load(const wchar_t* filename);

// 描画用テクスチャバインド
void Texture_SetTexture(int textureId);

// 使用例:
int playerTexture = Texture_Load(L"assets/player.png");
int backgroundTexture = Texture_Load(L"assets/background.jpg");

// 描画時:
Texture_SetTexture(playerTexture);
Sprite_Draw(playerTexture, x, y, width, height);
```

### 基本ポリゴン描画（`polygon.h/.cpp`）

**目的**: シンプルなポリゴン/プリミティブ形状描画

```cpp
// ポリゴンシステム初期化
void Polygon_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Polygon_Finalize();

// 色付きポリゴン描画
void Polygon_Draw();

// 使用方法:
Polygon_Initialize(device, context);
// 単色用白テクスチャ設定
Texture_SetTexture(whiteTextureId);
Polygon_Draw(); // 事前設定されたポリゴン描画
```

## 入力システム API

### 高度入力ログ（`key_logger.h/.cpp`）

**目的**: トリガー検出付き拡張キー状態追跡

```cpp
// キー定数
enum KeyCode {
    KK_UP, KK_DOWN, KK_LEFT, KK_RIGHT,
    KK_W, KK_A, KK_S, KK_D,
    KK_SPACE, KK_ENTER,
    // ... 更なるキー
};

// 入力ログ初期化
void KeyLogger_Initialize();
void KeyLogger_Update();

// 入力状態確認
bool KeyLogger_IsPressed(KeyCode key);   // 押下中
bool KeyLogger_IsTrigger(KeyCode key);   // このフレームで押された
bool KeyLogger_IsRelease(KeyCode key);   // このフレームで離された

// ゲームループでの使用例:
KeyLogger_Update();

// WASDまたは矢印キーでの移動
if (KeyLogger_IsPressed(KK_UP) || KeyLogger_IsPressed(KK_W)) {
    player.y -= speed * deltaTime;
}
if (KeyLogger_IsPressed(KK_DOWN) || KeyLogger_IsPressed(KK_S)) {
    player.y += speed * deltaTime;
}
if (KeyLogger_IsPressed(KK_LEFT) || KeyLogger_IsPressed(KK_A)) {
    player.x -= speed * deltaTime;
}
if (KeyLogger_IsPressed(KK_RIGHT) || KeyLogger_IsPressed(KK_D)) {
    player.x += speed * deltaTime;
}

// キー押下時のみジャンプ（押し続けでない）
if (KeyLogger_IsTrigger(KK_SPACE)) {
    player.Jump();
}
```

### マウス入力管理（`mouse.h/.cpp`）

**目的**: マウス位置とボタン状態追跡

```cpp
// マウス状態構造体
typedef struct {
    int x, y;              // マウス位置
    bool leftButton;       // ボタン状態
    bool rightButton;
    bool middleButton;
    int wheelDelta;        // スクロールホイール
} Mouse_State;

// マウスシステム初期化
void Mouse_Initialize(HWND hWnd);
void Mouse_Finalize();

// 現在のマウス状態取得
void Mouse_GetState(Mouse_State* state);

// マウスカーソル表示/非表示
void Mouse_SetVisible(bool visible);

// 使用例:
Mouse_State mouseState;
Mouse_GetState(&mouseState);

// マウスに追従するスプライト
SpriteAnim_Draw(cursorSprite,
    mouseState.x - 32, mouseState.y - 32, 64, 64);

// クリック検出
if (mouseState.leftButton) {
    ProcessClick(mouseState.x, mouseState.y);
}
```

### Windows キーボード（`keyboard.h/.cpp`）

**目的**: 基本 Windows キーボード状態管理

```cpp
namespace hal {
    class Keyboard {
    public:
        Keyboard();
        ~Keyboard();

        void Update();
        bool IsKeyPressed(int virtualKey);
        bool IsKeyDown(int virtualKey);
        bool IsKeyUp(int virtualKey);
    };
}

// 使用方法:
hal::Keyboard keyboard;
keyboard.Update();

if (keyboard.IsKeyPressed(VK_SPACE)) {
    // スペースバーが現在押されている
}
```

## ユーティリティシステム API

### デバッグテキスト描画（`debug_text.h/.cpp`）

**目的**: SHIFT-JIS 対応画面デバッグ情報表示

```cpp
namespace hal {
    class DebugText {
    public:
        // フォントテクスチャ付きコンストラクタ
        DebugText(ID3D11Device* device, ID3D11DeviceContext* context,
                 const wchar_t* fontTexture,
                 int screenWidth, int screenHeight,
                 float startX = 0.0f, float startY = 0.0f,
                 int charWidth = 0, int charHeight = 0,
                 float scaleX = 1.0f, float scaleY = 1.0f);

        ~DebugText();

        // テキスト描画
        void SetText(const char* text);
        void Draw();
        void Clear();

        // テキスト位置設定
        void SetPosition(float x, float y);
        void SetScale(float scaleX, float scaleY);
    };
}

// 使用例:
hal::DebugText debugText(
    device, context,
    L"assets/consolab_ascii_512.png",  // フォントテクスチャ
    1024, 768,                         // 画面サイズ
    10.0f, 10.0f                       // 開始位置
);

// ゲームループ内:
std::stringstream ss;
ss << "FPS: " << fps;
ss << "\n位置: (" << playerX << ", " << playerY << ")";
ss << "\n体力: " << playerHealth;

debugText.SetText(ss.str().c_str());
debugText.Draw();
debugText.Clear();
```

### 高精度タイミング（`system_timer.h/.cpp`）

**目的**: ゲームタイミングとフレームレート制御

```cpp
// タイミングシステム初期化
void SystemTimer_Initialize();

// 現在時刻を秒で取得（高精度）
double SystemTimer_GetTime();

// フレームレート制御での使用例:
SystemTimer_Initialize();

double lastTime = SystemTimer_GetTime();
double fpsLastTime = lastTime;
ULONG frameCount = 0;
double fps = 0.0;

// ゲームループ内:
double currentTime = SystemTimer_GetTime();
double deltaTime = currentTime - lastTime;

// FPS計算（毎秒）
double fpsElapsed = currentTime - fpsLastTime;
if (fpsElapsed >= 1.0) {
    fps = frameCount / fpsElapsed;
    fpsLastTime = currentTime;
    frameCount = 0;
}

// 60 FPSフレーム制限
if (deltaTime >= (1.0 / 60.0)) {
    UpdateGame(deltaTime);
    RenderGame();
    lastTime = currentTime;
    frameCount++;
}
```

### デバッグ出力ストリーミング（`debug_ostream.h/.cpp`）

**目的**: SHIFT-JIS 対応拡張デバッグ出力

```cpp
// カスタムデバッグ出力関数
void DebugOutput(const char* message);
void DebugOutputFormat(const char* format, ...);

// 一般型用ストリーム演算子
template<typename T>
void DebugStream(const T& value);

// 使用方法:
DebugOutput("ゲーム初期化成功");
DebugOutputFormat("プレイヤー位置: (%.2f, %.2f)", x, y);
DebugStream(playerHealth);
```

## ウィンドウ管理 API

### Windows アプリケーションフレームワーク（`game_window.h/.cpp`）

**目的**: Win32 ウィンドウ作成とメッセージ処理

```cpp
// アプリケーションウィンドウ作成
HWND GameWindow_Create(HINSTANCE hInstance);

// ウィンドウ設定:
// - DirectX互換設定でのウィンドウクラス登録
// - 適切な寸法でのウィンドウ作成
// - メッセージ処理セットアップ
// - DirectX11スワップチェーン互換

// 使用方法:
HWND hWnd = GameWindow_Create(hInstance);
if (!hWnd) {
    return -1; // ウィンドウ作成失敗
}

ShowWindow(hWnd, nCmdShow);
UpdateWindow(hWnd);
```

## シェーダープログラミング

### 2D 頂点シェーダー（`shader_vertex_2d.hlsl`）

**目的**: 2D スプライトの画面空間変換

```hlsl
// 変換行列用定数バッファー
cbuffer VS_CONSTANT_BUFFER: register(b0) {
    float4x4 proj;    // 投影行列（正射影）
};

cbuffer VS_CONSTANT_BUFFER: register(b1) {
    float4x4 world;   // ワールド変換行列
};

// 頂点入力構造体
struct VS_INPUT {
    float4 posL : POSITION0;  // ローカル位置
    float4 color : COLOR0;    // 頂点色
    float2 uv : TEXCOORD0;    // テクスチャ座標
};

// 頂点出力構造体
struct VS_OUTPUT {
    float4 posH : SV_POSITION; // 同次座標位置
    float4 color : COLOR0;     // 色（パススルー）
    float2 uv : TEXCOORD0;     // UV座標（パススルー）
};

// 頂点変換パイプライン
VS_OUTPUT main(VS_INPUT vs_in) {
    VS_OUTPUT vs_out;

    // 頂点位置変換: ローカル → ワールド → 投影
    vs_in.posL = mul(vs_in.posL, world);
    vs_out.posH = mul(vs_in.posL, proj);

    // 色とUV座標をパススルー
    vs_out.color = vs_in.color;
    vs_out.uv = vs_in.uv;

    return vs_out;
}
```

### 2D ピクセルシェーダー（`shader_pixel_2d.hlsl`）

**目的**: テクスチャサンプリングと色合成

```hlsl
// テクスチャとサンプラー
Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

// ピクセル入力（頂点シェーダーから）
struct PS_INPUT {
    float4 posH : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

// ピクセルシェーダーメイン関数
float4 main(PS_INPUT ps_in) : SV_Target {
    // テクスチャサンプリング
    float4 texColor = tex.Sample(samLinear, ps_in.uv);

    // 色調整用頂点色と乗算
    return texColor * ps_in.color;
}
```

## ゲームループ実装

### 完全アプリケーション例（`main.cpp`）

**目的**: 完全ゲームアプリケーションでのすべてのシステム統合

```cpp
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    // DirectX用COM初期化
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    // アプリケーションウィンドウ作成
    HWND hWnd = GameWindow_Create(hInstance);
    if (!hWnd) return -1;

    // 全サブシステム初期化
    SystemTimer_Initialize();
    KeyLogger_Initialize();
    Mouse_Initialize(hWnd);

    Direct3D_Initialize(hWnd);
    Shader_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
    Sprite_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
    Texture_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
    SpriteAnim_Initialize();
    Polygon_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());

    // ゲームリソース読み込み
    int playerTexture = Texture_Load(L"assets/player.png");
    int backgroundTexture = Texture_Load(L"assets/background.png");

    // アニメーション作成
    int walkAnimation = SpriteAnim_RegisterPattern(
        playerTexture, 8, 16, 0.1f, {32, 32}, {0, 0}, true);
    int playerId = SpriteAnim_CreatePlayer(walkAnimation);

    // デバッグテキスト初期化
    hal::DebugText debugText(
        Direct3D_GetDevice(), Direct3D_GetContext(),
        L"assets/font.png",
        Direct3D_GetBackBufferWidth(),
        Direct3D_GetBackBufferHeight()
    );

    // ウィンドウ表示
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // ゲーム状態変数
    float playerX = 400.0f, playerY = 300.0f;
    float speed = 200.0f;

    // タイミング変数
    double lastTime = SystemTimer_GetTime();
    double fpsLastTime = lastTime;
    ULONG frameCount = 0;
    double fps = 0.0;

    // ゲームループ
    MSG msg;
    do {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            // タイミング
            double currentTime = SystemTimer_GetTime();
            double deltaTime = currentTime - lastTime;

            // FPS計算
            double fpsElapsed = currentTime - fpsLastTime;
            if (fpsElapsed >= 1.0) {
                fps = frameCount / fpsElapsed;
                fpsLastTime = currentTime;
                frameCount = 0;
            }

            // 60 FPSフレーム制限
            if (deltaTime >= (1.0 / 60.0)) {
                lastTime = currentTime;

                // 入力処理
                KeyLogger_Update();

                // プレイヤー移動
                if (KeyLogger_IsPressed(KK_LEFT) || KeyLogger_IsPressed(KK_A)) {
                    playerX -= speed * deltaTime;
                }
                if (KeyLogger_IsPressed(KK_RIGHT) || KeyLogger_IsPressed(KK_D)) {
                    playerX += speed * deltaTime;
                }
                if (KeyLogger_IsPressed(KK_UP) || KeyLogger_IsPressed(KK_W)) {
                    playerY -= speed * deltaTime;
                }
                if (KeyLogger_IsPressed(KK_DOWN) || KeyLogger_IsPressed(KK_S)) {
                    playerY += speed * deltaTime;
                }

                // マウス入力
                Mouse_State mouseState;
                Mouse_GetState(&mouseState);

                // XInputゲームパッド対応
                XINPUT_STATE xinputState;
                if (XInputGetState(0, &xinputState) == ERROR_SUCCESS) {
                    // アナログスティック移動
                    float stickX = xinputState.Gamepad.sThumbLX / 32767.0f;
                    float stickY = xinputState.Gamepad.sThumbLY / 32767.0f;
                    playerX += stickX * speed * deltaTime;
                    playerY -= stickY * speed * deltaTime;

                    // ボタン押下時バイブレーション
                    XINPUT_VIBRATION vibration = {0, 0};
                    if (xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
                        vibration.wLeftMotorSpeed = 65535;
                        vibration.wRightMotorSpeed = 65535;
                    }
                    XInputSetState(0, &vibration);
                }

                // 描画
                Direct3D_Clear();
                Sprite_Begin();

                // 背景描画
                Sprite_Draw(backgroundTexture, 0, 0,
                    Direct3D_GetBackBufferWidth(),
                    Direct3D_GetBackBufferHeight());

                // アニメーション更新と描画
                SpriteAnim_Update(deltaTime);
                SpriteAnim_Draw(playerId, playerX, playerY, 64, 64);

                // マウスカーソル描画
                SpriteAnim_Draw(playerId,
                    mouseState.x - 32, mouseState.y - 32, 64, 64);

#if defined(DEBUG) || defined(_DEBUG)
                // デバッグ情報
                std::stringstream ss;
                ss << "FPS: " << (int)fps;
                ss << "\nプレイヤー: (" << (int)playerX << ", " << (int)playerY << ")";
                ss << "\nマウス: (" << mouseState.x << ", " << mouseState.y << ")";

                debugText.SetText(ss.str().c_str());
                debugText.Draw();
                debugText.Clear();
#endif

                Direct3D_Present();
                frameCount++;
            }
        }
    } while (msg.message != WM_QUIT);

    // クリーンアップ
    SpriteAnim_Finalize();
    Polygon_Finalize();
    Texture_Finalize();
    Sprite_Finalize();
    Shader_Finalize();
    Direct3D_Finalize();
    Mouse_Finalize();

    return (int)msg.wParam;
}
```

## ビルド設定

### SHIFT-JIS 用プロジェクト設定

プロジェクトは SHIFT-JIS エンコーディング用特定設定で構成されています：

- **文字セット**: マルチバイト（Unicode ではない）
- **ランタイムライブラリ**: Release 用 MT（マルチスレッド）、Debug 用 MTd
- **プラットフォームツールセット**: v143（Visual Studio 2022）
- **DirectX SDK**: Windows 10 SDK 含有
- **追加ライブラリ**: XInput.lib、DirectXTex ライブラリ

### シェーダーコンパイル

HLSL シェーダーはビルドプロセス一部として自動コンパイルされます：

- 頂点シェーダーは`vs_5_0`プロファイルでコンパイル
- ピクセルシェーダーは`ps_5_0`プロファイルでコンパイル
- コンパイル済みシェーダーオブジェクト（.cso）はプロジェクトディレクトリにコピー

# DirectX11 Template - Technical Reference

This document provides detailed technical information, code examples, and implementation details for the DirectX11 template project.

**Language**: English | [日本語](TECHNICAL_REFERENCE_JP.md)

## Architecture Overview

```
DirectX Application Flow:

    WinMain() Entry Point
           │
           ▼
    ┌─────────────────┐    ┌─────────────────┐
    │  Game Window    │    │   DirectX11     │
    │   Creation      │?──?│ Initialization  │
    └─────────────────┘    └─────────────────┘
           │                        │
           ▼                        ▼
    ┌─────────────────┐    ┌─────────────────┐
    │ Input Systems   │    │ Rendering Loop  │
    │ Initialization  │    │     Begin       │
    └─────────────────┘    └─────────────────┘
           │                        │
           ▼                        ▼
    ┌─────────────────────────────────────────┐
    │            Game Loop                    │
    │  ┌─────────┬─────────┬─────────────────┐│
    │  │ Update  │  Draw   │     Present     ││
    │  │ Logic   │ Sprites │   to Screen     ││
    │  └─────────┴─────────┴─────────────────┘│
    └─────────────────────────────────────────┘
```

## Core System APIs

### DirectX11 Device Management (`direct3d.h/.cpp`)

**Purpose**: Core DirectX11 device and context initialization

```cpp
// Initialize DirectX11 device and swap chain
bool Direct3D_Initialize(HWND hWnd);
void Direct3D_Clear();    // Clear back buffer
void Direct3D_Present();  // Present to screen

// Get device and context for other systems
ID3D11Device* Direct3D_GetDevice();
ID3D11DeviceContext* Direct3D_GetContext();

// Get back buffer dimensions
unsigned int Direct3D_GetBackBufferWidth();
unsigned int Direct3D_GetBackBufferHeight();

// Usage example:
Direct3D_Initialize(hWnd);
Direct3D_Clear();
// Draw operations here...
Direct3D_Present();
```

### HLSL Shader Management (`shader.h/.cpp`)

**Purpose**: Compile and manage vertex/pixel shaders

```cpp
// Shader system handles HLSL compilation
void Shader_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Shader_Finalize();

// Shader files are automatically compiled:
// - shader_vertex_2d.hlsl → Vertex transformations
// - shader_pixel_2d.hlsl → Pixel/texture operations

// Usage:
Shader_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
```

### 2D Sprite Rendering Engine (`sprite.h/.cpp`)

**Purpose**: Hardware-accelerated 2D sprite rendering with transforms

```cpp
// Initialize sprite system
void Sprite_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Sprite_Finalize();

// Begin sprite batch
void Sprite_Begin();

// Basic sprite drawing
void Sprite_Draw(int textureId, float x, float y, float width, float height);

// Advanced sprite with rotation and UV coordinates
void Sprite_Draw(int textureId, float x, float y,
                float srcX, float srcY, float srcW, float srcH,
                float dstW, float dstH, float angle);

// Color tinted sprite
void Sprite_Draw(int textureId, float x, float y, float width, float height,
                DirectX::XMFLOAT4 color);

// Usage in game loop:
Sprite_Begin();
Sprite_Draw(knightTexture, 100, 100, 128, 128);
Sprite_Draw(backgroundTexture, 0, 0, screenWidth, screenHeight);
// More sprite draws...
```

### Animation System (`sprite_anim.h/.cpp`)

**Purpose**: Frame-based sprite sheet animation management

```cpp
// Initialize animation system
void SpriteAnim_Initialize();
void SpriteAnim_Finalize();

// Register animation pattern from sprite sheet
int SpriteAnim_RegisterPattern(
    int textureId,        // Sprite sheet texture
    int frameCount,       // Number of frames
    int totalFrames,      // frames each line in sheet
    float frameDuration,  // Time per frame
    DirectX::XMINT2 frameSize,    // Frame dimensions {width, height}
    DirectX::XMINT2 startPos,     // Start position {x, y}
    bool loop = true      // Loop animation
);

// Create animation player instance
int SpriteAnim_CreatePlayer(int patternId);

// Update and draw in game loop
void SpriteAnim_Update(double deltaTime);
void SpriteAnim_Draw(int playerId, float x, float y, float width, float height);

// Example usage:
int walkPattern = SpriteAnim_RegisterPattern(
    characterTexture,  // 512x256 sprite sheet
    8,                 // 8 frames of animation
    4,                 // 4 frames each line in sprite sheet
    0.1f,              // 100ms per frame
    {32, 32},          // 32x32 pixel frames
    {0, 0},            // Start at top-left
    true               // Loop animation
);

int playerId = SpriteAnim_CreatePlayer(walkPattern);

// In game loop:
SpriteAnim_Update(deltaTime);
SpriteAnim_Draw(playerId, playerX, playerY, 64, 64);
```

### Texture Resource Management (`texture.h/.cpp`)

**Purpose**: Loading and managing texture resources

```cpp
// Initialize texture system
void Texture_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Texture_Finalize();

// Load texture from file
int Texture_Load(const wchar_t* filename);

// Bind texture for rendering
void Texture_SetTexture(int textureId);

// Example usage:
int playerTexture = Texture_Load(L"assets/player.png");
int backgroundTexture = Texture_Load(L"assets/background.jpg");

// In rendering:
Texture_SetTexture(playerTexture);
Sprite_Draw(playerTexture, x, y, width, height);
```

### Basic Polygon Rendering (`polygon.h/.cpp`)

**Purpose**: Simple polygon/primitive shape rendering

```cpp
// Initialize polygon system
void Polygon_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Polygon_Finalize();

// Draw colored polygons
void Polygon_Draw();

// Usage:
Polygon_Initialize(device, context);
// Set white texture for solid colors
Texture_SetTexture(whiteTextureId);
Polygon_Draw(); // Draws pre-configured polygon
```

## Input System APIs

### Advanced Input Logging (`key_logger.h/.cpp`)

**Purpose**: Enhanced key state tracking with trigger detection

```cpp
// Key constants
enum KeyCode {
    KK_UP, KK_DOWN, KK_LEFT, KK_RIGHT,
    KK_W, KK_A, KK_S, KK_D,
    KK_SPACE, KK_ENTER,
    // ... more keys
};

// Initialize input logging
void KeyLogger_Initialize();
void KeyLogger_Update();

// Check input states
bool KeyLogger_IsPressed(KeyCode key);   // Held down
bool KeyLogger_IsTrigger(KeyCode key);   // Just pressed this frame
bool KeyLogger_IsRelease(KeyCode key);   // Just released this frame

// Usage in game loop:
KeyLogger_Update();

// Movement with WASD or arrow keys
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

// Jump only on key press (not held)
if (KeyLogger_IsTrigger(KK_SPACE)) {
    player.Jump();
}
```

### Mouse Input Management (`mouse.h/.cpp`)

**Purpose**: Mouse position and button state tracking

```cpp
// Mouse state structure
typedef struct {
    int x, y;              // Mouse position
    bool leftButton;       // Button states
    bool rightButton;
    bool middleButton;
    int wheelDelta;        // Scroll wheel
} Mouse_State;

// Initialize mouse system
void Mouse_Initialize(HWND hWnd);
void Mouse_Finalize();

// Get current mouse state
void Mouse_GetState(Mouse_State* state);

// Show/hide mouse cursor
void Mouse_SetVisible(bool visible);

// Usage example:
Mouse_State mouseState;
Mouse_GetState(&mouseState);

// Follow mouse with sprite
SpriteAnim_Draw(cursorSprite,
    mouseState.x - 32, mouseState.y - 32, 64, 64);

// Click detection
if (mouseState.leftButton) {
    ProcessClick(mouseState.x, mouseState.y);
}
```

### Windows Keyboard (`keyboard.h/.cpp`)

**Purpose**: Basic Windows keyboard state management

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

// Usage:
hal::Keyboard keyboard;
keyboard.Update();

if (keyboard.IsKeyPressed(VK_SPACE)) {
    // Space bar is currently pressed
}
```

## Utility System APIs

### Debug Text Rendering (`debug_text.h/.cpp`)

**Purpose**: On-screen debug information display with SHIFT-JIS support

```cpp
namespace hal {
    class DebugText {
    public:
        // Constructor with font texture
        DebugText(ID3D11Device* device, ID3D11DeviceContext* context,
                 const wchar_t* fontTexture,
                 int screenWidth, int screenHeight,
                 float startX = 0.0f, float startY = 0.0f,
                 int charWidth = 0, int charHeight = 0,
                 float scaleX = 1.0f, float scaleY = 1.0f);

        ~DebugText();

        // Text rendering
        void SetText(const char* text);
        void Draw();
        void Clear();

        // Text positioning
        void SetPosition(float x, float y);
        void SetScale(float scaleX, float scaleY);
    };
}

// Usage example:
hal::DebugText debugText(
    device, context,
    L"assets/consolab_ascii_512.png",  // Font texture
    1024, 768,                         // Screen size
    10.0f, 10.0f                       // Start position
);

// In game loop:
std::stringstream ss;
ss << "FPS: " << fps;
ss << "\nPosition: (" << playerX << ", " << playerY << ")";
ss << "\nHealth: " << playerHealth;

debugText.SetText(ss.str().c_str());
debugText.Draw();
debugText.Clear();
```

### High-Precision Timing (`system_timer.h/.cpp`)

**Purpose**: Game timing and frame rate control

```cpp
// Initialize timing system
void SystemTimer_Initialize();

// Get current time in seconds (high precision)
double SystemTimer_GetTime();

// Usage for frame rate control:
SystemTimer_Initialize();

double lastTime = SystemTimer_GetTime();
double fpsLastTime = lastTime;
ULONG frameCount = 0;
double fps = 0.0;

// In game loop:
double currentTime = SystemTimer_GetTime();
double deltaTime = currentTime - lastTime;

// FPS calculation (every second)
double fpsElapsed = currentTime - fpsLastTime;
if (fpsElapsed >= 1.0) {
    fps = frameCount / fpsElapsed;
    fpsLastTime = currentTime;
    frameCount = 0;
}

// 60 FPS frame limiting
if (deltaTime >= (1.0 / 60.0)) {
    UpdateGame(deltaTime);
    RenderGame();
    lastTime = currentTime;
    frameCount++;
}
```

### Debug Output Streaming (`debug_ostream.h/.cpp`)

**Purpose**: Enhanced debug output with SHIFT-JIS support

```cpp
// Custom debug output functions
void DebugOutput(const char* message);
void DebugOutputFormat(const char* format, ...);

// Stream operators for common types
template<typename T>
void DebugStream(const T& value);

// Usage:
DebugOutput("Game initialized successfully");
DebugOutputFormat("Player position: (%.2f, %.2f)", x, y);
DebugStream(playerHealth);
```

## Window Management API

### Windows Application Framework (`game_window.h/.cpp`)

**Purpose**: Win32 window creation and message handling

```cpp
// Create application window
HWND GameWindow_Create(HINSTANCE hInstance);

// Window configuration:
// - Window class registration with DirectX-compatible settings
// - Window creation with proper dimensions
// - Message handling setup
// - Compatible with DirectX11 swap chain

// Usage:
HWND hWnd = GameWindow_Create(hInstance);
if (!hWnd) {
    return -1; // Window creation failed
}

ShowWindow(hWnd, nCmdShow);
UpdateWindow(hWnd);
```

## Shader Programming

### 2D Vertex Shader (`shader_vertex_2d.hlsl`)

**Purpose**: Transform 2D sprites to screen space

```hlsl
// Constant buffers for transformation matrices
cbuffer VS_CONSTANT_BUFFER: register(b0) {
    float4x4 proj;    // Projection matrix (orthographic)
};

cbuffer VS_CONSTANT_BUFFER: register(b1) {
    float4x4 world;   // World transformation matrix
};

// Vertex input structure
struct VS_INPUT {
    float4 posL : POSITION0;  // Local position
    float4 color : COLOR0;    // Vertex color
    float2 uv : TEXCOORD0;    // Texture coordinates
};

// Vertex output structure
struct VS_OUTPUT {
    float4 posH : SV_POSITION; // Homogeneous position
    float4 color : COLOR0;     // Color (passed through)
    float2 uv : TEXCOORD0;     // UV coordinates (passed through)
};

// Vertex transformation pipeline
VS_OUTPUT main(VS_INPUT vs_in) {
    VS_OUTPUT vs_out;

    // Transform vertex position: Local → World → Projection
    vs_in.posL = mul(vs_in.posL, world);
    vs_out.posH = mul(vs_in.posL, proj);

    // Pass through color and UV coordinates
    vs_out.color = vs_in.color;
    vs_out.uv = vs_in.uv;

    return vs_out;
}
```

### 2D Pixel Shader (`shader_pixel_2d.hlsl`)

**Purpose**: Texture sampling and color blending

```hlsl
// Texture and sampler
Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

// Pixel input (from vertex shader)
struct PS_INPUT {
    float4 posH : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

// Pixel shader main function
float4 main(PS_INPUT ps_in) : SV_Target {
    // Sample texture
    float4 texColor = tex.Sample(samLinear, ps_in.uv);

    // Multiply with vertex color for tinting
    return texColor * ps_in.color;
}
```

## Game Loop Implementation

### Complete Application Example (`main.cpp`)

**Purpose**: Integration of all systems in a complete game application

```cpp
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    // COM initialization for DirectX
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    // Create application window
    HWND hWnd = GameWindow_Create(hInstance);
    if (!hWnd) return -1;

    // Initialize all subsystems
    SystemTimer_Initialize();
    KeyLogger_Initialize();
    Mouse_Initialize(hWnd);

    Direct3D_Initialize(hWnd);
    Shader_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
    Sprite_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
    Texture_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
    SpriteAnim_Initialize();
    Polygon_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());

    // Load game resources
    int playerTexture = Texture_Load(L"assets/player.png");
    int backgroundTexture = Texture_Load(L"assets/background.png");

    // Create animations
    int walkAnimation = SpriteAnim_RegisterPattern(
        playerTexture, 8, 16, 0.1f, {32, 32}, {0, 0}, true);
    int playerId = SpriteAnim_CreatePlayer(walkAnimation);

    // Initialize debug text
    hal::DebugText debugText(
        Direct3D_GetDevice(), Direct3D_GetContext(),
        L"assets/font.png",
        Direct3D_GetBackBufferWidth(),
        Direct3D_GetBackBufferHeight()
    );

    // Show window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Game state variables
    float playerX = 400.0f, playerY = 300.0f;
    float speed = 200.0f;

    // Timing variables
    double lastTime = SystemTimer_GetTime();
    double fpsLastTime = lastTime;
    ULONG frameCount = 0;
    double fps = 0.0;

    // Game loop
    MSG msg;
    do {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            // Timing
            double currentTime = SystemTimer_GetTime();
            double deltaTime = currentTime - lastTime;

            // FPS calculation
            double fpsElapsed = currentTime - fpsLastTime;
            if (fpsElapsed >= 1.0) {
                fps = frameCount / fpsElapsed;
                fpsLastTime = currentTime;
                frameCount = 0;
            }

            // 60 FPS frame limiting
            if (deltaTime >= (1.0 / 60.0)) {
                lastTime = currentTime;

                // Input processing
                KeyLogger_Update();

                // Player movement
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

                // Mouse input
                Mouse_State mouseState;
                Mouse_GetState(&mouseState);

                // XInput gamepad support
                XINPUT_STATE xinputState;
                if (XInputGetState(0, &xinputState) == ERROR_SUCCESS) {
                    // Analog stick movement
                    float stickX = xinputState.Gamepad.sThumbLX / 32767.0f;
                    float stickY = xinputState.Gamepad.sThumbLY / 32767.0f;
                    playerX += stickX * speed * deltaTime;
                    playerY -= stickY * speed * deltaTime;

                    // Vibration on button press
                    XINPUT_VIBRATION vibration = {0, 0};
                    if (xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
                        vibration.wLeftMotorSpeed = 65535;
                        vibration.wRightMotorSpeed = 65535;
                    }
                    XInputSetState(0, &vibration);
                }

                // Rendering
                Direct3D_Clear();
                Sprite_Begin();

                // Draw background
                Sprite_Draw(backgroundTexture, 0, 0,
                    Direct3D_GetBackBufferWidth(),
                    Direct3D_GetBackBufferHeight());

                // Update and draw animations
                SpriteAnim_Update(deltaTime);
                SpriteAnim_Draw(playerId, playerX, playerY, 64, 64);

                // Draw mouse cursor
                SpriteAnim_Draw(playerId,
                    mouseState.x - 32, mouseState.y - 32, 64, 64);

#if defined(DEBUG) || defined(_DEBUG)
                // Debug information
                std::stringstream ss;
                ss << "FPS: " << (int)fps;
                ss << "\nPlayer: (" << (int)playerX << ", " << (int)playerY << ")";
                ss << "\nMouse: (" << mouseState.x << ", " << mouseState.y << ")";

                debugText.SetText(ss.str().c_str());
                debugText.Draw();
                debugText.Clear();
#endif

                Direct3D_Present();
                frameCount++;
            }
        }
    } while (msg.message != WM_QUIT);

    // Cleanup
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

## Build Configuration

### Project Settings for SHIFT-JIS

The project is configured with specific settings for SHIFT-JIS encoding:

- **Character Set**: MultiByte (not Unicode)
- **Runtime Library**: MT (Multi-threaded) for Release, MTd for Debug
- **Platform Toolset**: v143 (Visual Studio 2022)
- **DirectX SDK**: Windows 10 SDK included
- **Additional Libraries**: XInput.lib, DirectXTex libraries

### Shader Compilation

HLSL shaders are automatically compiled as part of the build process:

- Vertex shaders compiled with `vs_5_0` profile
- Pixel shaders compiled with `ps_5_0` profile
- Compiled shader objects (.cso) are copied to project directory

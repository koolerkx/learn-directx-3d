## Shooting Game

* **Project Overview**

  * DirectX11 Shooting Game
  * Developed with SHIFT-JIS encoding and Japanese comments

* **Language Options**

  * English (default)
  * [日本語](README_JP.md)

---

### Key Features

1. **Core Graphics System**

   * DirectX11 Device and Context management
   * 2D sprite rendering with transform support
   * Frame-based sprite sheet animation
   * Texture loading and resource management
   * HLSL shader compilation and management
   * Basic polygon rendering

2. **Input Handling**

   * Keyboard: key press, release, and trigger detection
   * Mouse: position and button state tracking
   * XInput Gamepad: analog sticks and vibration
   * Customizable key mapping

3. **Development Utilities**

   * High-precision timing for consistent frame rate
   * Debug text rendering (SHIFT-JIS font support)
   * Enhanced debug output streaming
   * FPS monitoring and performance metrics

---

### Project Structure

```text
learn-directX/
├── README.md                    # Project overview (this file)
├── TECHNICAL_REFERENCE.md       # API documentation and examples
├── DirectX.sln                  # Visual Studio solution file
├── DirectX/                     # Main source code
│   ├── DirectX.vcxproj          # Project settings
│   ├── main.cpp                 # Entry point and game loop
│   ├── direct3d/                # Graphics core
│   │   ├── direct3d.h/.cpp      # Device & context management
│   │   ├── shader.h/.cpp        # Shader compilation and management
│   │   ├── sprite.h/.cpp        # Sprite rendering and animation
│   │   ├── texture.h/.cpp       # Texture loading
│   │   └── polygon.h/.cpp       # Polygon rendering
│   ├── game_window/             # Win32 window management
│   ├── input/                   # Input systems
│   ├── utils/                   # Utility functions
│   ├── shaders/                 # HLSL source files
│   └── assets/                  # Textures and compiled shaders
└── doc/                         # Documentation and setup guides
    ├── TECHNICAL_REFERENCE.md   # English API reference
    ├── TECHNICAL_REFERENCE_JP.md# Japanese API reference
    └── プロジェクト設定.md      # Project setup (Japanese)
```

---

### Documentation

* [Technical Reference (English)](doc/TECHNICAL_REFERENCE.md)
* [Technical Reference (日本語)](doc/TECHNICAL_REFERENCE_JP.md)

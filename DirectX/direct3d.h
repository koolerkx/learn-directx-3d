/**
 * @file direct3d.h
 * @brief Direct3Dの初期化関連
 * @author KOOLER FAN
 * @date 2025/06/06
 */

#ifndef DIRECT3D_H
#define DIRECT3D_H

#include <Windows.h>

// セーフリリースマクロ
#define SAFE_RELEASE(o) if (o) { (o)->Release(); o = NULL; }

bool Direct3D_Initialize(HWND hWnd); // Direct3Dの初期化
void Direct3D_Finalize(); // Direct3Dの終了処理

void Direct3D_Clear(); // バックバッファのクリア
void Direct3D_Present(); // バックバッファの表示

#endif // DIRECT3D_H

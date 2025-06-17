/**
 * @file sprite.h
 * @brief スプライト表示
 * @author KOOLER FAN
 * @date 2025/06/12
 */

#ifndef SPRITE_H
#define SPRITE_H

#include <d3d11.h>
#include <DirectXMath.h>

void Sprite_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

void Sprite_Finalize(void);

void Sprite_Begin();

/// @brief テクスチャ全表示（表示サイズ変更可）
void Sprite_Draw(int texid, float dx, float dy, const DirectX::XMFLOAT4& color = {1.0f, 1.0f, 1.0f, 1.0f});

/// @brief テクスチャ全表示（表示サイズ変更可）
void Sprite_Draw(int texid, float dx, float dy, float dw, float dh, const DirectX::XMFLOAT4& color = {1.0f, 1.0f, 1.0f, 1.0f});

/// @brief UVカット
void Sprite_Draw(int texid, float dx, float dy, float px, float py, float pw, float ph, const DirectX::XMFLOAT4& color = {1.0f, 1.0f, 1.0f, 1.0f});

/// @brief UVカット（表示サイズ変更可）
void Sprite_Draw(int texid, float dx, float dy, float px, float py, float pw, float ph, float tw, float th, const DirectX::XMFLOAT4& color = {1.0f, 1.0f, 1.0f, 1.0f});

#endif // SPRITE_H

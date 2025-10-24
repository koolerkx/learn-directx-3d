/**
 * @file light.cpp
 * @brief ライトの設定
 * 
 * @author KOOLER FAN
 * @date 2025/09/30
 */

#include "light.h"
#include "debug_ostream.h"

#include "direct3d.h"
#include <DirectXMath.h>

using namespace DirectX;

static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

static ID3D11Buffer* g_pPSConstantBuffer1 = nullptr; // Ambient Light
static ID3D11Buffer* g_pPSConstantBuffer2 = nullptr; // Directional Light

struct AmbientLightBuffer
{
    XMFLOAT3 color;
    float pad;
};

struct DirectionalLightBuffer
{
    XMFLOAT4 world_vector;
    XMFLOAT3 color;
    float pad;
    XMFLOAT3 eye_posW;
    float pad2;
};

void Light_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    // デバイスとデバイスコンテキストのチェック
    if (!pDevice || !pContext)
    {
        hal::dout << "Polygon_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
        return;
    }

    g_pDevice = pDevice;
    g_pContext = pContext;

    // 頂点シェーダー用定数バッファの作成
    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // バインドフラグ

    buffer_desc.ByteWidth = sizeof(XMFLOAT4); // バッファのサイズ
    g_pDevice->CreateBuffer(&buffer_desc, nullptr, &g_pPSConstantBuffer1);
    buffer_desc.ByteWidth = sizeof(DirectionalLightBuffer); // バッファのサイズ
    g_pDevice->CreateBuffer(&buffer_desc, nullptr, &g_pPSConstantBuffer2);
}

void Light_Finalize(void)
{
    SAFE_RELEASE(g_pPSConstantBuffer1);
    SAFE_RELEASE(g_pPSConstantBuffer2);
}

void Light_SetAmbient(const DirectX::XMFLOAT3& color)
{
    AmbientLightBuffer data = { color };
    g_pContext->UpdateSubresource(g_pPSConstantBuffer1, 0, nullptr, &data, 0, 0);
    g_pContext->PSSetConstantBuffers(1, 1, &g_pPSConstantBuffer1);
}

void Light_SetDirectional(
    const DirectX::XMFLOAT4& directional,
    const DirectX::XMFLOAT3& color,
    const DirectX::XMFLOAT3& camera_position
    )
{
    DirectionalLightBuffer data = { directional, color, 0.0f, camera_position};
    g_pContext->UpdateSubresource(g_pPSConstantBuffer2, 0, nullptr, &data, 0, 0);
    g_pContext->PSSetConstantBuffers(2, 1, &g_pPSConstantBuffer2);
}

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
static ID3D11Buffer* g_pPSConstantBuffer3 = nullptr; // Phong Light
static ID3D11Buffer* g_pPSConstantBuffer4 = nullptr; // Point Light

struct AmbientLightBuffer
{
    XMFLOAT3 color;
    float pad;
};

struct DirectionalLightBuffer
{
    XMFLOAT4 world_vector;
    XMFLOAT3 color;
    float padding;
};

struct SpecularLightBuffer
{
    XMFLOAT3 eye_posW;
    float specular_power;
    XMFLOAT3 specular_light_color;
    float padding;
};

struct PointLight 
{
    XMFLOAT3 LightPosition;
    float Range;
    XMFLOAT4 Color;
    //float SpecularPower;
    //XMFLOAT3 SpecularColor;
};

struct PointLightList 
{
    PointLight light[4];
    int count;
    XMFLOAT3 dummy;
};

static PointLightList g_PointLights{};

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
    buffer_desc.ByteWidth = sizeof(SpecularLightBuffer); // バッファのサイズ
    g_pDevice->CreateBuffer(&buffer_desc, nullptr, &g_pPSConstantBuffer3);
    buffer_desc.ByteWidth = sizeof(PointLightList); // バッファのサイズ
    g_pDevice->CreateBuffer(&buffer_desc, nullptr, &g_pPSConstantBuffer4);
}

void Light_Finalize(void)
{
    SAFE_RELEASE(g_pPSConstantBuffer1);
    SAFE_RELEASE(g_pPSConstantBuffer2);
    SAFE_RELEASE(g_pPSConstantBuffer3);
    SAFE_RELEASE(g_pPSConstantBuffer4);
}

void Light_SetAmbient(const DirectX::XMFLOAT3& color)
{
    AmbientLightBuffer data = { color };
    g_pContext->UpdateSubresource(g_pPSConstantBuffer1, 0, nullptr, &data, 0, 0);
    g_pContext->PSSetConstantBuffers(1, 1, &g_pPSConstantBuffer1);
}

void Light_SetDirectional(
    const DirectX::XMFLOAT4& directional,
    const DirectX::XMFLOAT3& color
    )
{
    DirectionalLightBuffer data = { directional, color };
    g_pContext->UpdateSubresource(g_pPSConstantBuffer2, 0, nullptr, &data, 0, 0);
    g_pContext->PSSetConstantBuffers(2, 1, &g_pPSConstantBuffer2);
}

void Light_SetSpecular(
    const DirectX::XMFLOAT3& camera_position,
    float specular_power,
    const DirectX::XMFLOAT3& color
    )
{
    SpecularLightBuffer data = { camera_position, specular_power, color };
    g_pContext->UpdateSubresource(g_pPSConstantBuffer3, 0, nullptr, &data, 0, 0);
    g_pContext->PSSetConstantBuffers(3, 1, &g_pPSConstantBuffer3);
}

void Light_SetPointCount(int count)
{
    g_PointLights.count = count;

    g_pContext->UpdateSubresource(g_pPSConstantBuffer4, 0, nullptr, &g_PointLights, 0, 0);
    g_pContext->PSSetConstantBuffers(4, 1, &g_pPSConstantBuffer4);
}

void Light_SetPointLight(int n, const DirectX::XMFLOAT3& position, float range, const DirectX::XMFLOAT3& color)
{
    g_PointLights.light[n].LightPosition = position;
    g_PointLights.light[n].Range = range;
    g_PointLights.light[n].Color = { color.x,color.y,color.z,1.0f };

    g_pContext->UpdateSubresource(g_pPSConstantBuffer4, 0, nullptr, &g_PointLights, 0, 0);
    g_pContext->PSSetConstantBuffers(4, 1, &g_pPSConstantBuffer4);
}
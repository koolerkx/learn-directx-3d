/**
 * @file cube.cpp
 * @brief 3Dキューブの表示
 * 動かない状態の３Dキューブ
 * 
 * @author KOOLER FAN
 * @date 2025/09/09
 */

#include "cube.h"
#include <DirectXMath.h>
#include "debug_ostream.h"
#include "direct3d.h"
#include "shader3d.h"

using namespace DirectX;

// 三角形二つ、六面
static constexpr int NUM_VERTEX = 3 * 2 * 6;

static ID3D11Buffer* g_pVertexBuffer = nullptr; // 頂点バッファ

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

// 頂点構造体
struct Vertex3d
{
    XMFLOAT3 position; // 頂点座標
    XMFLOAT4 color; // 色
};

static Vertex3d g_CubeVertex[36]
{
    // 前
    { { -0.5f, 0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1 } }, // LT
    { { 0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1 } }, // RB
    { { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1 } }, // LB
    { { -0.5f, 0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1 } }, // LT
    { { 0.5f, 0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1 } }, // RT
    { { 0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1 } }, // RB
};

void Cube_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    // デバイスとデバイスコンテキストのチェック
    if (!pDevice || !pContext)
    {
        hal::dout << "Polygon_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
        return;
    }

    g_pDevice = pDevice;
    g_pContext = pContext;

    // 頂点バッファ生成
    D3D11_BUFFER_DESC bd = {};
    // bd.Usage = D3D11_USAGE_DYNAMIC; // 書き換えて使えます
    bd.Usage = D3D11_USAGE_DEFAULT; // 変換行列があるため、実質動かなくていいです
    bd.ByteWidth = sizeof(Vertex3d) * NUM_VERTEX;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = g_CubeVertex;

    g_pDevice->CreateBuffer(&bd, &sd, &g_pVertexBuffer);
}

void Cube_Finalize()
{
    SAFE_RELEASE(g_pVertexBuffer);
}

void Cube_Draw()
{
    Shader3D_Begin();

    // 頂点バッファを描画パイプラインに設定
    UINT stride = sizeof(Vertex3d);
    UINT offset = 0;
    g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // 頂点シェーダーに変換行列を設定
    // パースペクティブ行列
    float fovAngleY = XMConvertToRadians(60.0f);
    float aspectRatio = static_cast<float>(Direct3D_GetBackBufferWidth()) / static_cast<float>(Direct3D_GetBackBufferWidth());
    float nearZ = 0.1f;
    float farZ = 10000.0f;

    XMMATRIX mtxPerspective = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
    Shader3D_SetProjectionMatrix(mtxPerspective);

    // ワールド座標変換行列
    XMMATRIX mtxWorld = XMMatrixIdentity();
    Shader3D_SetWorldMatrix(mtxWorld);

    // ビュー座標変換行列
    constexpr FXMVECTOR cameraPosition = { 2.0f, 2.0f, -5.0f };
    constexpr FXMVECTOR cameraFocusPosition = { 0.0f, 0.0f, 0.0f };
    constexpr FXMVECTOR cameraUpDirection = { 0.0f, 1.0f, 0.0f };

    XMMATRIX mtxView = XMMatrixLookAtLH(cameraPosition, cameraFocusPosition, cameraUpDirection);
    // Shader_SetViewMatrix(mtxView);

    // Shader_SetColor();

    // プリミティブトポロジ設定
    g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // ポリゴン描画命令発行
    // g_pContext->Draw(NUM_VERTEX, 0);
    g_pContext->Draw(6, 0);
}

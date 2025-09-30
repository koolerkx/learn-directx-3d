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
#include "color.h"
#include "texture.h"

using namespace DirectX;

// 三角形二つ、六面
static constexpr int NUM_VERTEX = 24;
static constexpr int NUM_INDEX = 36;

static ID3D11Buffer* g_pVertexBuffer = nullptr; // 頂点バッファ
static ID3D11Buffer* g_pIndexBuffer = nullptr; // インデックスバッファ

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

static int g_CubeTexId = -1;
static std::wstring TEXTURE_PATH = L"assets/cube_test_2048.png";

// 頂点構造体
struct Vertex3d
{
    XMFLOAT3 position; // 頂点座標
    XMFLOAT3 normal;
    XMFLOAT4 color; // 色
    XMFLOAT2 uv;
};

static Vertex3d g_CubeVertex[NUM_VERTEX]
{
    // 前
    { { -0.5f, +0.5f, -0.5f }, { 0, 0, -1 }, Color::WHITE, { 0, 0 } }, // LT
    { { +0.5f, -0.5f, -0.5f }, { 0, 0, -1 }, Color::WHITE, { 0.25f, 0.25f } }, // RB
    { { -0.5f, -0.5f, -0.5f }, { 0, 0, -1 }, Color::WHITE, { 0, 0.25f } }, // LB
    { { +0.5f, +0.5f, -0.5f }, { 0, 0, -1 }, Color::WHITE, { 0.25f, 0 } }, // RT

    // 後
    { { +0.5f, -0.5f, +0.5f }, { 0, 0, 1 }, Color::WHITE, { 0.25f, 0 } }, // LT
    { { +0.5f, +0.5f, +0.5f }, { 0, 0, 1 }, Color::WHITE, { 0.25f, 0.25f } }, // LB
    { { -0.5f, +0.5f, +0.5f }, { 0, 0, 1 }, Color::WHITE, { 0.5f, 0.25f } }, // RB
    { { -0.5f, -0.5f, +0.5f }, { 0, 0, 1 }, Color::WHITE, { 0.5f, 0 } }, // RT

    // 上
    { { -0.5f, +0.5f, +0.5f }, { 0, 1, 0 }, Color::WHITE, { 0.5f, 0 } }, // LT
    { { +0.5f, +0.5f, -0.5f }, { 0, 1, 0 }, Color::WHITE, { 0.75f, 0.25f } }, // RB
    { { -0.5f, +0.5f, -0.5f }, { 0, 1, 0 }, Color::WHITE, { 0.5f, 0.25f } }, // LB
    { { +0.5f, +0.5f, +0.5f }, { 0, 1, 0 }, Color::WHITE, { 0.75f, 0 } }, // RT

    // 下
    { { +0.5f, -0.5f, -0.5f }, { 0, -1, 0 }, Color::WHITE, { 0.75f, 0 } }, // LT
    { { +0.5f, -0.5f, +0.5f }, { 0, -1, 0 }, Color::WHITE, { 0.75f, 0.25f } }, // LB
    { { -0.5f, -0.5f, +0.5f }, { 0, -1, 0 }, Color::WHITE, { 1.0f, 0.25f } }, // RB
    { { -0.5f, -0.5f, -0.5f }, { 0, -1, 0 }, Color::WHITE, { 1.0f, 0 } }, // RT

    // 右
    { { +0.5f, +0.5f, -0.5f }, { 1, 0, 0 }, Color::WHITE, { 0, 0.25f } }, // LT
    { { +0.5f, -0.5f, +0.5f }, { 1, 0, 0 }, Color::WHITE, { 0.25f, 0.5f } }, // RB
    { { +0.5f, -0.5f, -0.5f }, { 1, 0, 0 }, Color::WHITE, { 0, 0.5f } }, // LB
    { { +0.5f, +0.5f, +0.5f }, { 1, 0, 0 }, Color::WHITE, { 0.25f, 0.25f } }, // RT

    // 左
    { { -0.5f, -0.5f, +0.5f }, { -1, 0, 0 }, Color::WHITE, { 0.25f, 0.25f } }, // LT
    { { -0.5f, +0.5f, +0.5f }, { -1, 0, 0 }, Color::WHITE, { 0.25f, 0.5f } }, // LB
    { { -0.5f, +0.5f, -0.5f }, { -1, 0, 0 }, Color::WHITE, { 0.5f, 0.5f } }, // RB
    { { -0.5f, -0.5f, -0.5f }, { -1, 0, 0 }, Color::WHITE, { 0.5f, 0.25f } }, // RT
};

static uint16_t g_CubeIndex[NUM_INDEX] = {
    0, 1, 2, 0, 3, 1,
    4, 5, 6, 4, 6, 7,
    8, 9, 10, 8, 11, 9,
    12, 13, 14, 12, 14, 15,
    16, 17, 18, 16, 19, 17,
    20, 21, 22, 20, 22, 23
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
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = g_CubeVertex;

    g_pDevice->CreateBuffer(&bd, &sd, &g_pVertexBuffer);

    bd.ByteWidth = sizeof(uint16_t) * NUM_INDEX;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    sd.pSysMem = g_CubeIndex;

    g_pDevice->CreateBuffer(&bd, &sd, &g_pIndexBuffer);

    g_CubeTexId = Texture_Load(TEXTURE_PATH.c_str());
}

void Cube_Finalize()
{
    SAFE_RELEASE(g_pVertexBuffer);
    SAFE_RELEASE(g_pIndexBuffer);
}

void Cube_Update(double)
{}

void Cube_Draw(const DirectX::XMMATRIX& mtxWorld)
{
    Shader3D_Begin();
    Direct3D_DepthStencilStateDepthIsEnable(true);

    // 頂点バッファを描画パイプラインに設定
    UINT stride = sizeof(Vertex3d);
    UINT offset = 0;
    g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
    g_pContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // 頂点シェーダーに変換行列を設定
    // ワールド座標変換行列
    Shader3D_SetWorldMatrix(mtxWorld);

    // プリミティブトポロジ設定
    g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // ポリゴン描画命令発行
    Texture_SetTexture(g_CubeTexId);
    g_pContext->DrawIndexed(36, 0, 0);

    Direct3D_DepthStencilStateDepthIsEnable(false);
}

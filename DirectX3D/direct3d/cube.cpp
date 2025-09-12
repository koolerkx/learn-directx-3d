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
    { { -0.5f, +0.5f, -0.5f }, Color::RED }, // LT
    { { +0.5f, -0.5f, -0.5f }, Color::RED }, // RB
    { { -0.5f, -0.5f, -0.5f }, Color::RED }, // LB
    { { -0.5f, +0.5f, -0.5f }, Color::RED }, // LT
    { { +0.5f, +0.5f, -0.5f }, Color::RED }, // RT
    { { +0.5f, -0.5f, -0.5f }, Color::RED }, // RB

    // 後
    { { +0.5f, -0.5f, +0.5f }, Color::BLUE }, // LT
    { { +0.5f, +0.5f, +0.5f }, Color::BLUE }, // LB
    { { -0.5f, +0.5f, +0.5f }, Color::BLUE }, // RB
    { { +0.5f, -0.5f, +0.5f }, Color::BLUE }, // LT
    { { -0.5f, +0.5f, +0.5f }, Color::BLUE }, // RB
    { { -0.5f, -0.5f, +0.5f }, Color::BLUE }, // RT

    // 上
    { { -0.5f, +0.5f, +0.5f }, Color::GREEN }, // LT
    { { +0.5f, +0.5f, -0.5f }, Color::GREEN }, // RB
    { { -0.5f, +0.5f, -0.5f }, Color::GREEN }, // LB
    { { -0.5f, +0.5f, +0.5f }, Color::GREEN }, // LT
    { { +0.5f, +0.5f, +0.5f }, Color::GREEN }, // RT
    { { +0.5f, +0.5f, -0.5f }, Color::GREEN }, // RB

    // 下
    { { +0.5f, -0.5f, -0.5f }, Color::YELLOW }, // LT
    { { +0.5f, -0.5f, +0.5f }, Color::YELLOW }, // LB
    { { -0.5f, -0.5f, +0.5f }, Color::YELLOW }, // RB
    { { +0.5f, -0.5f, -0.5f }, Color::YELLOW }, // LT
    { { -0.5f, -0.5f, +0.5f }, Color::YELLOW }, // RB
    { { -0.5f, -0.5f, -0.5f }, Color::YELLOW }, // RT

    // 右
    { { +0.5f, +0.5f, -0.5f }, Color::AQUA }, // LT
    { { +0.5f, -0.5f, +0.5f }, Color::AQUA }, // RB
    { { +0.5f, -0.5f, -0.5f }, Color::AQUA }, // LB
    { { +0.5f, +0.5f, -0.5f }, Color::AQUA }, // LT
    { { +0.5f, +0.5f, +0.5f }, Color::AQUA }, // RT
    { { +0.5f, -0.5f, +0.5f }, Color::AQUA }, // RB

    // 左
    { { -0.5f, -0.5f, +0.5f }, Color::ORANGE }, // LT
    { { -0.5f, +0.5f, +0.5f }, Color::ORANGE }, // LB
    { { -0.5f, +0.5f, -0.5f }, Color::ORANGE }, // RB
    { { -0.5f, -0.5f, +0.5f }, Color::ORANGE }, // LT
    { { -0.5f, +0.5f, -0.5f }, Color::ORANGE }, // RB
    { { -0.5f, -0.5f, -0.5f }, Color::ORANGE }, // RT
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
}

void Cube_Finalize()
{
    SAFE_RELEASE(g_pVertexBuffer);
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

    // 頂点シェーダーに変換行列を設定
    // ワールド座標変換行列
    Shader3D_SetWorldMatrix(mtxWorld);

    // プリミティブトポロジ設定
    g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // ポリゴン描画命令発行
    // g_pContext->Draw(NUM_VERTEX, 0);
    g_pContext->Draw(36, 0);

    Direct3D_DepthStencilStateDepthIsEnable(false);
}

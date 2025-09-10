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

static constexpr float FACE_OPACITY = 0.6f; // TEMP

static Vertex3d g_CubeVertex[36]
{
    // 前
    { { -0.5f, +0.5f, -0.5f }, Color::SetOpacity(Color::RED, FACE_OPACITY) }, // LT
    { { +0.5f, -0.5f, -0.5f }, Color::SetOpacity(Color::RED, FACE_OPACITY) }, // RB
    { { -0.5f, -0.5f, -0.5f }, Color::SetOpacity(Color::RED, FACE_OPACITY) }, // LB
    { { -0.5f, +0.5f, -0.5f }, Color::SetOpacity(Color::RED, FACE_OPACITY) }, // LT
    { { +0.5f, +0.5f, -0.5f }, Color::SetOpacity(Color::RED, FACE_OPACITY) }, // RT
    { { +0.5f, -0.5f, -0.5f }, Color::SetOpacity(Color::RED, FACE_OPACITY) }, // RB
    
    // 後
    { { +0.5f, -0.5f, +0.5f }, Color::SetOpacity(Color::BLUE, FACE_OPACITY) }, // LT
    { { -0.5f, +0.5f, +0.5f }, Color::SetOpacity(Color::BLUE, FACE_OPACITY) }, // RB
    { { +0.5f, +0.5f, +0.5f }, Color::SetOpacity(Color::BLUE, FACE_OPACITY) }, // LB
    { { +0.5f, -0.5f, +0.5f }, Color::SetOpacity(Color::BLUE, FACE_OPACITY) }, // LT
    { { -0.5f, -0.5f, +0.5f }, Color::SetOpacity(Color::BLUE, FACE_OPACITY) }, // RT
    { { -0.5f, +0.5f, +0.5f }, Color::SetOpacity(Color::BLUE, FACE_OPACITY) }, // RB
    
    // 上
    { { -0.5f, +0.5f, +0.5f }, Color::SetOpacity(Color::GREEN, FACE_OPACITY) }, // LT
    { { +0.5f, +0.5f, -0.5f }, Color::SetOpacity(Color::GREEN, FACE_OPACITY) }, // RB
    { { -0.5f, +0.5f, -0.5f }, Color::SetOpacity(Color::GREEN, FACE_OPACITY) }, // LB
    { { -0.5f, +0.5f, +0.5f }, Color::SetOpacity(Color::GREEN, FACE_OPACITY) }, // LT
    { { +0.5f, +0.5f, +0.5f }, Color::SetOpacity(Color::GREEN, FACE_OPACITY) }, // RT
    { { +0.5f, +0.5f, -0.5f }, Color::SetOpacity(Color::GREEN, FACE_OPACITY) }, // RB
    
    // 下
    { { +0.5f, -0.5f, -0.5f }, Color::SetOpacity(Color::YELLOW, FACE_OPACITY) }, // LT
    { { -0.5f, -0.5f, +0.5f }, Color::SetOpacity(Color::YELLOW, FACE_OPACITY) }, // RB
    { { +0.5f, -0.5f, +0.5f }, Color::SetOpacity(Color::YELLOW, FACE_OPACITY) }, // LB
    { { +0.5f, -0.5f, -0.5f }, Color::SetOpacity(Color::YELLOW, FACE_OPACITY) }, // LT
    { { -0.5f, -0.5f, -0.5f }, Color::SetOpacity(Color::YELLOW, FACE_OPACITY) }, // RT
    { { -0.5f, -0.5f, +0.5f }, Color::SetOpacity(Color::YELLOW, FACE_OPACITY) }, // RB

    // 右
    { { +0.5f, +0.5f, -0.5f }, Color::SetOpacity(Color::AQUA, FACE_OPACITY) }, // LT
    { { +0.5f, -0.5f, +0.5f }, Color::SetOpacity(Color::AQUA, FACE_OPACITY) }, // RB
    { { +0.5f, -0.5f, -0.5f }, Color::SetOpacity(Color::AQUA, FACE_OPACITY) }, // LB
    { { +0.5f, +0.5f, -0.5f }, Color::SetOpacity(Color::AQUA, FACE_OPACITY) }, // LT
    { { +0.5f, +0.5f, +0.5f }, Color::SetOpacity(Color::AQUA, FACE_OPACITY) }, // RT
    { { +0.5f, -0.5f, +0.5f }, Color::SetOpacity(Color::AQUA, FACE_OPACITY) }, // RB

    // 左
    { { -0.5f, -0.5f, +0.5f }, Color::SetOpacity(Color::ORANGE, FACE_OPACITY) }, // LT
    { { -0.5f, +0.5f, -0.5f }, Color::SetOpacity(Color::ORANGE, FACE_OPACITY) }, // RB
    { { -0.5f, +0.5f, +0.5f }, Color::SetOpacity(Color::ORANGE, FACE_OPACITY) }, // LB
    { { -0.5f, -0.5f, +0.5f }, Color::SetOpacity(Color::ORANGE, FACE_OPACITY) }, // LT
    { { -0.5f, -0.5f, -0.5f }, Color::SetOpacity(Color::ORANGE, FACE_OPACITY) }, // RT
    { { -0.5f, +0.5f, -0.5f }, Color::SetOpacity(Color::ORANGE, FACE_OPACITY) }, // RB
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

void Cube_Draw()
{
    Shader3D_Begin();
    Direct3D_DepthStencilStateDepthIsEnable(true);

    // 頂点バッファを描画パイプラインに設定
    UINT stride = sizeof(Vertex3d);
    UINT offset = 0;
    g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // 頂点シェーダーに変換行列を設定
    // パースペクティブ行列
    float fovAngleY = XMConvertToRadians(60.0f);
    float aspectRatio = static_cast<float>(Direct3D_GetBackBufferWidth()) / static_cast<float>(Direct3D_GetBackBufferHeight());
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
    Shader3D_SetViewMatrix(mtxView);

    // Shader_SetColor();

    // プリミティブトポロジ設定
    g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // ポリゴン描画命令発行
    // g_pContext->Draw(NUM_VERTEX, 0);
    g_pContext->Draw(36, 0);

    Direct3D_DepthStencilStateDepthIsEnable(false);
}

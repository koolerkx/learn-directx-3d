/**
 * @file mesh_field.cpp
 * @brief メッシュフィールドの表示
 * @author KOOLER FAN
 * @date 2025/09/18
 */

#include "mesh_field.h"

#include <DirectXMath.h>

#include "color.h"
#include "direct3d.h"
#include "shader3d.h"
#include "texture.h"
using namespace DirectX;

#include "debug_ostream.h"

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

static ID3D11Buffer* g_pVertexBuffer = nullptr; // 頂点バッファ
static ID3D11Buffer* g_pIndexBuffer = nullptr; // インデックスバッファ


static int g_CubeTexId = -1;
static std::wstring TEXTURE_PATH = L"assets/white.png";

// 頂点構造体
struct Vertex3d
{
    XMFLOAT3 position; // 頂点座標
    XMFLOAT4 color; // 色
    XMFLOAT2 uv;
};

void MeshField_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    // デバイスとデバイスコンテキストのチェック
    if (!pDevice || !pContext)
    {
        hal::dout << "Polygon_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
        return;
    }

    g_pDevice = pDevice;
    g_pContext = pContext;
    
    static Vertex3d g_CubeVertex;
    static uint16_t g_CubeIndex;

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

void MeshField_Finalize()
{
    SAFE_RELEASE(g_pVertexBuffer);
    SAFE_RELEASE(g_pIndexBuffer);
}

void MeshField_Update(double elapsed_time) {}

void MeshField_Draw(const DirectX::XMMATRIX& mtxWorld)
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

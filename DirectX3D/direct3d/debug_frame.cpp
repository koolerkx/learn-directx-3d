/**
 * @file debug_frame.cpp
 * 
 * @author KOOLER FAN
 * @date 2025/09/09
 */

#include "debug_frame.h"
#include <DirectXMath.h>
#include <vector>

#include "debug_ostream.h"
#include "direct3d.h"
#include "shader3d.h"
#include "color.h"
#include "texture.h"

using namespace DirectX;

static constexpr int NUM_VERTEX = 8;
static constexpr int NUM_INDEX  = 24;

static ID3D11Buffer* g_pVertexBuffer = nullptr;
static ID3D11Buffer* g_pIndexBuffer  = nullptr;

static ID3D11Device*        g_pDevice  = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

static int g_FrameTexId = -1;
static std::wstring TEXTURE_PATH = L"assets/white.png";

struct Vertex3d
{
    XMFLOAT3 position;
    XMFLOAT3 normal;
    XMFLOAT4 color;
    XMFLOAT2 uv;
};

static Vertex3d g_BoxVertex[NUM_VERTEX]
{
    {}, {}, {}, {}, {}, {}, {}, {}
};

static uint16_t g_BoxIndex[NUM_INDEX] = {
    0, 1,
    1, 2,
    2, 3,
    3, 0,
    4, 5,
    5, 6,
    6, 7,
    7, 4,
    0, 4,
    1, 5,
    2, 6,
    3, 7,
};

void DebugFrame_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    if (!pDevice || !pContext)
    {
        hal::dout << "DebugFrame_Initialize() : device/context invalid" << std::endl;
        return;
    }

    g_pDevice  = pDevice;
    g_pContext = pContext;

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(Vertex3d) * NUM_VERTEX;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = g_BoxVertex;

    g_pDevice->CreateBuffer(&bd, &sd, &g_pVertexBuffer);

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(uint16_t) * NUM_INDEX;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    sd.pSysMem = g_BoxIndex;

    g_pDevice->CreateBuffer(&bd, &sd, &g_pIndexBuffer);

    g_FrameTexId = Texture_Load(TEXTURE_PATH.c_str());
}

void DebugFrame_Finalize()
{
    SAFE_RELEASE(g_pVertexBuffer);
    SAFE_RELEASE(g_pIndexBuffer);
}

void DebugFrame_Update(double)
{}

void DebugFrame_AABB_Draw(const AABB& aabb)
{
    const XMFLOAT3& mn = aabb.min;
    const XMFLOAT3& mx = aabb.max;

    Vertex3d v[NUM_VERTEX];
    v[0].position = XMFLOAT3(mn.x, mx.y, mn.z);
    v[1].position = XMFLOAT3(mx.x, mx.y, mn.z);
    v[2].position = XMFLOAT3(mx.x, mx.y, mx.z);
    v[3].position = XMFLOAT3(mn.x, mx.y, mx.z);
    v[4].position = XMFLOAT3(mn.x, mn.y, mn.z);
    v[5].position = XMFLOAT3(mx.x, mn.y, mn.z);
    v[6].position = XMFLOAT3(mx.x, mn.y, mx.z);
    v[7].position = XMFLOAT3(mn.x, mn.y, mx.z);

    for (int i = 0; i < NUM_VERTEX; ++i)
    {
        v[i].normal = XMFLOAT3(0, 1, 0);
        v[i].color  = Color::WHITE;
        v[i].uv     = XMFLOAT2(0, 0);
    }

    D3D11_MAPPED_SUBRESOURCE mapped{};
    if (SUCCEEDED(g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
    {
        memcpy(mapped.pData, v, sizeof(v));
        g_pContext->Unmap(g_pVertexBuffer, 0);
    }

    Shader3D_Begin();
    Shader3D_SetMaterialColor(Color::SetOpacity(Color::WHITE, 1.0f));
    Direct3D_DepthStencilStateDepthIsEnable(true);

    UINT stride = sizeof(Vertex3d);
    UINT offset = 0;
    g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
    g_pContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    Shader3D_SetWorldMatrix(XMMatrixIdentity());

    Texture_SetTexture(g_FrameTexId);
    g_pContext->DrawIndexed(NUM_INDEX, 0, 0);

    Direct3D_DepthStencilStateDepthIsEnable(false);
}

/**
 * @file mesh_field.cpp
 * @brief メッシュフィールドの表示
 * 四角形のメッシュフィールド
 * 
 * @author KOOLER FAN
 * @date 2025/09/18
 */

#include "mesh_field.h"
#include <DirectXMath.h>
#include "debug_ostream.h"
#include "direct3d.h"
#include "shader3d.h"
#include "color.h"
#include "texture.h"
#include <vector>

#include "debug_imgui_mesh_field.h"
#include "sampler.h"

using namespace DirectX;

static constexpr int DEFAULT_X_COUNT = 10;
static constexpr int DEFAULT_Z_COUNT = 10;

static ID3D11Buffer* g_pVertexBuffer = nullptr; // 頂点バッファ
static ID3D11Buffer* g_pIndexBuffer = nullptr; // インデックスバッファ

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

static int g_CubeTexId = -1;
static std::wstring TEXTURE_PATH = L"assets/grass.jpg";

// 頂点構造体
struct Vertex3d
{
    XMFLOAT3 position; // 頂点座標
    XMFLOAT4 color; // 色
    XMFLOAT2 uv;
};

namespace
{
    bool g_is_display = true;
    
    float g_size_x = 1.0f;
    float g_size_z = 1.0f;

    int g_x_count = DEFAULT_X_COUNT;
    int g_z_count = DEFAULT_Z_COUNT;

    float g_pos_y = 0.0f;

    std::vector<Vertex3d> g_field_vertex;
    std::vector<uint16_t> g_field_index;

    std::vector<Vertex3d> MeshField_MakeVertex(int x_count, int z_count)
    {
        std::vector<Vertex3d> cube_vertex = {};
        cube_vertex.reserve((z_count + 1) * (x_count + 1));

        float offset_x = g_size_x * static_cast<float>(x_count) * 0.5f;
        float offset_z = g_size_z * static_cast<float>(z_count) * 0.5f;

        for (int z = 0; z < (z_count + 1); z++)
        {
            for (int x = 0; x < (x_count + 1); x++)
            {
                cube_vertex.emplace_back(Vertex3d{
                    {
                        static_cast<float>(x) * g_size_x - offset_x,
                        // g_pos_y + exp(static_cast<float>(x) / x_count * 1.5f) + exp(static_cast<float>(z) / z_count * 1.5f) - 2.0f,
                        g_pos_y,
                        static_cast<float>(z) * g_size_z - offset_z
                    },
                    Color::WHITE,
                    { static_cast<float>(x), static_cast<float>(z) }
                });
            }
        }

        return cube_vertex;
    }

    std::vector<uint16_t> MeshField_MakeIndex(int x_count, int z_count)
    {
        std::vector<uint16_t> cube_index = {};
        cube_index.reserve(x_count * z_count * 6);

        int vertex_x_count = x_count + 1;
        for (int i = 0; i < x_count * z_count; i++)
        {
            int x = i % x_count;
            int z = i / x_count;

            int LT = x + z * vertex_x_count;
            int RT = x + z * vertex_x_count + 1;
            int LB = LT + vertex_x_count;
            int RB = RT + vertex_x_count;

            // FIXME: Workaround for showing polygon with CCW front
            cube_index.push_back(LT);
            cube_index.push_back(LB);
            cube_index.push_back(RB);

            cube_index.push_back(LT);
            cube_index.push_back(RB);
            cube_index.push_back(RT);
        }

        return cube_index;
    }

    void MeshField_MakeIndexVertex(int x_count, int z_count)
    {
        g_field_vertex = MeshField_MakeVertex(x_count, z_count);
        g_field_index = MeshField_MakeIndex(x_count, z_count);
    }

    void MeshField_CreateBuffer()
    {
        // 頂点バッファ生成
        D3D11_BUFFER_DESC bd = {};
        // bd.Usage = D3D11_USAGE_DYNAMIC; // 書き換えて使えます
        bd.Usage = D3D11_USAGE_DEFAULT; // 変換行列があるため、実質動かなくていいです
        bd.ByteWidth = sizeof(Vertex3d) * g_field_vertex.size();
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA sd{};
        sd.pSysMem = g_field_vertex.data();

        g_pDevice->CreateBuffer(&bd, &sd, &g_pVertexBuffer);

        bd.ByteWidth = sizeof(uint16_t) * static_cast<int>(g_field_index.size());
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        sd.pSysMem = g_field_index.data();

        g_pDevice->CreateBuffer(&bd, &sd, &g_pIndexBuffer);
    }

    void MeshField_UpdateBuffer()
    {
        g_pContext->UpdateSubresource(
            g_pVertexBuffer,
            0,
            nullptr,
            g_field_vertex.data(),
            0, 0
            );

        g_pContext->UpdateSubresource(
            g_pIndexBuffer,
            0, nullptr,
            g_field_index.data(),
            0, 0
            );
    }
}

void MeshField_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    // デバイスとデバイスコンテキストのチェック
    if (!pDevice || !pContext)
    {
        hal::dout << "MeshField_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
        return;
    }

    g_pDevice = pDevice;
    g_pContext = pContext;

    MeshField_MakeIndexVertex(g_x_count, g_z_count);

    MeshField_CreateBuffer();

    g_CubeTexId = Texture_Load(TEXTURE_PATH.c_str());

    DebugImgui_MeshField_SetCallback(MeshFieldDataCallbackFn{
        MeshField_SetIsDisplay,
        MeshField_SetXSize,
        MeshField_SetZSize,
        MeshField_SetXCount,
        MeshField_SetZCount,
        MeshField_SetY
    });
}

void MeshField_Finalize()
{
    SAFE_RELEASE(g_pVertexBuffer);
    SAFE_RELEASE(g_pIndexBuffer);
}

void MeshField_Update(double)
{
    DebugImGui_UpdateMeshFieldData(MeshFieldData{
        g_is_display,
        g_size_x,
        g_size_z,
        g_x_count,
        g_z_count,
        g_pos_y
    });
}

void MeshField_Draw(const DirectX::XMMATRIX& mtxWorld)
{
    if (!g_is_display) return;
    
    Shader3D_Begin();
    Sampler_SetFilter(FILTER::POINT);
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
    g_pContext->DrawIndexed(g_field_index.size(), 0, 0);

    Direct3D_DepthStencilStateDepthIsEnable(false);
}

void MeshField_SetXCount(int x_count)
{
    if (x_count < 1)
        x_count = 1;
    g_x_count = x_count;
    MeshField_MakeIndexVertex(g_x_count, g_z_count);
    SAFE_RELEASE(g_pVertexBuffer);
    SAFE_RELEASE(g_pIndexBuffer);
    MeshField_CreateBuffer();
}

void MeshField_SetZCount(int z_count)
{
    if (z_count < 1)
        z_count = 1;
    g_z_count = z_count;
    MeshField_MakeIndexVertex(g_x_count, g_z_count);
    SAFE_RELEASE(g_pVertexBuffer);
    SAFE_RELEASE(g_pIndexBuffer);
    MeshField_CreateBuffer();
}

void MeshField_SetXSize(float x_size)
{
    if (x_size < 0.001f)
        x_size = 0.001f;
    g_size_x = x_size;
    MeshField_MakeIndexVertex(g_x_count, g_z_count);
    MeshField_UpdateBuffer();
}

void MeshField_SetZSize(float z_size)
{
    if (z_size < 0.001f)
        z_size = 0.001f;
    g_size_z = z_size;
    MeshField_MakeIndexVertex(g_x_count, g_z_count);
    MeshField_UpdateBuffer();
}

void MeshField_SetY(float y)
{
    g_pos_y = y;
    MeshField_MakeIndexVertex(g_x_count, g_z_count);
    MeshField_UpdateBuffer();
}

int MeshField_GetXCount()
{
    return g_x_count;
}

int MeshField_GetZCount()
{
    return g_z_count;
}

float MeshField_GetXSize()
{
    return g_size_x;
}

float MeshField_GetZSize()
{
    return g_size_z;
}

float MeshField_GetY()
{
    return g_pos_y;
}

void MeshField_SetIsDisplay(bool is_display)
{
    g_is_display = is_display;
}

bool MeshField_GetIsDisplay()
{
    return g_is_display;
}

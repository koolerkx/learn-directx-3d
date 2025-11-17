#include "billboard.h"
#include <DirectXMath.h>
#include "direct3d.h"
using namespace DirectX;
#include "player_camera.h"
#include "shader_billboard.h"
#include "texture.h"

static constexpr int NUM_VERTEX = 4; // 頂点

static ID3D11Buffer* g_pVertexBuffer = nullptr; // 頂点バッファ

// 頂点構造体
struct Vertex_Billboard
{
    XMFLOAT3 position; // 頂点座標
    XMFLOAT4 color; // 色
    XMFLOAT2 texcoord; // テクスチャーUV
};

void Billboard_Initialize()
{
    ShaderBillBoard_Initialize();

    static Vertex_Billboard vertex[]{
        { { -0.5f, 0.5f, 0.0f },
          { 1.0f, 1.0f, 1.0f, 1.0f },
          { 0.0f, 0.0f } }, // 左上
        { { 0.5f, 0.5f, 0.0f },
          { 1.0f, 1.0f, 1.0f, 1.0f },
          { 1.0f, 0.0f } }, // 右上
        { { -0.5f, -0.5f, 0.0f },
          { 1.0f, 1.0f, 1.0f, 1.0f },
          { 0.0f, 1.0f } }, // 左下
        { { 0.5f, -0.5f, 0.0f },
          { 1.0f, 1.0f, 1.0f, 1.0f },
          { 1.0f, 1.0f } }, // 右下
    };


    // 頂点バッファ生成
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(Vertex_Billboard) * NUM_VERTEX;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertex;

    Direct3D_GetDevice()->CreateBuffer(&bd, &sd, &g_pVertexBuffer);
}

void Billboard_Finalize(void)
{
    SAFE_RELEASE(g_pVertexBuffer); // 頂点バッファの解放
}

void Billboard_Draw(
    int texId, const DirectX::XMFLOAT3& position, float scale_x, float scale_y,
    const DirectX::XMFLOAT2& pivot
)
{
    ShaderBillBoard_SetUVParameter({ { 1.0f, 1.0f }, { 0.0f, 0.0f } });
    // シェーダーを描画パイプラインに設定
    ShaderBillBoard_Begin();

    ShaderBillBoard_SetMaterialColor({ 0.7f, 0.7f, 0.7f, 1.0f });

    // テクスチャの設定
    Texture_SetTexture(texId);


    // 頂点バッファを描画パイプラインに設定
    UINT stride = sizeof(Vertex_Billboard);
    UINT offset = 0;
    Direct3D_GetContext()->IASetVertexBuffers(
        0, 1, &g_pVertexBuffer, &stride, &offset
    );

    // プリミティブトポロジ設定
    Direct3D_GetContext()->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
    );

    // ワールド座標変換行列の作成


    // 頂点シェーダーにワールド座標変換行列を設定
    // 回転軸までのオフセット行列
    XMMATRIX pivot_offset = XMMatrixTranslation(-pivot.x, -pivot.y, 1.0f);
    // 　カメラ行列の回転だけ逆行列を作る
    XMFLOAT4X4 mtxCamera = PlayerCamera_GetViewMatrix();
    mtxCamera._41 = mtxCamera._42 = mtxCamera._43 = 0.0f;
    XMMATRIX iv = XMMatrixInverse(nullptr, XMLoadFloat4x4(&mtxCamera));
    // 正方直交行列の逆行列は転置行列と等しい


    XMMATRIX s = XMMatrixScaling(scale_x, scale_y, 1.0f);
    XMMATRIX t = XMMatrixTranslation(
        position.x + pivot.x, position.y + pivot.y, position.z
    );
    ShaderBillBoard_SetWorldMatrix(pivot_offset * s * iv * t);

    // ポリゴン描画命令発行
    Direct3D_GetContext()->Draw(NUM_VERTEX, 0);
}

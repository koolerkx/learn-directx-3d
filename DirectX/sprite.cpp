/**
 * @file sprite.h
 * @brief スプライト表示
 * @author KOOLER FAN
 * @date 2025/06/12
 */

#include <d3d11.h>
#include <DirectXMath.h>
#include "DirectXTex.h"
using namespace DirectX;
#include "direct3d.h"
#include "shader.h"
#include "debug_ostream.h"

static constexpr int NUM_VERTEX = 4; // 頂点数

static ID3D11Buffer* g_pVertexBuffer = nullptr; // 頂点バッファ
static ID3D11ShaderResourceView* g_pTexture = nullptr;	// テクスチャバッファ

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

// 頂点構造体
struct Vertex
{
	XMFLOAT3 position;	// 頂点座標
	XMFLOAT4 color;		// 色
	XMFLOAT2 uv;	// テクスチャー
};

void Sprite_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	// デバイスとデバイスコンテキストのチェック
	if (!pDevice || !pContext) {
		hal::dout << "Sprite_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
		return;
	}

	// デバイスとデバイスコンテキストの保存
	g_pDevice = pDevice;
	g_pContext = pContext;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	g_pDevice->CreateBuffer(&bd, NULL, &g_pVertexBuffer);

	// テクスチャの読み込み
	TexMetadata metadata;
	ScratchImage image;

	//LoadFromWICFile(L"knight.png", WIC_FLAGS_NONE, &metadata, image);
	LoadFromWICFile(L"knight_3.png", WIC_FLAGS_NONE, &metadata, image);
	HRESULT hr = CreateShaderResourceView(g_pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_pTexture);

	if (FAILED(hr)) {
		MessageBox(nullptr, "テクスチャの初期化に失敗しました", "エラー", MB_OK);
	}
}

void Sprite_Finalize(void)
{
	SAFE_RELEASE(g_pTexture);
	SAFE_RELEASE(g_pVertexBuffer);
}

void Sprite_Draw(float dx, float dy, float dw, float dh)
{
	// シェーダーを描画パイプラインに設定
	Shader_Begin();

	// 頂点バッファをロックする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点バッファへの仮想ポインタを取得
	Vertex* v = (Vertex*)msr.pData;

	// 頂点情報を書き込み
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();
	
	float w = 512;
	float h = 512;

	// 画面の左上から右下に向かう線分を描画する -> 時計回り
	v[0].position = { dx, dy, 0.0f };	// LT
	v[1].position = { dx + dw, dy, 0.0f };	// RT
	v[2].position = { dx, dy + dh, 0.0f };	// LB
	v[3].position = { dx + dw, dy + dh, 0.0f };	// RB

	v[0].color = { 1.0f, 0.0f, 0.0f, 1.0f };
	v[1].color = { 0.0f, 1.0f, 0.0f, 0.5f };
	v[2].color = { 0.0f, 0.0f, 1.0f, 0.5f };
	v[3].color = { 1.0f, 0.0f, 0.0f, 1.0f };

	v[0].uv = { 0.0f, 0.0f };
	v[1].uv = { 1.0f, 0.0f };
	v[2].uv = { 0.0f, 1.0f };
	v[3].uv = { 1.0f, 1.0f };

	// 頂点バッファのロックを解除
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// 頂点シェーダーに変換行列を設定
	Shader_SetMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

	// プリミティブトポロジ設定
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ設定
	g_pContext->PSSetShaderResources(0, 1, &g_pTexture);

	// ポリゴン描画命令発行
	g_pContext->Draw(NUM_VERTEX, 0);
}

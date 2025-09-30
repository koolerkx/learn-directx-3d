/**
 * @file shader_vertex_2d.hlsl
 * @brief 2D描画用頂点シェーダー
 * @author KOOLER FAN
 * @date 2025/06/10
 */

// 定数バッファ
cbuffer VS_CONSTANT_BUFFER: register(b0)
{
    float4x4 proj;
};

cbuffer VS_CONSTANT_BUFFER: register(b1)
{
    float4x4 world;
};

cbuffer VS_CONSTANT_BUFFER: register(b2)
{
    float4x4 view;
};

cbuffer VS_CONSTANT_BUFFER: register(b3)
{
    float3 ambient_color;
}

cbuffer VS_CONSTANT_BUFFER: register(b4)
{
    float4 directional_world_vector;
    float3 directional_color;
}

struct VS_INPUT
{
    float3 posL : POSITION0; // ローカル座標
    float3 normalL : NORMAL0;
    float4 color : COLOR0; // Color
    float2 uv: TEXCOORD0;
};
struct VS_OUTPUT
{
    float4 posH : SV_POSITION; // 変換済み座標
    float4 color : COLOR0; // Color
    float2 uv: TEXCOORD0;
};

// 頂点シェーダ
VS_OUTPUT main(VS_INPUT vs_in)
{
    VS_OUTPUT vs_out;

    // 座標変換
    float4 pos = float4(vs_in.posL, 1.0f);
    float4 posW = mul(pos, world);
    float4 posWV = mul(posW, view);
    float4 posH = mul(posWV, proj);
    vs_out.posH = posH;

    // ライト計算
    float4 normalW = normalize(mul(float4(vs_in.normalL, 0), world));
    float dl = max(dot(-directional_world_vector, normalW), 0);

    float3 color = vs_in.color.rgb * directional_color.rgb * dl +vs_in.color.rgb * ambient_color.rgb;
    vs_out.color = float4(color, vs_in.color.a);
    vs_out.uv = vs_in.uv;
    return vs_out;
}

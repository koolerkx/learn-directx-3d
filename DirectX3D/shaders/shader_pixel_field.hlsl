/**
 * @file shader_pixel_field.hlsl
 * @brief 3Dフィールド描画用ピクセルシェーダー
 * @author KOOLER FAN
 * @date 2025/06/10
 */

struct PS_INPUT
{
    float4 posH : SV_POSITION;
    float4 color : COLOR0;
    float2 uv: TEXCOORD0;
};

Texture2D tex; // テクスチャ
SamplerState samp; // テクスチャさんプラ

float4 main(PS_INPUT ps_in) : SV_TARGET
{
    // return tex.Sample(samp, ps_in.uv) * ps_in.color;

    // 反復しているので
    float2 uv = ps_in.uv;

    // 1. 違うスケールのものを合成
    // float2 uv0 = uv * 0.5f;
    // return tex.Sample(samp, uv0) * 0.5f + tex.Sample(samp, uv) * 0.5f; // * ps_in.color;

    // 2. 回転合成
    // 小さいほうを回転させて、大きいほうはそのまま
    float2 uv1;
    const float PI = acos(-1.0);
    const float A = 45.0f / 180.0f * PI;
    uv1.x = uv.x * cos(A) + uv.y * sin(A);
    uv1.y = - uv.x * sin(A) + uv.y * cos(A);
    return tex.Sample(samp, uv1) * 0.5f + tex.Sample(samp, uv) * 0.5f;
}

/**
 * @file shader_pixel_2d.hlsl
 * @brief 2D描画用ピクセルシェーダー
 * @author KOOLER FAN
 * @date 2025/06/10
 */

cbuffer PS_CONSTANT_BUFFER: register(b0)
{
    float4 color;
};

cbuffer PS_CONSTANT_BUFFER: register(b1)
{
    float3 ambient_color;
}

cbuffer PS_CONSTANT_BUFFER: register(b2)
{
    float4 directional_world_vector;
    float3 directional_color;
    float3 eye_powW;
    // float3 specular_light_color;
    // float specular_power;
}

struct PS_INPUT
{
    float4 posH : SV_POSITION;
    float4 posW : POSITION1; 
    float4 color : COLOR0;
    float2 uv: TEXCOORD0;
    float4 normalW: NORMAL1;
};

Texture2D tex; // テクスチャ
SamplerState samp; // テクスチャさんプラ

float4 main(PS_INPUT ps_in) : SV_TARGET
{
    // ライト
    float3 normalW = normalize(ps_in.normalW);
    float dl = max(dot(-directional_world_vector, normalW), 0);
    
    // スペキュラーライト
    float3 toEye = normalize(eye_powW - ps_in.posW.xyz);
    float3 r = reflect(directional_world_vector, normalW).xyz;
    float spec = pow(max(dot(r, toEye), 0.0f), 10.0f);

    float3 _light_color = ps_in.color.rgb * directional_color.rgb * dl + ps_in.color.rgb * ambient_color.rgb;
    _light_color += float3(1.0f, 1.0f, 1.0f) * spec;
    float4 light_color = float4(_light_color, 1.0f);
    
    return tex.Sample(samp, ps_in.uv) * color * light_color;
}

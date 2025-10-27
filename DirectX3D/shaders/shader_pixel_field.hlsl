/**
 * @file shader_pixel_field.hlsl
 * @brief 3Dフィールド描画用ピクセルシェーダー
 * @author KOOLER FAN
 * @date 2025/06/10
 */

cbuffer PS_CONSTANT_BUFFER: register(b0)
{
    // float4 diffuse_color; // not in use
};

cbuffer PS_CONSTANT_BUFFER: register(b1)
{
    float3 ambient_color;
}

cbuffer PS_CONSTANT_BUFFER: register(b2)
{
    float4 directional_world_vector;
    float3 directional_color;
}

cbuffer PS_CONSTANT_BUFFER: register(b3)
{
    float3 eye_powW;
    float specular_power;
    float3 specular_light_color;
}

struct PS_INPUT
{
    float4 posH : SV_POSITION;
    float4 posW : POSITION1; 
    float4 blend : COLOR0;  // used to blend texture
    float2 uv: TEXCOORD0;
    float4 normalW : NORMAL1;
};

Texture2D tex0: register(t0); // テクスチャ
Texture2D tex1: register(t1); // テクスチャ
SamplerState samp; // テクスチャさんプラ

float4 main(PS_INPUT ps_in) : SV_TARGET
{
    // return tex1.Sample(samp, ps_in.uv) * ps_in.color;

    // テクスチャ合成
    // 反復しているので
    float2 uv = ps_in.uv;

    // 1. 違うスケールのものを合成
    // float2 uv0 = uv * 0.5f;
    // return tex0.Sample(samp, uv0) * 0.5f + tex0.Sample(samp, uv) * 0.5f; // * ps_in.color;

    // 2. 回転合成
    // 小さいほうを回転させて、大きいほうはそのまま
    // float2 uv1;
    // const float PI = acos(-1.0);
    // const float A = 45.0f / 180.0f * PI;
    // uv1.x = uv.x * cos(A) + uv.y * sin(A);
    // uv1.y = - uv.x * sin(A) + uv.y * cos(A);
    // return tex0.Sample(samp, uv1) * 0.5f + tex0.Sample(samp, uv) * 0.5f;

    // 3. 違うテクスチャ合成
    // return tex0.Sample(samp, uv) * 0.3f + tex1.Sample(samp, uv) * 0.7f; // * ps_in.color;

    float4 texColor = tex0.Sample(samp, uv) * ps_in.blend.r + tex1.Sample(samp, uv) * ps_in.blend.g; // * ps_in.color;

    // ライト計算
    float3 materialColor = texColor.rgb; // * diffuse_color.rgb;
    
    float3 normalW = normalize(ps_in.normalW).xyz;
    float dl = max(dot(-directional_world_vector.xyz, normalW), 0);
    
    float3 diffuse = materialColor * directional_color.rgb * dl;
    float3 ambient = materialColor * ambient_color.rgb;
    
    float3 toEye = normalize(ps_in.posW.xyz - eye_powW);
    float3 r = reflect(directional_world_vector.xyz, normalW).xyz;
    float t = pow(max(dot(r, toEye), 0.0f), specular_power);
    float3 specular = t * specular_light_color;

    float4 light_color = float4(ambient.rgb + diffuse.rgb + specular.rgb, 1.0f);

    return light_color;
}

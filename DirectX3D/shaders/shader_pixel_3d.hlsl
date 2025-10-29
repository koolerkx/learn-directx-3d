/**
 * @file shader_pixel_2d.hlsl
 * @brief 2D描画用ピクセルシェーダー
 * @author KOOLER FAN
 * @date 2025/06/10
 */

cbuffer PS_CONSTANT_BUFFER: register(b0)
{
    float4 diffuse_color;
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
    float4 color : COLOR0;
    float2 uv: TEXCOORD0;
    float4 normalW: NORMAL1;
};

Texture2D tex; // テクスチャ
SamplerState samp; // テクスチャさんプラ

float4 main(PS_INPUT ps_in) : SV_TARGET
{
    float4 texture_color = tex.Sample(samp, ps_in.uv);
    float3 material_color = texture_color.rgb * ps_in.color.rgb * diffuse_color.rgb;

    // ライト
    float3 normalW = normalize(ps_in.normalW);
    float dl = max(dot(-directional_world_vector, normalW), 0);
    float3 diffuse = material_color * directional_color.rgb * dl;
    float3 ambient = material_color * ambient_color.rgb;

    // スペキュラーライト
    float3 toEye = normalize(eye_powW - ps_in.posW.xyz);
    float3 r = reflect(directional_world_vector, normalW).xyz;
    float t = pow(max(dot(r, toEye), 0.0f), specular_power);
    float3 specular = t * specular_light_color;

    float alpha = texture_color.a * ps_in.color.a * diffuse_color.a;

    // lim Light
    float power1 = 1.0f - dl;
    float power2 = 1.0f - max(dot(toEye, normalW.xyz), 0.0f);
    float lim_power = power1 * power2;
    lim_power = pow(lim_power, 1.3f);
    float3 lim_light = lim_power * directional_color.rgb;

    float4 light_color = float4(ambient.rgb + diffuse.rgb + specular.rgb + lim_light.rgb, alpha);

    return light_color;
}

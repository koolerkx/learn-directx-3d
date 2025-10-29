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

struct PointLight
{
    float3 posW ;
    float range;
    float4 color;
};

cbuffer PS_CONSTANT_BUFFER : register(b4)
{
    PointLight point_light[4];
    int point_light_count;
    float3 point_light_dummy;
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

    // normalize inputs
    float3 normal = normalize(ps_in.normalW.xyz);
    float3 posW = ps_in.posW.xyz;
    float3 dirLight = normalize(directional_world_vector);

    // diffuse + ambient
    float dl = max(dot(-dirLight, normal), 0.0f);
    float3 diffuse = material_color * directional_color * dl;
    float3 ambient = material_color * ambient_color;

    // specular
    float3 toEye = normalize(eye_powW - posW);
    float3 r = reflect(dirLight, normal);
    float t = pow(max(dot(r, toEye), 0.0f), specular_power);
    float3 specular = t * specular_light_color;

    float alpha = texture_color.a * ps_in.color.a * diffuse_color.a;

    float3 color = ambient + diffuse + specular;
    
    float lim_power = 1.0f - max(dot(normal, toEye), 0.0f);
    float lim = pow(lim_power, 3.2f);
    color += float3(lim, lim, lim);
    
    for (int i = 0; i < point_light_count; i++)
    {
        float3 lightToPixel = posW - point_light[i].posW;
        
        float point_D = length(lightToPixel);
    
        float point_A = pow(max(1.0f - 1.0f / point_light[i].range * point_D, 0.0f), 2.0f);
        
        float point_dl = max(0.0f, dot(-normalize(lightToPixel), normal));
        
        color += material_color * point_light[i].color.rgb * point_A * point_dl;
        
        float3 point_r = reflect(normalize(lightToPixel), normal);
        float point_t = pow(max(dot(point_r, toEye), 0.0f), specular_power);
        
        color += point_light[i].color.rgb * point_t;
    }
    
    return float4(color, alpha);
}

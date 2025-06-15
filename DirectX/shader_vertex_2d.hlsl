/**
 * @file shader_vertex_2d.hlsl
 * @brief 2D描画用頂点シェーダー
 * @author KOOLER FAN
 * @date 2025/06/10
 */

// 定数バッファ
float4x4 mtx;


//=============================================================================
// 頂点シェーダ
//=============================================================================
float4 main(in float4 posL : POSITION0) : SV_POSITION
{
    return mul(posL, mtx);
}

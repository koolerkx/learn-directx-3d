/**
 * @file sampler.h
 * @brief サンプラーの設定ユーティリティー
 * @author KOOLER FAN
 * @date 2025/09/18
 */
#ifndef SAMPLER_H
#define SAMPLER_H

#include <cstdint>
#include <d3d11.h>

void Sampler_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Sampler_Finalize(void);

enum class FILTER: uint8_t
{
    POINT,
    LINEAR,
    ANISOTROPIC
};

void Sampler_SetFilter(FILTER filter);

#endif // SAMPLER_H

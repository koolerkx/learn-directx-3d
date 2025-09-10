/**
 * @file effect.cpp
 * @brief エフェクトとの描画
 * @author KOOLER FAN
 * @date 2025/07/04
 */

#ifndef EFFECT_H
#define EFFECT_H

#include "DirectXMath.h"

void Effect_Initialize();
void Effect_Finalize();

void Effect_Update(double elapsed_time);
void Effect_Draw();

void Effect_Create(const DirectX::XMFLOAT2& position);

#endif

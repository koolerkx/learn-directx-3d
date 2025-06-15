/**
 * @file sprite.h
 * @brief スプライト表示
 * @author KOOLER FAN
 * @date 2025/06/12
 */

#ifndef SPRITE_H
#define SPRITE_H

#include <d3d11.h>

void Sprite_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Sprite_Finalize(void);
void Sprite_Draw(
    float dx,	// drawX
    float dy	// drawY
);

#endif // SPRITE_H
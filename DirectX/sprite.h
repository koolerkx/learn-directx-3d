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

/**
* @param dx Draw X
* @param dy Draw Y
* @param dw Width
* @param dh Height
*/
void Sprite_Draw(
    float dx,
    float dy,
    float dw,
    float dh
);

#endif // SPRITE_H
/**
 * @file cube.h
 * @brief 3Dキューブの表示
 * @author KOOLER FAN
 * @date 2025/09/09
 */
#ifndef CUBE_H
#define CUBE_H

#include <d3d11.h>
#include <DirectXMath.h>

void Cube_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Cube_Finalize(void);
void Cube_Update(double elapsed_time);
void Cube_Draw(const DirectX::XMMATRIX& mtxWorld = DirectX::XMMatrixIdentity());

#endif // CUBE_H

/**
 * @file mesh_field.h
 * @brief メッシュフィールドの表示
 * @author KOOLER FAN
 * @date 2025/09/18
 */

#ifndef MESH_FIELD_H
#define MESH_FIELD_H

#include <d3d11.h>
#include <DirectXMath.h>

void MeshField_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void MeshField_Finalize(void);

void MeshField_Update(double elapsed_time);
void MeshField_Draw(const DirectX::XMMATRIX& mtxWorld = DirectX::XMMatrixIdentity());

#endif // MESH_FIELD_H

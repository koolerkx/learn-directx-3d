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

void MeshField_SetXCount(int x_count);
void MeshField_SetZCount(int z_count);
void MeshField_SetXSize(float x_size);
void MeshField_SetZSize(float z_size);
void MeshField_SetY(float y);

int MeshField_GetXCount();
int MeshField_GetZCount();
float MeshField_GetXSize();
float MeshField_GetZSize();
float MeshField_GetY();

#endif // MESH_FIELD_H

/**
 * @file shader_field.h
 * @brief シェーダー
 * @author KOOLER FAN
 * @date 2025/09/26
 */

#ifndef SHADER_PIXEL_FIELD_H
#define	SHADER_PIXEL_FIELD_H

#include <d3d11.h>
#include <DirectXMath.h>

bool Shader_Field_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Shader_Field_Finalize();

void Shader_Field_SetProjectionMatrix(const DirectX::XMMATRIX& matrix);
void Shader_Field_SetWorldMatrix(const DirectX::XMMATRIX& matrix);
void Shader_Field_SetViewMatrix(const DirectX::XMMATRIX& matrix);

void Shader_Field_Begin();

#endif // SHADER_PIXEL_FIELD_H


#ifndef SHADER_BILLBOARD_H
#define SHADER_BILLBOARD_H

#include <DirectXMath.h>

struct UVParameter
{
    DirectX::XMFLOAT2 scale;
    DirectX::XMFLOAT2 translation;
};

bool ShaderBillBoard_Initialize();
void ShaderBillBoard_Finalize();

void ShaderBillBoard_SetWorldMatrix(const DirectX::XMMATRIX& matrix);
void ShaderBillBoard_SetViewMatrix(const DirectX::XMMATRIX& matrix);
void ShaderBillBoard_SetProjectionMatrix(const DirectX::XMMATRIX& matrix);

void ShaderBillBoard_SetMaterialColor(const DirectX::XMFLOAT4& material_color);

void ShaderBillBoard_SetUVParameter(const UVParameter& parameter);


void ShaderBillBoard_Begin();

#endif // SHADER_BILLBOARD_H

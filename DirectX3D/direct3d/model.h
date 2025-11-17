#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/matrix4x4.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "collision.h"
#pragma comment(lib, "assimp-vc143-mt.lib")

struct MODEL
{
    const aiScene* AiScene = nullptr;

    ID3D11Buffer** VertexBuffer;
    ID3D11Buffer** IndexBuffer;

    std::unordered_map<std::string, ID3D11ShaderResourceView*> Texture;

    AABB local_aabb;
};


MODEL*
ModelLoad(const char* FileName, float scale = 1.0f, bool bBlender = false);
void ModelRelease(MODEL* model);

void ModelDraw(MODEL* pModel, const DirectX::XMMATRIX& mtxWorld);
AABB Model_GetAABB(MODEL* model, const DirectX::XMFLOAT3& position);

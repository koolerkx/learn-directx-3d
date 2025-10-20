#include "direct3d.h"
#include "WICTextureLoader11.h"
#include "texture.h"
#include "model.h"

#include <DirectXMath.h>
#include <cassert>

#include "debug_ostream.h"
#include "shader3d.h"

using namespace DirectX;

int g_default_texture_id = -1;

// 頂点構造体
struct Vertex
{
    XMFLOAT3 position; // 頂点座標
    XMFLOAT3 normal;
    XMFLOAT4 color; // 色
    XMFLOAT2 uv;
};

MODEL* ModelLoad(const char* FileName, float scale, bool bBlender)
{
    MODEL* model = new MODEL;

    const std::string modelPath(FileName);

    model->AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
    assert(model->AiScene);

    model->VertexBuffer = new ID3D11Buffer*[model->AiScene->mNumMeshes];
    model->IndexBuffer = new ID3D11Buffer*[model->AiScene->mNumMeshes];


    for (unsigned int m = 0; m < model->AiScene->mNumMeshes; m++)
    {
        aiMesh* mesh = model->AiScene->mMeshes[m];

        // 頂点バッファ生成
        {
            Vertex* vertex = new Vertex[mesh->mNumVertices];

            for (unsigned int v = 0; v < mesh->mNumVertices; v++)
            {
                if (bBlender)
                {
                    vertex[v].position = XMFLOAT3(mesh->mVertices[v].x * scale, -mesh->mVertices[v].z * scale, mesh->mVertices[v].y * scale);
                    vertex[v].normal = XMFLOAT3(mesh->mNormals[v].x, -mesh->mNormals[v].z, mesh->mNormals[v].y);
                }
                else
                {
                    vertex[v].position = XMFLOAT3(mesh->mVertices[v].x * scale, mesh->mVertices[v].y * scale, mesh->mVertices[v].z * scale);
                    vertex[v].normal = XMFLOAT3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
                }

                vertex[v].uv = XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
                vertex[v].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            }

            D3D11_BUFFER_DESC bd;
            ZeroMemory(&bd, sizeof(bd));
            bd.Usage = D3D11_USAGE_DEFAULT;
            bd.ByteWidth = sizeof(Vertex) * mesh->mNumVertices;
            bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bd.CPUAccessFlags = 0;

            D3D11_SUBRESOURCE_DATA sd;
            ZeroMemory(&sd, sizeof(sd));
            sd.pSysMem = vertex;

            Direct3D_GetDevice()->CreateBuffer(&bd, &sd, &model->VertexBuffer[m]);

            delete[] vertex;
        }

        // インデックスバッファ生成
        {
            unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

            for (unsigned int f = 0; f < mesh->mNumFaces; f++)
            {
                const aiFace* face = &mesh->mFaces[f];

                assert(face->mNumIndices == 3);

                index[f * 3 + 0] = face->mIndices[0];
                index[f * 3 + 1] = face->mIndices[1];
                index[f * 3 + 2] = face->mIndices[2];
            }

            D3D11_BUFFER_DESC bd;
            ZeroMemory(&bd, sizeof(bd));
            bd.Usage = D3D11_USAGE_DEFAULT;
            bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
            bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
            bd.CPUAccessFlags = 0;

            D3D11_SUBRESOURCE_DATA sd;
            ZeroMemory(&sd, sizeof(sd));
            sd.pSysMem = index;

            Direct3D_GetDevice()->CreateBuffer(&bd, &sd, &model->IndexBuffer[m]);

            delete[] index;
        }

    }


    //テクスチャ読み込み
    if (model->AiScene->mNumTextures)
    {
        for (int i = 0; i < model->AiScene->mNumTextures; i++)
        {
            aiTexture* aitexture = model->AiScene->mTextures[i];

            ID3D11Resource* pTexture = nullptr;
            ID3D11ShaderResourceView* pTextureSrv = nullptr;

            HRESULT texture_hr = CreateWICTextureFromMemory(
                Direct3D_GetDevice(),
                reinterpret_cast<const uint8_t*>(aitexture->pcData),
                aitexture->mWidth,
                &pTexture,
                &pTextureSrv
                );

            if (FAILED(texture_hr))
            {
                MessageBoxW(nullptr, L"モデルのテクスチャの読込に失敗しました", L"モデルテクスチャ読み込み失敗！", MB_OK | MB_ICONERROR);
                assert(false);
            }

            model->Texture[aitexture->mFilename.data] = pTextureSrv;
        }
    }
    else
    {
        g_default_texture_id = Texture_Load(L"assets/white.png");
    }

    return model;
}

void ModelRelease(MODEL* model)
{
    for (unsigned int m = 0; m < model->AiScene->mNumMeshes; m++)
    {
        model->VertexBuffer[m]->Release();
        model->IndexBuffer[m]->Release();
    }

    delete[] model->VertexBuffer;
    delete[] model->IndexBuffer;

    for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : model->Texture)
    {
        pair.second->Release();
    }

    aiReleaseImport(model->AiScene);

    delete model;
}

void ModelDraw(MODEL* pModel, const DirectX::XMMATRIX& mtxWorld)
{
    ID3D11DeviceContext* pContext = Direct3D_GetContext();

    Shader3D_Begin();

    Shader3D_SetWorldMatrix(mtxWorld);
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    Direct3D_DepthStencilStateDepthIsEnable(true);

    for (unsigned int m = 0; m < pModel->AiScene->mNumMeshes; m++)
    {
        aiString texture;
        aiMaterial* pMaterial = pModel->AiScene->mMaterials[pModel->AiScene->mMeshes[m]->mMaterialIndex];
        pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texture);

        if (texture != aiString(""))
        {
            pContext->PSSetShaderResources(0, 1, &pModel->Texture[texture.data]);
        }
        else
        {
            Texture_SetTexture(g_default_texture_id);
        }

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        pContext->IASetVertexBuffers(0, 1, &pModel->VertexBuffer[m], &stride, &offset);
        pContext->IASetIndexBuffer(pModel->IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

        pContext->DrawIndexed(pModel->AiScene->mMeshes[m]->mNumFaces * 3, 0, 0);
    }

    Direct3D_DepthStencilStateDepthIsEnable(false);
}

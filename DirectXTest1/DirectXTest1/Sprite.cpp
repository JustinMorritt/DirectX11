#include "Sprite.h"


Sprite::Sprite(float size)
{
	m_pVertexBuffer		= nullptr;
	m_pTexture			= nullptr;
	m_pShader			= nullptr;
	m_size				= size;
}


Sprite::~Sprite()
{
	Memory::SafeDelete(m_pVertexBuffer);
	
}

void Sprite::Initialize(ID3D11Device* device,  ID3D11DeviceContext* deviceContext, Shader* shader, LPCSTR textureFileName, bool isWritable)
{
	m_pShader = shader;
	m_pVertexBuffer = new VertexBuffer();
	m_pVertexBuffer->Initialize(device, shader, m_size, isWritable);

	if (textureFileName != NULL) //used if you dont want a texture
	{
		m_pTexture = new Texture();
		m_pTexture->Initialize(device, textureFileName);
	}
}

void Sprite::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	if (m_pTexture)
	{
		m_pShader->SetShaderParameters(deviceContext, m_pTexture->GetTexture());
		m_pShader->SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);

		m_pVertexBuffer->Render(deviceContext);
	}
}

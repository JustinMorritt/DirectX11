#include "ResourceShader.h"


ResourceShader::ResourceShader()
{
	m_pShader = nullptr;
}


ResourceShader::~ResourceShader()
{
	Memory::SafeDelete(m_pShader);
}

bool ResourceShader::Load(ID3D11Device* device, HWND hwnd, char* shaderFileName, LPCSTR vertexFunctionName, LPCSTR pixelFunctionName)
{
	m_pShader = new Shader(device, hwnd, shaderFileName, vertexFunctionName, pixelFunctionName);
	if (!m_pShader->IsInitialized())
	{
		Memory::SafeDelete(m_pShader);
		return false;
	}
	return true;
}

bool ResourceShader::Load(Shader* shader)
{
	if (shader == NULL) return false;

	m_pShader = shader;
	if (!m_pShader->IsInitialized())
	{
		Memory::SafeDelete(m_pShader);
		return false;
	}
	return true;
}

Shader* ResourceShader::GetShader()
{
	return m_pShader;
}

std::string ResourceShader::GetName()
{
	return m_pShader->GetName();
}

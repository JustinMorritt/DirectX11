#include "ResourceTexture.h"


ResourceTexture::ResourceTexture()
{
	m_pTexture = nullptr;
}


ResourceTexture::~ResourceTexture()
{
	Memory::SafeDelete(m_pTexture);
}

bool ResourceTexture::Load(ID3D11Device* device, char* textureFileName)
{
	m_pTexture = new Texture();
	if (!m_pTexture->Initialize(device, textureFileName))
	{
		Memory::SafeDelete(m_pTexture);
		return false;
	}
	return true;
}

Texture* ResourceTexture::GetTexture()
{
	return m_pTexture;
}

std::string ResourceTexture::GetName()
{
	return m_pTexture->GetName();
}

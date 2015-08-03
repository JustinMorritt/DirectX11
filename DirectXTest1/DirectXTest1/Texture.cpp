#include "Texture.h"


Texture::Texture()
{
	m_pTexture		= nullptr;
	m_Width			= 0;
	m_Height		= 0;
}


Texture::~Texture()
{
	Memory::SafeRelease(m_pTexture);
}

bool Texture::Initialize(ID3D11Device* device, LPCSTR fileName)
{
	HRESULT result;

	//load texture
	result = D3DX11CreateShaderResourceViewFromFile(device, fileName, NULL, NULL, &m_pTexture, NULL);
	if (FAILED(result))
	{
		return false;
	}

	ID3D11Resource* resource		= 0;
	ID3D11Texture2D* texture2D		= 0;

	m_pTexture->GetResource(&resource);


	result = resource->QueryInterface(&texture2D);
	if (SUCCEEDED(result))
	{
		D3D11_TEXTURE2D_DESC desc;
		texture2D->GetDesc(&desc);

		m_Width = desc.Width;
		m_Height = desc.Height;
	}
	Memory::SafeRelease(resource);
	Memory::SafeRelease(texture2D);

	return true;
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_pTexture;
}

int Texture::GetWidth()
{
	return m_Width;
}

int Texture::GetHeight()
{
	return m_Height;
}

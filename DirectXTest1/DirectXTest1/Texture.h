#ifndef _TEX_H
#define _TEX_H


#include "DXUtil.h"
#include <D3DX11tex.h>

class Texture
{
public:
	Texture();
	~Texture();

	bool Initialize(ID3D11Device* device, LPCSTR fileName);

	ID3D11ShaderResourceView* GetTexture();
	int GetWidth();
	int GetHeight();

private:
	ID3D11ShaderResourceView* m_pTexture;
	int m_Width;
	int m_Height;
};
#endif

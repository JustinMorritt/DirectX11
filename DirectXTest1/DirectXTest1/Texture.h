#ifndef _TEX_H
#define _TEX_H


#include "DXUtil.h"
#include <D3DX11tex.h>
#include <string>

using namespace std;

class Texture
{
public:
	Texture();
	~Texture();

	bool Initialize(ID3D11Device* device, LPCSTR fileName);

	ID3D11ShaderResourceView* GetTexture();
	int GetWidth();
	int GetHeight();
	string GetName();

private:
	ID3D11ShaderResourceView* m_pTexture;
	string m_name;

	int m_Width;
	int m_Height;
};
#endif

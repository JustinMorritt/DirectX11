#ifndef _RESOURCE_TEX_H_
#define _RESOURCE_TEX_H_

#include "Texture.h"


class ResourceTexture
{
public:
	ResourceTexture(void);
	~ResourceTexture(void);

	bool Load(ID3D11Device* device, char* textureFileName);

	Texture* GetTexture();
	string GetName();

private:
	Texture* m_pTexture;
};


#endif

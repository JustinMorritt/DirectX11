#ifndef _RMANAGER_H_
#define _RMANAGER_H_


#include "ResourceShader.h"
#include "ResourceTexture.h"

#include <iostream>
#include <vector>
using namespace std;


class ResourceManager
{
public:
	~ResourceManager(void);
	void LoadShaderResource(ID3D11Device* device, HWND hwnd, char* shaderFileName, LPCSTR vertexFunctionName, LPCSTR pixelFunctionName);
	void LoadShaderResource(Shader* shader);
	void LoadTextureResource(ID3D11Device* device, char* textureFileName);

	Shader* GetShaderByName(char* shaderName);
	Texture* GetTextureByName(char* textureName);

	static ResourceManager* GetInstance();


private:
	ResourceManager(void);
	
	vector<ResourceTexture*> m_Textures;
	vector<ResourceShader*> m_Shaders;

	static ResourceManager* m_pInstance;
};

#endif
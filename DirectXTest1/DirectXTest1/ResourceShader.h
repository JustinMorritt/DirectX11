#ifndef _RESOURCE_SHADER_H_
#define _RESOURCE_SHADER_H_

#include "Shader.h"

class ResourceShader
{
public:
	ResourceShader(void);
	~ResourceShader(void);

	bool Load(ID3D11Device* device, HWND hwnd, char* shaderFileName, LPCSTR vertexFunctionName, LPCSTR pixelFunctionName);
	bool Load(Shader* shader);

	Shader* GetShader();
	string GetName();

private: 

	Shader* m_pShader;

};

#endif
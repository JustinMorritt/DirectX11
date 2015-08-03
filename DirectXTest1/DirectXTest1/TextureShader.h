#ifndef _SHADERTEX_H_
#define _SHADERTEX_H_

#include "Shader.h"




class TextureShader :
	public Shader
{
public:
	TextureShader(ID3D11Device* device, HWND hwnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName);

	virtual ~TextureShader();

	void Begin(ID3D11DeviceContext* deviceContext, int indexCount) override;

	void End(ID3D11DeviceContext* deviceContext) override;

	bool Initialize(ID3D11Device* device, HWND hwnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixleFuncName) override;

private: 
	ID3D11SamplerState* m_pSamplerState;

};

#endif
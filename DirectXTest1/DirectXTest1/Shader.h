
#ifndef _SHADER_H_
#define _SHADER_H_

#include "DXUtil.h"
#include "D3DX11.h"
#include "D3DX10math.h"
#include "xnamath.h"
#include <fstream>
using namespace std;


class Shader
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX worldMatrix;
		D3DXMATRIX viewMatrix;
		D3DXMATRIX projectionMatrix;
	};
public:
	Shader(ID3D11Device* device, HWND hwnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName);
    virtual ~Shader();
	virtual void Begin(ID3D11DeviceContext* deviceContext, int indexCount);
	virtual void End(ID3D11DeviceContext* deviceContext);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture);
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix);

	char* GetName();
	bool IsInitialized();

protected:

	virtual bool Initialize(ID3D11Device* device, HWND hwnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixleFuncName);

private: 
	bool InitializeShader(ID3D11Device* device, HWND hwnd, LPCSTR vsFileName, LPCSTR psFileName, LPCSTR vertexFuncName, LPCSTR pixleFuncName);
	void OutPutShadeErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, LPCSTR shaderFileName);


	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pLayout;
	ID3D11Buffer*			m_pMatrixBuffer;

	char* m_pName;
	bool m_initialized;
};

#endif
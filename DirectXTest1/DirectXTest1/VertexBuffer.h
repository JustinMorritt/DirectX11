#ifndef _VERTEXBUF_H_
#define _VERTEXBUF_H_

#include "Shader.h"
#include <d3d11.h>
#include <D3DX10math.h>



class VertexBuffer
{
public:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 uv;
	};

	VertexBuffer();
	~VertexBuffer();

	bool Initialize(ID3D11Device* device, Shader* shader, float size, bool writeable = false);
	void Render(ID3D11DeviceContext* deviceContext);

	VertexType* GetVertices();
	ID3D11Buffer* GetVertexBuffer();
	int GetIndexCount();
	int GetVertexCount();

private:
	ID3D11Buffer*		m_pVertexBuffer;
	ID3D11Buffer*		m_pIndexBuffer;
	VertexType*			m_pVertices;
	Shader*				m_pShader;
	int					m_VertexCount;
	int					m_IndexCount;
};
#endif

#include "VertexBuffer.h"


VertexBuffer::VertexBuffer()
{
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer	= nullptr;
	m_pVertices		= nullptr;
	m_pShader		= nullptr;
}


VertexBuffer::~VertexBuffer()
{
	Memory::SafeRelease(m_pVertexBuffer);
	Memory::SafeRelease(m_pIndexBuffer);
	Memory::SafeDeleteArr(m_pVertices);
	Memory::SafeDelete(m_pShader);
}

bool VertexBuffer::Initialize(ID3D11Device* device, Shader* shader, float size, bool writeable /*= false*/)
{
	m_pShader = shader;

	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//Going to be a plane with 4 vertices
	m_VertexCount = 4;
	m_IndexCount = 6;

	m_pVertices		= new VertexType[m_VertexCount];
	indices			= new unsigned long[m_IndexCount];

	float halfSize = size / 2.0f;

	//load Vertex array
	m_pVertices[0].position = D3DXVECTOR3(-halfSize, -halfSize, 0.0f); //0 on the z
	m_pVertices[0].uv = D3DXVECTOR2(0.0f, 1.0f);
	
	m_pVertices[1].position = D3DXVECTOR3(-halfSize, halfSize, 0.0f); //0 on the z
	m_pVertices[1].uv = D3DXVECTOR2(0.0f, 0.0f);
	
	m_pVertices[2].position = D3DXVECTOR3(halfSize, halfSize, 0.0f); //0 on the z
	m_pVertices[2].uv = D3DXVECTOR2(1.0f, 0.0f);

	m_pVertices[3].position = D3DXVECTOR3(halfSize, -halfSize, 0.0f); //0 on the z
	m_pVertices[3].uv = D3DXVECTOR2(1.0f, 1.0f);

	//load indices 
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	//set up vertex Buffer Desc
	vertexBufferDesc.Usage					= (writeable) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth				= sizeof(VertexType) * m_VertexCount;
	vertexBufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags			= (writeable) ? D3D11_CPU_ACCESS_WRITE : D3D11_CPU_ACCESS_READ;
	vertexBufferDesc.MiscFlags				= 0;
	vertexBufferDesc.StructureByteStride	= 0;

	//Set up vertex data
	vertexData.pSysMem			= m_pVertices;
	vertexData.SysMemPitch		= 0;
	vertexData.SysMemSlicePitch = 0;

	//create vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Set up index buffer desc 
	indexBufferDesc.Usage				= D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth			= sizeof(unsigned long) * m_IndexCount;
	indexBufferDesc.BindFlags			= D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_READ;
	indexBufferDesc.StructureByteStride = 0;
	indexBufferDesc.MiscFlags			= 0;

	//set up index data
	indexData.pSysMem			= indices; 
	indexData.SysMemPitch		= 0;
	indexData.SysMemSlicePitch	= 0;

	//create index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	Memory::SafeDeleteArr(indices);
	
	return true;
}


void VertexBuffer::Render(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	m_pShader->Begin(deviceContext, m_IndexCount);

	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

VertexBuffer::VertexType* VertexBuffer::GetVertices()
{
	return m_pVertices;
}

ID3D11Buffer* VertexBuffer::GetVertexBuffer()
{
	return m_pVertexBuffer;
}

int VertexBuffer::GetIndexCount()
{
	return m_IndexCount;
}

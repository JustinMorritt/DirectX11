#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "VertexBuffer.h"
#include "Texture.h"

class Sprite
{
public:
	Sprite(float size);
	virtual ~Sprite();

	virtual void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader, LPCSTR textureFileName, bool isWritable = false);
	virtual void Update(){}; //For animated sprite
	virtual void Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

protected:
	VertexBuffer* m_pVertexBuffer;
	Texture* m_pTexture;
	Shader* m_pShader;
	float m_size; //size of the plane.
};
#endif

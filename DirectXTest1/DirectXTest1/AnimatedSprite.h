

#ifndef _ANIMSPRITE_H_
#define _ANIMSPRITE_H_

#include "Sprite.h"
class AnimatedSprite :
	public Sprite
{
public:
	AnimatedSprite(float size, float FPS, float animSpeed = 1.0f, bool isLooping = true);
	virtual ~AnimatedSprite();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader, LPCSTR textureFileName);

	void Update();

	void Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

private: 
	ID3D11DeviceContext* m_deviceContext;
	float m_CurrentFrame;
	float m_PreviousFrame;
	float m_MaxFrames;
	float m_AnimationSpeed;
	float m_CurrentSpeed;
	float m_FramesPerSecond;
	bool m_IsLooping;
};

#endif
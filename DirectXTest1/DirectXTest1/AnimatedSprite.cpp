#include "AnimatedSprite.h"
#include "GameTimer.h"


AnimatedSprite::AnimatedSprite(float size, float FPS, float animSpeed /*= 1.0f*/, bool isLooping /*= true*/) : Sprite(size)
{
	m_FramesPerSecond		= (1000.0f / FPS) / 1000.0f;
	m_AnimationSpeed		= animSpeed;
	m_IsLooping				= isLooping;
	m_CurrentSpeed			= 0;
}

AnimatedSprite::~AnimatedSprite()
{

}

void AnimatedSprite::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader, LPCSTR textureFileName)
{
	Sprite::Initialize(device, deviceContext, shader, textureFileName, true);
	m_deviceContext = deviceContext;
	m_CurrentFrame = 0;
	m_PreviousFrame = -1.0f;
	m_MaxFrames = (float)m_pTexture->GetWidth() / (float)m_pTexture->GetHeight(); // ONLY WORKS FOR HORIZONTAL SPRITE STRIPS ... Ill code the other style eventually
}

void AnimatedSprite::Update()
{
	if (m_MaxFrames == 1.0f){ return; }
	if (m_CurrentFrame < m_MaxFrames)
	{
		float dt = GameTimer::DeltaTime();
		m_CurrentSpeed += m_AnimationSpeed * dt;
		if (m_CurrentSpeed > m_FramesPerSecond)
		{
			m_CurrentFrame++;
			m_CurrentSpeed = 0.0f;

			//check if last frame 
			if (m_CurrentFrame >= m_MaxFrames)
			{
				if (m_IsLooping)
				{
					m_CurrentFrame = 0;
				}
				else
				{
					m_CurrentFrame = m_MaxFrames;
					//Probably set a boolean here flagging that animation is complete.
				}
			}
		}
	}
	// no reason to update buffer if its the same frame 
	if (m_CurrentFrame == m_PreviousFrame){ return; }

	//Big stuff then
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexBuffer::VertexType* vertices = m_pVertexBuffer->GetVertices();

	//update all our UV's
	vertices[0].uv.x = m_CurrentFrame / m_MaxFrames; // gets the normalized difference from 0 -> 1 in the texture 
	vertices[0].uv.y = 1.0f; //Bottom Left Point

	vertices[1].uv.x = m_CurrentFrame / m_MaxFrames; // gets the normalized difference from 0 -> 1 in the texture 
	vertices[1].uv.y = 0.0f; // Top Left Point

	vertices[2].uv.x = (m_CurrentFrame + 1.0f) / m_MaxFrames; // gets the normalized difference from 0 -> 1 in the texture 
	vertices[2].uv.y = 0.0f; // TopRight

	vertices[3].uv.x = (m_CurrentFrame + 1.0f) / m_MaxFrames; // gets the normalized difference from 0 -> 1 in the texture 
	vertices[3].uv.y = 1.0f; // BotRight

	//Lock the Vertex Buffer so it can be Written To 
	HRESULT result = m_deviceContext->Map(m_pVertexBuffer->GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return;
	}
	VertexBuffer::VertexType* verticesPtr = (VertexBuffer::VertexType*)mappedResource.pData; //What grabs the previous vetrices 
	memcpy(verticesPtr, (void*)vertices, sizeof(VertexBuffer::VertexType) * m_pVertexBuffer->GetVertexCount()); // TODO: Hardcoded 4 for the amout of vertices on this square..

	m_deviceContext->Unmap(m_pVertexBuffer->GetVertexBuffer(), 0);

	//update Previous frame
	m_PreviousFrame = m_CurrentFrame;
}

void AnimatedSprite::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	//Could enable Apha Blending here and ...
	Sprite::Render(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	//Disable Alpha Blending Here  .. Get it from the engine and call it true and false..
}

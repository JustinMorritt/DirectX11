#include "DXApp.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureShader.h"
#include "VertexBuffer.h"

class Engine : public DXApp
{
public:

	Engine(HINSTANCE hInstance);
	~Engine();

	//Override the methods
	bool Init() override;
	void Update(float dt) override;
	void Render(float dt) override;

private:
	//TEMP
	VertexBuffer* m_pVertexBuffer;
	TextureShader* m_pTextureShader;
	Texture* m_pTexture;

};




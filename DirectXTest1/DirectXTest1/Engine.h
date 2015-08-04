#include "DXApp.h"
#include "Sprite.h"
#include "TextureShader.h"

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
	Sprite* m_pSprite;
	TextureShader* m_pTextureShader;

};




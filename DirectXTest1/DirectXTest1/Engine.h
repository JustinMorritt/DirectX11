#include "DXApp.h"
#include "AnimatedSprite.h"
#include "ResourceManager.h"
#include "TextureShader.h"
#include "GameTimer.h"

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
	AnimatedSprite* m_pSprite;
	ResourceManager* m_pRManager;
};




//ENTRY POINT 

#include "DXApp.h"


class Graphics : public DXApp
{
	public:

	Graphics(HINSTANCE hInstance);
	~Graphics();

	//Override the methods
	bool Init() override;
	void Update(float dt) override;
	void Render(float dt) override;
};

Graphics::Graphics(HINSTANCE hInstance) : DXApp(hInstance)
{

}

Graphics::~Graphics()
{

}

bool Graphics::Init()
{
	return DXApp::Init();
	EnableAlphaBlending(true);
	EnableZBuffer(true);
}

void Graphics::Update(float dt)
{
	
}

void Graphics::Render(float dt)
{
	BeginScene(1.0f, 0.0f, 0.0f, 1.0f);
	EndScene();

	//m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::Lime);
	//HR(m_pSwapChain->Present(0, 0)); //flushes the buffer to our screen to our window
}






//Win 32 entry point ...   "handle to the application"			"optional old deprecated"    "command line arguments" "show code optional"                          
int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	//CREATE THE TEST APP
	Graphics App(hInstance);
	if (!App.Init())return 1;

	//MessageBox(NULL, "Hello, World", "Test", NULL);
	return App.Run();
}
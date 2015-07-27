//ENTRY POINT 

#include "DXApp.h"


class TestApp : public DXApp
{
	public:

	TestApp(HINSTANCE hInstance);
	~TestApp();

	//Override the methods
	bool Init() override;
	void Update(float dt) override;
	void Render(float dt) override;
};

TestApp::TestApp(HINSTANCE hInstance) : DXApp(hInstance)
{

}


TestApp::~TestApp()
{

}

bool TestApp::Init()
{
	return DXApp::Init();
}

void TestApp::Update(float dt)
{

}

void TestApp::Render(float dt)
{
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::DarkSalmon);

	HR(m_pSwapChain->Present(0, 0)); //flushes the buffer to our screen to our window
}

//Win 32 entry point ...   "handle to the application"			"optional old deprecated"    "command line arguments" "show code optional"                          
int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	
	//CREATE THE TEST APP
	TestApp tApp(hInstance);
	if (!tApp.Init())return 1;

	//MessageBox(NULL, "Hello, World", "Test", NULL);
	return tApp.Run();
}
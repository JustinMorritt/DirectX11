#include "DXApp.h"


namespace
{
	//USED TO FORWARD MESSAGES TO USER DEFINED PROC FUNCTION
	DXApp* g_pApp = nullptr; //GLOBAL POINTER
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (g_pApp)return g_pApp->MsgProc(hwnd, msg, wParam, lParam); //HOME MADE MSG PROC 
	else return DefWindowProc(hwnd, msg, wParam, lParam);
}


DXApp::DXApp(HINSTANCE hInstance)
{
	m_hAppInstance = hInstance; 
	m_hAppWnd = NULL;
	m_ClientHeight = 600;
	m_ClientWidth = 800;
	m_AppTitle = "DIRECTX11 APPLICATION -- WIN 32 SETUP";
	m_WndStyle = WS_OVERLAPPEDWINDOW; //BASIc resize window
	g_pApp = this;

	m_pDevice = nullptr;
	m_pImmediateContext = nullptr;
	m_pRenderTargetView = nullptr;
	m_pSwapChain = nullptr;
}


DXApp::~DXApp()
{
	//CLEANUP DIRECT3D X 
	if (m_pImmediateContext)m_pImmediateContext->ClearState(); //clears its state
	Memory::SafeRelease(m_pRenderTargetView);
	Memory::SafeRelease(m_pSwapChain);
	Memory::SafeRelease(m_pImmediateContext);
	Memory::SafeRelease(m_pDevice);
}


int DXApp::Run()
{
	//MAIN MESSAGE LOOP
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else // FREE OF messages so update
		{
			//UPDATE
			Update(0.0f);

			//RENDER 
			Render(0.0f);
		}
	}

	return static_cast<int>(msg.wParam);

}

bool DXApp::Init()
{
	if (!InitWindow())
	{
		return false;
	}

	if (!InitDirect3D())
	{
		return false;
	}

	return true;
}

bool DXApp::InitWindow()
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX)); //Setting it to empty memory
	wcex.cbClsExtra = 0; // settings for setting aside more memory
	wcex.cbWndExtra = 0; // same .. don't need any extra memory 
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW; //not using the gdi .. overriding the pixel data.
	wcex.hInstance = m_hAppInstance;
	wcex.lpfnWndProc = MainWndProc; // setting the call back we created 
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); //ICON 
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "DXAPPWNDCLASS";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		OutputDebugString("\nFailed to create window class\n");
		return false;
	}
	//IF IT SUCCEEDS WE SHOULD TRY TO MAKE OUR WINDOW

	RECT r = { 0, 0, m_ClientWidth, m_ClientHeight };
	AdjustWindowRect(&r, m_WndStyle, FALSE);
	UINT width = r.right - r.left;
	UINT height = r.bottom - r.top;

	UINT x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2; // GETTING THE CENTER OF THE SCREEN
	UINT y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

	m_hAppWnd = CreateWindow("DXAPPWNDCLASS", m_AppTitle.c_str(), m_WndStyle, x, y, width, height, NULL , NULL, m_hAppInstance, NULL);
	
	if (!m_hAppWnd)
	{
		OutputDebugString("\nFailed to create window\n");
		return false;
	}

	ShowWindow(m_hAppWnd, SW_SHOW);
	
	return true;
}



bool DXApp::InitDirect3D()
{
	UINT createDeviceFlags = 0;

//flag we pass to our device creation .. if you have debug enabled
#ifdef DEBUG
	createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,		//Means you have a graphics card or some sort of integrated graphics card (allows lighting transforms)
		D3D_DRIVER_TYPE_WARP,			//if you dont have a card *generally dont use*
		D3D_DRIVER_TYPE_REFERENCE		//*generally dont use*
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,

	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	//SWAPCHAIN ... front buffer , back buffer... double buffer rendering ... describes how we want the swap to happen

	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1; //0 single 1 double buffered
	swapDesc.BufferDesc.Width = m_ClientWidth;
	swapDesc.BufferDesc.Height = m_ClientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1; //BAsically setting to 60 FRAMES/ SEC
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //we want to render to the target of the window
	swapDesc.OutputWindow = m_hAppWnd; //Handle to the Application Window
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //Fastest and most efficient
	swapDesc.Windowed = true;
	swapDesc.SampleDesc.Count = 1;  //MultiSampling kinda like anti aliasing ... Wrecks frame rate.
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allows when u do alt-Enter  .. puts it into full screen wont resize the buffer tho .. 


	HRESULT result;	//ERROR CODE HANDLE
	for (int i = 0; i < numDriverTypes; ++i)
	{
		result = D3D11CreateDeviceAndSwapChain(NULL,
			driverTypes[i],
			NULL,
			createDeviceFlags,
			featureLevels,
			numFeatureLevels,
			D3D11_SDK_VERSION,
			&swapDesc,
			&m_pSwapChain,			//Presenting and managing the buffers 
			&m_pDevice,				//sets states on the device, shader ..
			&m_FeatureLevel,
			&m_pImmediateContext);	//HANDLES RENDERING 
		if (SUCCEEDED(result))
		{
			m_DriverType = driverTypes[i];
			break;
		}

	}
	if (FAILED(result))
	{
		OutputDebugString("fAILED TO CREATE DEVICE AND SWAP CHAIN");
		return false;
	}


	//CREATE THE RENDER TARGET VIEW  *basically a big image that we render stuff to first create the texture*
	ID3D11Texture2D*		m_pBackBufferTex;
	HR(m_pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_pBackBufferTex)));
	HR(m_pDevice->CreateRenderTargetView(m_pBackBufferTex, nullptr, &m_pRenderTargetView)); //returns an HRESULT SO WE GRAB IT TO DEAL WITH ERRORS
	Memory::SafeRelease(m_pBackBufferTex);

	//BIND RENDER TARGET VIEW
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr); //Binding one view to the render target .. nullptr is the depth sense .. not doing this yet.

	//VIEWPORT CREATION
	m_Viewport.Width = static_cast<float>(m_ClientWidth);
	m_Viewport.Height = static_cast<float>(m_ClientHeight);
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	//BIND VIEWPORT
	m_pImmediateContext->RSSetViewports(1, &m_Viewport);

	return true;

	//THIS SHOULD SET IT UP FOR USING DIRECT X 11

}






LRESULT DXApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default: 
		return DefWindowProc(hwnd, msg, wParam, lParam); //KEYCODES STORED IN LPARAM AND WPARAM
	}
}



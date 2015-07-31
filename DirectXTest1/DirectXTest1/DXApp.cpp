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
	m_ScreenWidth		= 0;
	m_ScreenHeight		= 0;
	m_pGameTimer		= new GameTimer();
	m_hAppInstance		= hInstance; 
	m_hAppWnd			= NULL;
	m_ClientHeight		= SCREEN_HEIGHT;
	m_ClientWidth		= SCREEN_WIDTH;
	m_AppTitle			= WINDOW_TITLE;
	m_WndStyle			= WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX; //Basic resize window
	g_pApp				= this;

	m_pDevice					= nullptr;
	m_pImmediateContext			= nullptr;
	m_pRenderTargetView			= nullptr;
	m_pSwapChain				= nullptr;
	m_pDepthStencilBuffer		= nullptr;
	m_pDepthStencilState		= nullptr;
	m_pDisableDepthStencil		= nullptr;
	m_pDepthStencilView			= nullptr;
	m_pRasterState				= nullptr;
	m_pAlphaEnableBlendState	= nullptr;
	m_pAlphaDisableBlendState	= nullptr;
	

}


DXApp::~DXApp()
{
	if (m_pSwapChain)
	{
		m_pSwapChain->SetFullscreenState(false, NULL);
	}

	//CLEANUP DIRECT3D X 
	if (m_pImmediateContext)m_pImmediateContext->ClearState(); //clears its state
	Memory::SafeRelease(m_pRenderTargetView);
	Memory::SafeRelease(m_pSwapChain);
	Memory::SafeRelease(m_pImmediateContext);
	Memory::SafeRelease(m_pDevice);
	Memory::SafeRelease(m_pDepthStencilBuffer);
	Memory::SafeRelease(m_pDepthStencilState);
	Memory::SafeRelease(m_pDisableDepthStencil);
	Memory::SafeRelease(m_pDepthStencilView);
	Memory::SafeRelease(m_pRasterState);
	Memory::SafeRelease(m_pAlphaEnableBlendState);
	Memory::SafeRelease(m_pAlphaDisableBlendState);
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}
	delete m_pGameTimer;
	m_pGameTimer = nullptr;
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
			//GAMETIMER
			m_pGameTimer->Tick();

			//UPDATE
			Update(m_pGameTimer->DeltaTime());

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

void DXApp::BeginScene(float r, float g, float b, float a)
{

}

bool DXApp::InitWindow()
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX)); //Setting it to empty memory
	wcex.cbClsExtra			= 0; // settings for setting aside more memory
	wcex.cbWndExtra			= 0; // same .. don't need any extra memory 
	wcex.cbSize				= sizeof(WNDCLASSEX);
	wcex.style				= CS_HREDRAW | CS_VREDRAW; //not using the gdi .. overriding the pixel data.
	wcex.hInstance			= m_hAppInstance;
	wcex.lpfnWndProc		= MainWndProc; // setting the call back we created 
	wcex.hIcon				= LoadIcon(NULL, IDI_APPLICATION); //ICON 
	wcex.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground		= (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName		= NULL;
	wcex.lpszClassName		= "DXAPPWNDCLASS";
	wcex.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		OutputDebugString("\nFailed to create window class\n");
		return false;
	}
	//IF IT SUCCEEDS WE SHOULD TRY TO MAKE OUR WINDOW

	UINT newWidth	= GetSystemMetrics(SM_CXSCREEN);
	UINT newHeight	= GetSystemMetrics(SM_CYSCREEN);
	m_ScreenWidth	= newWidth;
	m_ScreenHeight	= newHeight; //Storing screen dimentions in the app
	UINT x;
	UINT y;
	if (FULL_SCREEN)
	{
		x									= 0;
		y									= 0;
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize				= sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth		= (unsigned long)newWidth;
		dmScreenSettings.dmPelsHeight		= (unsigned long)newHeight;
		dmScreenSettings.dmBitsPerPel		= 32;
		dmScreenSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}
	else
	{
		RECT r = { 0, 0, m_ClientWidth, m_ClientHeight };
		AdjustWindowRect(&r, m_WndStyle, FALSE);
		newWidth = r.right - r.left;
		newHeight = r.bottom - r.top;

		x = GetSystemMetrics(SM_CXSCREEN) / 2 - newWidth / 2; // GETTING THE CENTER OF THE SCREEN
		y = GetSystemMetrics(SM_CYSCREEN) / 2 - newHeight / 2;
	}



	m_hAppWnd = CreateWindowEx(WS_EX_APPWINDOW,"DXAPPWNDCLASS", m_AppTitle.c_str(), m_WndStyle, x, y, newWidth, newHeight, NULL, NULL, m_hAppInstance, NULL);
	
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


	//Get Graphics Card info And Monitor Dimentions/ Numerator/ Denomenator
	if (!InitGraphicsCard())
	{
		OutputDebugString("fAILED TO FIND MONITOR / GRAPHICS CARD");
		return false;
	}

	if (!InitSwapChain())
	{
		OutputDebugString("fAILED TO CREATE SWAP CHAIN AND DEVICE");
		return false;
	}

	if (!InitBackBuffer())
	{
		OutputDebugString("fAILED TO CREATE BACK BUFFER");
		return false;
	}

	if (!InitDepthBuffer())
	{
		OutputDebugString("fAILED TO CREATE DEPTH BUFFER");
		return false;
	}

	if (!InitDepthStencilBuffer())
	{
		OutputDebugString("fAILED TO CREATE DEPTH STENCIL BUFFER");
		return false;
	}

	D3D11_RASTERIZER_DESC rasterDesc;

	UINT m4MsaaQuality;
	HR(m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4MsaaQuality))
	assert(m4MsaaQuality > 0);


	//BIND RENDER TARGET VIEW
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr); //Binding one view to the render target .. nullptr is the depth sense .. not doing this yet.

	//VIEWPORT CREATION
	if (FULL_SCREEN)
	{
		m_Viewport.Width = static_cast<float>(m_ScreenWidth);
		m_Viewport.Height = static_cast<float>(m_ScreenHeight);
	}
	else
	{
		m_Viewport.Width = static_cast<float>(m_ClientWidth);
		m_Viewport.Height = static_cast<float>(m_ClientHeight);
	}

	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	//BIND VIEWPORT
	m_pImmediateContext->RSSetViewports(1, &m_Viewport);

	return true;

	//THIS SHOULD SET IT UP FOR USING DIRECT X 11

}



bool DXApp::InitGraphicsCard()
{

	HRESULT result; 
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, numerator, denomenator, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	
	//Create Direct x graphic interface factory
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	//use factory now
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result)){ return false; }

	//enumerate the primary output (monitor)
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	//Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the monitor 
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//Create a list to hold all the modes for the monitor/video card combo
	displayModeList = new DXGI_MODE_DESC[numModes];

	//fill the list
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	//Loop through the whole list finding what one equals screen width / height

	for (int i = 0; i < numModes; ++i)
	{
		if (displayModeList[i].Width == (unsigned int)m_ScreenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)m_ScreenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denomenator = displayModeList[i].RefreshRate.Denominator;
				m_MonitorDenumerator = numerator;
				m_MonitorNumerator = denomenator;
			}
		}
	}

	if (numerator == 0 && denomenator == 0)
	{
		return false;
	}

	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	//Store video car memory in MBs
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024); 

	//Convert the name of the car to a char array
	error = wcstombs_s(&stringLength, m_pVideoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}
	
	//RENAMING THE WINDOW 
	std::string newTitle = m_AppTitle;
	newTitle += " On The -> ";
	newTitle += m_pVideoCardDescription;
	newTitle += " With ";
	newTitle += std::to_string(m_videoCardMemory);
	newTitle += "Mbs ";
	SetWindowText(m_hAppWnd, newTitle.c_str());
	

	//Release memory 
	Memory::SafeDelete(displayModeList);
	
	adapterOutput->Release();
	adapterOutput = nullptr;
	adapter->Release();
	adapter = nullptr;
	factory->Release();
	factory = nullptr;

	return true;
}

bool DXApp::InitSwapChain()
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
	swapDesc.BufferCount = 1;									//0 single 1 double buffered

	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// 
	swapDesc.BufferDesc.RefreshRate.Numerator = m_MonitorNumerator;
	swapDesc.BufferDesc.RefreshRate.Denominator = m_MonitorDenumerator;		//BAsically setting to 60 FRAMES/ SEC
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		//we want to render to the target of the window
	swapDesc.OutputWindow = m_hAppWnd;							//Handle to the Application Window
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;				//Fastest and most efficient

	swapDesc.SampleDesc.Count = 1;									//MultiSampling kinda like anti aliasing ... Wrecks frame rate.
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allows when u do alt-Enter  .. puts it into full screen wont resize the buffer tho .. 

	//SET FULLSCREEN CAPABILITIES
	if (FULL_SCREEN)
	{
		swapDesc.BufferDesc.Width = m_ScreenWidth;						// Set to the windows width
		swapDesc.BufferDesc.Height = m_ScreenHeight;
		swapDesc.Windowed = false;
	}
	else
	{
		swapDesc.BufferDesc.Width = m_ClientWidth;						// Set to the windows width
		swapDesc.BufferDesc.Height = m_ClientHeight;
		swapDesc.Windowed = true;
	}

	//Set Scan line ordering and scaling to unspecified
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;



	HRESULT result;	//ERROR CODE HANDLE
	for (int i = 0; i < numDriverTypes; ++i)
	{
		result = D3D11CreateDeviceAndSwapChain(
			NULL,
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
			return true;
		}

	}
	if (FAILED(result))
	{
		OutputDebugString("fAILED TO CREATE DEVICE AND SWAP CHAIN");
		return false;
	}
}

bool DXApp::InitBackBuffer()
{
	//CREATE THE RENDER TARGET VIEW  *basically a big image that we render stuff to first create the texture*
	ID3D11Texture2D* m_pBackBufferTex;
	//m_pBackBufferTex->GetDesc();
	HR(m_pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_pBackBufferTex)));
	HR(m_pDevice->CreateRenderTargetView(m_pBackBufferTex, nullptr, &m_pRenderTargetView)); //returns an HRESULT SO WE GRAB IT TO DEAL WITH ERRORS
	Memory::SafeRelease(m_pBackBufferTex);
	return true;
}

bool DXApp::InitDepthBuffer()
{
	//CREATE THE DEPTH BUFFER 
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	//initialize depth buffer description 
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	//set up depth buffer desc
	if (FULL_SCREEN)
	{
		depthBufferDesc.Width = m_ScreenWidth;
		depthBufferDesc.Height = m_ScreenHeight;
	}
	else
	{
		depthBufferDesc.Width = m_ClientWidth;
		depthBufferDesc.Height = m_ClientHeight;
	}
	depthBufferDesc.MipLevels				= 1;
	depthBufferDesc.ArraySize				= 1;
	depthBufferDesc.Format					= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count		= 1;
	depthBufferDesc.SampleDesc.Quality		= 0;
	depthBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags				= D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags			= 0;
	depthBufferDesc.MiscFlags				= 0;

	//create a texture for the depth buffer
	HR(m_pDevice->CreateTexture2D(&depthBufferDesc, NULL , &m_pDepthStencilBuffer));

	


	return true;
}

bool DXApp::InitDepthStencilBuffer()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable		= true;
	depthStencilDesc.DepthWriteMask		= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc			= D3D11_COMPARISON_LESS;
	
	depthStencilDesc.StencilEnable		= true;
	depthStencilDesc.StencilReadMask	= 0xFF;
	depthStencilDesc.StencilWriteMask	= 0xFF;

	//Stencil operations if pixel is front facing 
	depthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	//Stencil operations if pixel is back facing 
	depthStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	//create depth Stencil 
	HR(m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState));

	//SEt depth stencil State 
	m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

	return true;
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



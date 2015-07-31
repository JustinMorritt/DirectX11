#pragma once
#define WIN32_LEAN_AND_MEAN // Strips header file for only the necessary win 32 basics
#include <Windows.h>
#include <string>
#include "DXUtil.h"
#include "SystemDefs.h"
#include "GameTimer.h"

//ABSTRACT CLASS
class DXApp
{
public:
	DXApp(HINSTANCE hInstance);
	virtual ~DXApp();
	//TESTING OUT THE WAKA TIME 
	//MAIN APPLICATION LOOP
	int Run();

	//FRAMEWORK METHODS  *dt* obviously delta time
	virtual bool Init();
	virtual void Update(float dt) = 0; //PURE VIRTUAL *Must be overRidden*
	virtual void Render(float dt) = 0;
	
	

	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	

protected: //so we can have access in inherited classes 
	
	//TIMER 
	GameTimer* m_pGameTimer;
	
	//WIN32 ATTRIBUTES
	HWND		m_hAppWnd;
	HINSTANCE	m_hAppInstance;
	UINT		m_ClientWidth;
	UINT		m_ClientHeight;
	std::string m_AppTitle;
	DWORD		m_WndStyle;


	//DIRECTX ATTRIBUTES 
	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pImmediateContext;
	IDXGISwapChain*				m_pSwapChain;
	ID3D11RenderTargetView*		m_pRenderTargetView; //Where the Draws end up Going 
	ID3D11Texture2D*			m_pDepthStencilBuffer;
	ID3D11DepthStencilState*	m_pDepthStencilState;
	ID3D11DepthStencilState*	m_pDisableDepthStencil;  
	ID3D11DepthStencilView*		m_pDepthStencilView;
	ID3D11RasterizerState*		m_pRasterState;
	ID3D11BlendState*			m_pAlphaEnableBlendState;
	ID3D11BlendState*			m_pAlphaDisableBlendState;
	D3D_DRIVER_TYPE				m_DriverType;
	D3D_FEATURE_LEVEL			m_FeatureLevel;
	D3D11_VIEWPORT				m_Viewport;

	int							m_videoCardMemory;
	int							m_ScreenWidth;
	int							m_ScreenHeight;
	int							m_MonitorDenumerator;
	int							m_MonitorNumerator;
	char						m_pVideoCardDescription[128];

protected:

	//INITIALIZE WIN32 WINDOW
	bool InitWindow();

	bool InitDirect3D();
	bool InitGraphicsCard();
	bool InitSwapChain();
	bool InitBackBuffer();
	bool InitDepthBuffer();
	bool InitDepthStencilBuffer();


	void BeginScene(float r, float g, float b, float a);
	void EndScene();
	void EnableAlphaBlending(bool enable);
	void EnableZBuffer(bool enable);

};


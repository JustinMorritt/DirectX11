#ifndef _DXUTIL_H_
#define _DXUTIL_H_


#include <d3d11.h>
#include <Windows.h>
#include <d3dcommon.h>
#include <DirectXColors.h>
#include "dxerr.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

//CREATE A MACRO FOR ERROR HANDLING 

#ifdef _DEBUG
#ifndef HR
#define HR(x) \
{\
	HRESULT hr = x; \
	if (FAILED(hr))\
	{ \
		DXTraceW(__FILEW__, __LINE__, hr, L#x, TRUE); \
	} \
}
#endif
#ifndef HR
#define HR(x) x;
#endif
#endif


namespace Memory
{
	template <class T> void SafeDelete(T& t)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}

	template <class T> void SafeRelease(T& t)
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}

	template <class T> void SafeDeleteArr(T& t)
	{
		if (t)
		{
			delete[] t;
			t = nullptr;
		}
	}
	
}
#endif
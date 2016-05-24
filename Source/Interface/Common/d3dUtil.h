#ifndef D3DUTIL_H
#define D3DUTIL_H


#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <D3D11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "DDSTextureLoader.h"

#ifndef D3DX11INLINE
#ifdef _MSC_VER
#if (_MSC_VER >= 1200)
#define D3DX11INLINE __forceinline
#else
#define D3DX11INLINE __inline
#endif
#else
#ifdef __cplusplus
#define D3DX11INLINE inline
#else
#define D3DX11INLINE
#endif
#endif
#endif


#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)														\
	{																	\
		HRESULT hr = (x);												\
		if(FAILED(hr))													\
		{																\
			LPWSTR output;												\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |					\
							FORMAT_MESSAGE_IGNORE_INSERTS |				\
							FORMAT_MESSAGE_ALLOCATE_BUFFER ,			\
							NULL ,										\
							hr ,										\
							MAKELANGID(LANG_NEUTRAL , SUBLANG_DEFAULT),	\
							(LPWSTR)&output ,							\
							0 ,											\
							NULL);										\
		MessageBox(NULL , output , TEXT("Error") , MB_OK);				\
		}																\
	}
	#endif

#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif 



#define ReleaseCOM(x)	{ if(x) { x->Release(); x = 0; } }

#define SafeDelete(x)	{ delete x; x = 0; }

#endif
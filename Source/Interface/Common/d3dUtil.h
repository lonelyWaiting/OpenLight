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
#include "MathHelper.h"
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


class d3dHelper
{
public:
	//加载纹理数组
	static ID3D11ShaderResourceView* CreateTexture2DArraySRV(
			ID3D11Device* device, ID3D11DeviceContext* context,
			std::vector<std::wstring>& filenames );					//文件名数组
			


	static ID3D11ShaderResourceView* CreateRandomTexture1DSRV(ID3D11Device* device);
};



class TextHelper
{
public:
	//Convert T to string
	template<typename T>
	static D3DX11INLINE std::wstring ToString(const T& s)
	{
		std::wostringstream oss;		//宽字符版的输入字符串流
		oss << s;

		return oss.str;
	}


	//Convert string To type T
	template<typename T>
	static D3DX11INLINE T FromString(const std::wstring& s)
	{
		T x;
		std::wistringstream iss(s);
		iss >> x;

		return x;
	}
};


//提取视椎体的六个面
void ExtractFrustumPlanes(DirectX::XMFLOAT4 planes[6], DirectX::CXMMATRIX M);



// #define XMGLOBALCONST extern CONST __declspec(selectany)
//   1. extern so there is only one copy of the variable, and not a separate
//      private copy in each .obj.
//   2. __declspec(selectany) so that the compiler does not complain about
//      multiple definitions in a .cpp file (it can pick anyone and discard 
//      the rest because they are constant--all the same).

namespace Colors
{
	XMGLOBALCONST DirectX::XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Black = { 0.0f , 0.0f , 0.0f , 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Red = { 1.0f , 0.0f , 0.0f , 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Green = { 0.0f , 1.0f , 0.0f , 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Blue = { 0.0f , 0.0f , 1.0f , 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Yellow = { 1.0f , 1.0f , 0.0f , 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Cyan = { 0.0f , 1.0f , 1.0f , 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Magenta = { 1.0f , 0.0f , 1.0f , 1.0f };

	XMGLOBALCONST DirectX::XMVECTORF32 Silver = { 0.75f , 0.75f , 0.75f , 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 LightSteelBlue = { 0.69f , 0.77f , 0.87f , 1.0f };
}




class Convert
{
public:
	//Convert XMVECTOR to XMColor. where XMVECTOR represents a color
	static D3DX11INLINE DirectX::PackedVector::XMCOLOR ToXmColor(DirectX::FXMVECTOR v)
	{
		DirectX::PackedVector::XMCOLOR dest;
		DirectX::PackedVector::XMStoreColor(&dest, v);
		return dest;
	}

	//Convert XMVECTOR to XMFLOAT4 , where XMVECTOR represent a color
	static D3DX11INLINE DirectX::XMFLOAT4 ToXmFloat4(DirectX::FXMVECTOR v)
	{
		DirectX::XMFLOAT4 dest;
		DirectX::XMStoreFloat4(&dest, v);
		return dest;
	}


	static D3DX11INLINE UINT ArgbToAbgr(UINT argb)
	{
		BYTE A = (argb >> 24) & 0xff;
		BYTE R = (argb >> 16) & 0xff;
		BYTE G = (argb >> 8) & 0xff;
		BYTE B = (argb >> 0) & 0xff;

		return ((A << 24) | (B << 16) | (G << 8) | (R << 0));
	}
};
#endif
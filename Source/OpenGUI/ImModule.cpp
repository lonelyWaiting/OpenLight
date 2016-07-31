#include <imgui.h>
#include "ImModule.h"

// DirectX
#include <d3d11.h>
#include <d3dcompiler.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

static INT64                    gTime = 0;
static INT64                    gTicksPerSecond = 0;

static HWND                     ghWnd = 0;
static ID3D11Device*            gpDevice = NULL;
static ID3D11DeviceContext*     gpDeviceContext = NULL;
static ID3D11Buffer*            gpVertexBuffer = NULL;
static ID3D11Buffer*            gpIndexBuffer = NULL;
static ID3D10Blob *             gpVertexShaderBlob = NULL;
static ID3D11VertexShader*      gpVertexShader = NULL;
static ID3D11InputLayout*       gpInputLayout = NULL;
static ID3D11Buffer*            gpVertexConstantBuffer = NULL;
static ID3D10Blob *             gpPixelShaderBlob = NULL;
static ID3D11PixelShader*       gpPixelShader = NULL;
static ID3D11SamplerState*      gpFontSampler = NULL;
static ID3D11ShaderResourceView*gpFontTextureView = NULL;
static ID3D11RasterizerState*   gpRasterizerState = NULL;
static ID3D11BlendState*        gpBlendState = NULL;
static ID3D11DepthStencilState* gpDepthStencilState = NULL;
static int                      gVertexBufferSize = 5000 , gIndexBufferSize = 10000;

struct VERTEX_CONSTANT_BUFFER
{
	float        mvp[4][4];
};

void ImRenderDrawList( ImDrawData* data )
{
	ID3D11DeviceContext* context = gpDeviceContext;

	// Create and grow vertex/index buffers if needed
	if( !gpVertexBuffer || gVertexBufferSize < data->TotalVtxCount )
	{
		if( gpVertexBuffer )
		{
			gpVertexBuffer->Release(); 
			gpVertexBuffer = NULL;
		}
		gVertexBufferSize = data->TotalVtxCount + 5000;
		D3D11_BUFFER_DESC desc;
		memset( &desc , 0 , sizeof( D3D11_BUFFER_DESC ) );
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = gVertexBufferSize * sizeof( ImDrawVert );
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		if( gpDevice->CreateBuffer( &desc , NULL , &gpVertexBuffer ) < 0 )
		{
			return;
		}
	}
	if( !gpIndexBuffer || gIndexBufferSize < data->TotalIdxCount )
	{
		if( gpIndexBuffer )
		{
			gpIndexBuffer->Release(); gpIndexBuffer = NULL;
		}
		gIndexBufferSize = data->TotalIdxCount + 10000;
		D3D11_BUFFER_DESC desc;
		memset( &desc , 0 , sizeof( D3D11_BUFFER_DESC ) );
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = gIndexBufferSize * sizeof( ImDrawIdx );
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		if( gpDevice->CreateBuffer( &desc , NULL , &gpIndexBuffer ) < 0 )
		{
			return;
		}
	}

	// Copy and convert all vertices into a single contiguous buffer
	D3D11_MAPPED_SUBRESOURCE vtx_resource , idx_resource;
	if( context->Map( gpVertexBuffer , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &vtx_resource ) != S_OK )
	{
		return;
	}
	if( context->Map( gpIndexBuffer , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &idx_resource ) != S_OK )
	{
		return;
	}
	ImDrawVert* vtx_dst = ( ImDrawVert* )vtx_resource.pData;
	ImDrawIdx* idx_dst = ( ImDrawIdx* )idx_resource.pData;
	for( int n = 0; n < data->CmdListsCount; n++ )
	{
		const ImDrawList* cmd_list = data->CmdLists[n];
		memcpy( vtx_dst , &cmd_list->VtxBuffer[0] , cmd_list->VtxBuffer.size() * sizeof( ImDrawVert ) );
		memcpy( idx_dst , &cmd_list->IdxBuffer[0] , cmd_list->IdxBuffer.size() * sizeof( ImDrawIdx ) );
		vtx_dst += cmd_list->VtxBuffer.size();
		idx_dst += cmd_list->IdxBuffer.size();
	}
	context->Unmap( gpVertexBuffer , 0 );
	context->Unmap( gpIndexBuffer , 0 );

	// Setup orthographic projection matrix into our constant buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		if( context->Map( gpVertexConstantBuffer , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &mapped_resource ) != S_OK )
		{
			return;
		}
		VERTEX_CONSTANT_BUFFER* constant_buffer = ( VERTEX_CONSTANT_BUFFER* )mapped_resource.pData;
		float L = 0.0f;
		float R = ImGui::GetIO().DisplaySize.x;
		float B = ImGui::GetIO().DisplaySize.y;
		float T = 0.0f;
		float mvp[4][4] =
		{
			{ 2.0f / ( R - L ),   0.0f,           0.0f,       0.0f },
			{ 0.0f,         2.0f / ( T - B ),     0.0f,       0.0f },
			{ 0.0f,         0.0f,           0.5f,       0.0f },
			{ ( R + L ) / ( L - R ),  ( T + B ) / ( B - T ),    0.5f,       1.0f },
		};
		memcpy( &constant_buffer->mvp , mvp , sizeof( mvp ) );
		context->Unmap( gpVertexConstantBuffer , 0 );
	}

	// Backup DX state that will be modified to restore it afterwards (unfortunately this is very ugly looking and verbose. Close your eyes!)
	struct BACKUP_DX11_STATE
	{
		UINT                        ScissorRectsCount , ViewportsCount;
		D3D11_RECT                  ScissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
		D3D11_VIEWPORT              Viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
		ID3D11RasterizerState*      RS;
		ID3D11BlendState*           BlendState;
		FLOAT                       BlendFactor[4];
		UINT                        SampleMask;
		UINT                        StencilRef;
		ID3D11DepthStencilState*    DepthStencilState;
		ID3D11ShaderResourceView*   PSShaderResource;
		ID3D11SamplerState*         PSSampler;
		ID3D11PixelShader*          PS;
		ID3D11VertexShader*         VS;
		UINT                        PSInstancesCount , VSInstancesCount;
		ID3D11ClassInstance*        PSInstances[256] , *VSInstances[256];   // 256 is max according to PSSetShader documentation
		D3D11_PRIMITIVE_TOPOLOGY    PrimitiveTopology;
		ID3D11Buffer*               IndexBuffer , *VertexBuffer , *VSConstantBuffer;
		UINT                        IndexBufferOffset , VertexBufferStride , VertexBufferOffset;
		DXGI_FORMAT                 IndexBufferFormat;
		ID3D11InputLayout*          InputLayout;
	};

	BACKUP_DX11_STATE old;
	old.ScissorRectsCount = old.ViewportsCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	context->RSGetScissorRects( &old.ScissorRectsCount , old.ScissorRects );
	context->RSGetViewports( &old.ViewportsCount , old.Viewports );
	context->RSGetState( &old.RS );
	context->OMGetBlendState( &old.BlendState , old.BlendFactor , &old.SampleMask );
	context->OMGetDepthStencilState( &old.DepthStencilState , &old.StencilRef );
	context->PSGetShaderResources( 0 , 1 , &old.PSShaderResource );
	context->PSGetSamplers( 0 , 1 , &old.PSSampler );
	old.PSInstancesCount = old.VSInstancesCount = 256;
	context->PSGetShader( &old.PS , old.PSInstances , &old.PSInstancesCount );
	context->VSGetShader( &old.VS , old.VSInstances , &old.VSInstancesCount );
	context->VSGetConstantBuffers( 0 , 1 , &old.VSConstantBuffer );
	context->IAGetPrimitiveTopology( &old.PrimitiveTopology );
	context->IAGetIndexBuffer( &old.IndexBuffer , &old.IndexBufferFormat , &old.IndexBufferOffset );
	context->IAGetVertexBuffers( 0 , 1 , &old.VertexBuffer , &old.VertexBufferStride , &old.VertexBufferOffset );
	context->IAGetInputLayout( &old.InputLayout );

	// Setup viewport
	D3D11_VIEWPORT vp;
	memset( &vp , 0 , sizeof( D3D11_VIEWPORT ) );
	vp.Width = ImGui::GetIO().DisplaySize.x;
	vp.Height = ImGui::GetIO().DisplaySize.y;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = vp.TopLeftY = 0.0f;
	context->RSSetViewports( 1 , &vp );

	// Bind shader and vertex buffers
	unsigned int stride = sizeof( ImDrawVert );
	unsigned int offset = 0;
	context->IASetInputLayout( gpInputLayout );
	context->IASetVertexBuffers( 0 , 1 , &gpVertexBuffer , &stride , &offset );
	context->IASetIndexBuffer( gpIndexBuffer , sizeof( ImDrawIdx ) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT , 0 );
	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	context->VSSetShader( gpVertexShader , NULL , 0 );
	context->VSSetConstantBuffers( 0 , 1 , &gpVertexConstantBuffer );
	context->PSSetShader( gpPixelShader , NULL , 0 );
	context->PSSetSamplers( 0 , 1 , &gpFontSampler );

	// Setup render state
	const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
	context->OMSetBlendState( gpBlendState , blend_factor , 0xffffffff );
	context->OMSetDepthStencilState( gpDepthStencilState , 0 );
	context->RSSetState( gpRasterizerState );

	// Render command lists
	int vtx_offset = 0;
	int idx_offset = 0;
	for( int n = 0; n < data->CmdListsCount; n++ )
	{
		const ImDrawList* cmd_list = data->CmdLists[n];
		for( int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++ )
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if( pcmd->UserCallback )
			{
				pcmd->UserCallback( cmd_list , pcmd );
			}
			else
			{
				const D3D11_RECT r = { ( LONG )pcmd->ClipRect.x, ( LONG )pcmd->ClipRect.y, ( LONG )pcmd->ClipRect.z, ( LONG )pcmd->ClipRect.w };
				context->PSSetShaderResources( 0 , 1 , ( ID3D11ShaderResourceView** )&pcmd->TextureId );
				context->RSSetScissorRects( 1 , &r );
				context->DrawIndexed( pcmd->ElemCount , idx_offset , vtx_offset );
			}
			idx_offset += pcmd->ElemCount;
		}
		vtx_offset += cmd_list->VtxBuffer.size();
	}

	// Restore modified DX state
	context->RSSetScissorRects( old.ScissorRectsCount , old.ScissorRects );
	context->RSSetViewports( old.ViewportsCount , old.Viewports );
	context->RSSetState( old.RS ); if( old.RS ) old.RS->Release();
	context->OMSetBlendState( old.BlendState , old.BlendFactor , old.SampleMask ); if( old.BlendState ) old.BlendState->Release();
	context->OMSetDepthStencilState( old.DepthStencilState , old.StencilRef ); if( old.DepthStencilState ) old.DepthStencilState->Release();
	context->PSSetShaderResources( 0 , 1 , &old.PSShaderResource ); if( old.PSShaderResource ) old.PSShaderResource->Release();
	context->PSSetSamplers( 0 , 1 , &old.PSSampler ); if( old.PSSampler ) old.PSSampler->Release();
	context->PSSetShader( old.PS , old.PSInstances , old.PSInstancesCount ); if( old.PS ) old.PS->Release();
	for( UINT i = 0; i < old.PSInstancesCount; i++ ) if( old.PSInstances[i] ) old.PSInstances[i]->Release();
	context->VSSetShader( old.VS , old.VSInstances , old.VSInstancesCount ); if( old.VS ) old.VS->Release();
	context->VSSetConstantBuffers( 0 , 1 , &old.VSConstantBuffer ); if( old.VSConstantBuffer ) old.VSConstantBuffer->Release();
	for( UINT i = 0; i < old.VSInstancesCount; i++ ) if( old.VSInstances[i] ) old.VSInstances[i]->Release();
	context->IASetPrimitiveTopology( old.PrimitiveTopology );
	context->IASetIndexBuffer( old.IndexBuffer , old.IndexBufferFormat , old.IndexBufferOffset ); if( old.IndexBuffer ) old.IndexBuffer->Release();
	context->IASetVertexBuffers( 0 , 1 , &old.VertexBuffer , &old.VertexBufferStride , &old.VertexBufferOffset ); if( old.VertexBuffer ) old.VertexBuffer->Release();
	context->IASetInputLayout( old.InputLayout ); if( old.InputLayout ) old.InputLayout->Release();
}

IMGUI_API LRESULT ImWndProHandler( HWND hwnd , UINT msg , WPARAM wParam , LPARAM lParam )
{
	ImGuiIO& io = ImGui::GetIO();
	switch( msg )
	{
		case WM_LBUTTONDOWN:
			io.MouseDown[0] = true;
			return true;
		case WM_LBUTTONUP:
			io.MouseDown[0] = false;
			return true;
		case WM_RBUTTONDOWN:
			io.MouseDown[1] = true;
			return true;
		case WM_RBUTTONUP:
			io.MouseDown[1] = false;
			return true;
		case WM_MBUTTONDOWN:
			io.MouseDown[2] = true;
			return true;
		case WM_MBUTTONUP:
			io.MouseDown[2] = false;
			return true;
		case WM_MOUSEWHEEL:
			io.MouseWheel += GET_WHEEL_DELTA_WPARAM( wParam ) > 0 ? +1.0f : -1.0f;
			return true;
		case WM_MOUSEMOVE:
			io.MousePos.x = ( signed short )( lParam );
			io.MousePos.y = ( signed short )( lParam >> 16 );
			return true;
		case WM_KEYDOWN:
			if( wParam < 256 )
				io.KeysDown[wParam] = 1;
			return true;
		case WM_KEYUP:
			if( wParam < 256 )
				io.KeysDown[wParam] = 0;
			return true;
		case WM_CHAR:
			// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
			if( wParam > 0 && wParam < 0x10000 )
				io.AddInputCharacter( ( unsigned short )wParam );
			return true;
	}

	return 0;
}

static void ImCreateFontTexture()
{
	// Build texture atlas
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width , height;
	io.Fonts->GetTexDataAsRGBA32( &pixels , &width , &height );

	// Upload texture to graphics system
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory( &desc , sizeof( desc ) );
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		ID3D11Texture2D *pTexture = NULL;
		D3D11_SUBRESOURCE_DATA subResource;
		subResource.pSysMem = pixels;
		subResource.SysMemPitch = desc.Width * 4;
		subResource.SysMemSlicePitch = 0;
		gpDevice->CreateTexture2D( &desc , &subResource , &pTexture );

		// Create texture view
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory( &srvDesc , sizeof( srvDesc ) );
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		gpDevice->CreateShaderResourceView( pTexture , &srvDesc , &gpFontTextureView );
		pTexture->Release();
	}

	// Store our identifier
	io.Fonts->TexID = ( void * )gpFontTextureView;

	// Create texture sampler
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory( &desc , sizeof( desc ) );
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.MipLODBias = 0.f;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.MinLOD = 0.f;
		desc.MaxLOD = 0.f;
		gpDevice->CreateSamplerState( &desc , &gpFontSampler );
	}
}

bool ImCreateDeviceObjects()
{
	if( !gpDevice )		return false;
	if( gpFontSampler )	ImInvalidateDeviceObjects();

	{
		static const char* vertexShader =
			"cbuffer vertexBuffer : register(b0) \
            {\
				float4x4 ProjectionMatrix; \
            };\
            struct VS_INPUT\
            {\
				float2 pos : POSITION;\
				float4 col : COLOR0;\
				float2 uv  : TEXCOORD0;\
            };\
            \
            struct PS_INPUT\
            {\
				float4 pos : SV_POSITION;\
				float4 col : COLOR0;\
				float2 uv  : TEXCOORD0;\
            };\
            \
            PS_INPUT main(VS_INPUT input)\
            {\
				PS_INPUT output;\
				output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
				output.col = input.col;\
				output.uv  = input.uv;\
				return output;\
            }";

		D3DCompile( vertexShader , strlen( vertexShader ) , NULL , NULL , NULL , "main" , "vs_4_0" , 0 , 0 , &gpVertexShaderBlob , NULL );
		if( gpVertexShaderBlob == NULL )
		{
			return false;
		}

		if( gpDevice->CreateVertexShader( ( DWORD* )gpVertexShaderBlob->GetBufferPointer() , gpVertexShaderBlob->GetBufferSize() , NULL , &gpVertexShader ) != S_OK )
		{
			return false;
		}
	}

	{
		static const char* pixelShader =
			"struct PS_INPUT\
            {\
				float4 pos : SV_POSITION;\
				float4 col : COLOR0;\
				float2 uv  : TEXCOORD0;\
            };\
            sampler sampler0;\
            Texture2D texture0;\
            \
            float4 main(PS_INPUT input) : SV_Target\
            {\
				float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
				return out_col; \
            }";

		D3DCompile( pixelShader , strlen( pixelShader ) , NULL , NULL , NULL , "main" , "ps_4_0" , 0 , 0 , &gpPixelShaderBlob , NULL );
		if( gpPixelShaderBlob == NULL )
		{
			return false;
		}

		if( gpDevice->CreatePixelShader( ( DWORD* )gpPixelShaderBlob->GetBufferPointer() , gpPixelShaderBlob->GetBufferSize() , NULL , &gpPixelShader ) != S_OK )
		{
			return false;
		}
	}

	{
		D3D11_INPUT_ELEMENT_DESC local_layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, ( size_t )( &( ( ImDrawVert* )0 )->pos ), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, ( size_t )( &( ( ImDrawVert* )0 )->uv ),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, ( size_t )( &( ( ImDrawVert* )0 )->col ), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		if( gpDevice->CreateInputLayout( local_layout , 3 , gpVertexShaderBlob->GetBufferPointer() , gpVertexShaderBlob->GetBufferSize() , &gpInputLayout ) != S_OK )
		{
			return false;
		}

		{
			D3D11_BUFFER_DESC desc;
			desc.ByteWidth = sizeof( VERTEX_CONSTANT_BUFFER );
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			gpDevice->CreateBuffer( &desc , NULL , &gpVertexConstantBuffer );
		}
	}

	{
		D3D11_BLEND_DESC desc;
		ZeroMemory( &desc , sizeof( desc ) );
		desc.AlphaToCoverageEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		gpDevice->CreateBlendState( &desc , &gpBlendState );
	}

	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory( &desc , sizeof( desc ) );
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.ScissorEnable = true;
		desc.DepthClipEnable = true;
		gpDevice->CreateRasterizerState( &desc , &gpRasterizerState );
	}

	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory( &desc , sizeof( desc ) );
		desc.DepthEnable = false;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		desc.StencilEnable = false;
		desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.BackFace = desc.FrontFace;
		gpDevice->CreateDepthStencilState( &desc , &gpDepthStencilState );
	}

	ImCreateFontTexture();

	return true;
}

void ImInvalidateDeviceObjects()
{
	if( !gpDevice )
		return;

	if( gpFontSampler )
	{
		gpFontSampler->Release(); gpFontSampler = NULL;
	}
	if( gpFontTextureView )
	{
		gpFontTextureView->Release(); gpFontTextureView = NULL; ImGui::GetIO().Fonts->TexID = 0;
	}
	if( gpIndexBuffer )
	{
		gpIndexBuffer->Release(); gpIndexBuffer = NULL;
	}
	if( gpVertexBuffer )
	{
		gpVertexBuffer->Release(); gpVertexBuffer = NULL;
	}

	if( gpBlendState )
	{
		gpBlendState->Release(); gpBlendState = NULL;
	}
	if( gpDepthStencilState )
	{
		gpDepthStencilState->Release(); gpDepthStencilState = NULL;
	}
	if( gpRasterizerState )
	{
		gpRasterizerState->Release(); gpRasterizerState = NULL;
	}
	if( gpPixelShader )
	{
		gpPixelShader->Release(); gpPixelShader = NULL;
	}
	if( gpPixelShaderBlob )
	{
		gpPixelShaderBlob->Release(); gpPixelShaderBlob = NULL;
	}
	if( gpVertexConstantBuffer )
	{
		gpVertexConstantBuffer->Release(); gpVertexConstantBuffer = NULL;
	}
	if( gpInputLayout )
	{
		gpInputLayout->Release(); gpInputLayout = NULL;
	}
	if( gpVertexShader )
	{
		gpVertexShader->Release(); gpVertexShader = NULL;
	}
	if( gpVertexShaderBlob )
	{
		gpVertexShaderBlob->Release(); gpVertexShaderBlob = NULL;
	}
}

bool ImInit( void* hwnd , ID3D11Device* device , ID3D11DeviceContext* context )
{
	ghWnd = ( HWND )hwnd;
	gpDevice = device;
	gpDeviceContext = context;

	if( !QueryPerformanceFrequency( ( LARGE_INTEGER * )&gTicksPerSecond ) )
		return false;
	if( !QueryPerformanceCounter( ( LARGE_INTEGER * )&gTime ) )
		return false;

	ImGuiIO& io = ImGui::GetIO();
	io.KeyMap[ImGuiKey_Tab] = VK_TAB;                       // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array that we will update during the application lifetime.
	io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
	io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
	io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
	io.KeyMap[ImGuiKey_Home] = VK_HOME;
	io.KeyMap[ImGuiKey_End] = VK_END;
	io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
	io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
	io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
	io.KeyMap[ImGuiKey_A] = 'A';
	io.KeyMap[ImGuiKey_C] = 'C';
	io.KeyMap[ImGuiKey_V] = 'V';
	io.KeyMap[ImGuiKey_X] = 'X';
	io.KeyMap[ImGuiKey_Y] = 'Y';
	io.KeyMap[ImGuiKey_Z] = 'Z';

	io.RenderDrawListsFn = ImRenderDrawList;  // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
	io.ImeWindowHandle = ghWnd;

	return true;
}

void ImShutdown()
{
	ImInvalidateDeviceObjects();

	ImGui::Shutdown();

	gpDevice        = nullptr;
	gpDeviceContext = nullptr;
	ghWnd              = ( HWND )0;
}

void ImNewFrame()
{
	if( !gpFontSampler )
	{
		ImCreateDeviceObjects();
	}

	ImGuiIO& io = ImGui::GetIO();

	// Setup display size (every frame to accommodate for window resizing)
	RECT rect;
	GetClientRect( ghWnd , &rect );
	io.DisplaySize = ImVec2( ( float )( rect.right - rect.left ) , ( float )( rect.bottom - rect.top ) );

	// Setup time step
	INT64 current_time;
	QueryPerformanceCounter( ( LARGE_INTEGER * )&current_time );
	io.DeltaTime = ( float )( current_time - gTime ) / gTicksPerSecond;
	gTime = current_time;

	// Read keyboard modifiers inputs
	io.KeyCtrl  = ( GetKeyState( VK_CONTROL ) & 0x8000 ) != 0;
	io.KeyShift = ( GetKeyState( VK_SHIFT ) & 0x8000 ) != 0;
	io.KeyAlt   = ( GetKeyState( VK_MENU ) & 0x8000 ) != 0;
	io.KeySuper = false;
	// io.KeysDown : filled by WM_KEYDOWN/WM_KEYUP events
	// io.MousePos : filled by WM_MOUSEMOVE events
	// io.MouseDown : filled by WM_*BUTTON* events
	// io.MouseWheel : filled by WM_MOUSEWHEEL events

	// Hide OS mouse cursor if ImGui is drawing it
	SetCursor( io.MouseDrawCursor ? NULL : LoadCursor( NULL , IDC_ARROW ) );

	// Start the frame
	ImGui::NewFrame();
}
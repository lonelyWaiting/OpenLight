#pragma once

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ImDrawData;

IMGUI_IMPL_API bool ImInit( void* hwnd , ID3D11Device* device , ID3D11DeviceContext* context );
IMGUI_IMPL_API void ImShutdown();
IMGUI_IMPL_API void ImNewFrame();
IMGUI_IMPL_API void ImRenderDrawData();
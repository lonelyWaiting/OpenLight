#pragma once

struct ID3D11Device;
struct  ID3D11DeviceContext;

IMGUI_API bool ImInit( void* hwnd , ID3D11Device* device , ID3D11DeviceContext* context );

IMGUI_API void ImShutdown();

IMGUI_API void ImNewFrame();

IMGUI_API void ImInvalidateDeviceObjects();

IMGUI_API bool ImCreateDeviceObjects();
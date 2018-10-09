#include "imgui.h"
#include "imguiWin32.h"
#include "imguiDx11.h"
#include "ImModule.h"

bool ImInit(void* hwnd, ID3D11Device* device, ID3D11DeviceContext* context)
{
	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, context);

	return true;
}

void ImNewFrame()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImShutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImRenderDrawData()
{
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
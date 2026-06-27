#include <windows.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "Sodo.h"

void Sodo::CloseFenceEvent()
{
	CloseHandle(m_fenceEvent);
	m_fenceEvent = nullptr;
}

void Sodo::CloseImGui()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
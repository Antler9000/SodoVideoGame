#include <windows.h>
#include <d3dx12_root_signature.h>
#include <d3dx12_barriers.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <dxgi.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "Sodo.h"
#include "Game.h"
#include "Debug.h"

using namespace DirectX;
using std::wstring;

void Sodo::UpdateTimers()
{
	m_timerTotal.Update();
	m_timerFrame.Update();

	m_timerFrame.Mark();
}

void Sodo::UpdateCaption()
{
#ifdef _DEBUG
	m_timerCaption.Update();

	//NOTE : SetWindowTextW를 너무 자주 호출하면 시스템 부하로 인해 윈도우 전체가 먹통이 되니 반복에 텀을 주자
	if (m_timerCaption.GetTimeMilli() > 100.0f)
	{
		int fps = static_cast<int>(1000 / m_timerFrame.GetTimeMilli());

		wchar_t captionBuffer[256] = { };
		swprintf_s(
			captionBuffer,
			_countof(captionBuffer),
			L"%s (경과 시간 : %06.1f s / 프레임 시간 : %.4f ms / FPS : %3.d fps / 마우스 위치 : (%04d p, %04d p) / 스크롤 각도 : %04d unit)",
			m_pAppName,
			m_timerTotal.GetTimeMilli() / 1000,
			m_timerFrame.GetTimeMilli(),
			(fps > 999) ? 999 : fps,
			m_inputMousePositionClient.x,
			m_inputMousePositionClient.y,
			m_inputScrollDelta
		);

		SetWindowTextW(m_hWnd, captionBuffer);

		m_timerCaption.Mark();
	}
#endif
}

void Sodo::UpdateImGui()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	ImGuiViewport* imGuiViewPort = ImGui::GetMainViewport();
	ImVec2 imGuiCenterPos = imGuiViewPort->GetCenter();

	switch (m_gameState)
	{
		case GAME_STATE_IN_GAME:
		{
			RenderGuiInGame(imGuiViewPort, imGuiCenterPos);

			break;
		}

		case GAME_STATE_LOBBY:
		{
			RenderGuiLobbyMenu(imGuiViewPort, imGuiCenterPos);

			break;
		}

		case GAME_STATE_PAUSED:
		{
			RenderGuiPausedMenu(imGuiViewPort, imGuiCenterPos);

			break;
		}

		case GAME_STATE_LOADING_TO_GAME:
		{
			RenderGuiLoadingToGame(imGuiViewPort, imGuiCenterPos);

			break;
		}

		case GAME_STATE_LOADING_TO_LOBBY:
		{
			RenderGuiLoadingToLobby(imGuiViewPort, imGuiCenterPos);

			break;
		}

		case GAME_STATE_OPTION_FROM_LOBBY:
		{
			RenderGuiOptionFromLobby(imGuiViewPort, imGuiCenterPos);

			break;
		}

		case GAME_STATE_OPTION_FROM_PAUSED:
		{
			RenderGuiOptionFromPaused(imGuiViewPort, imGuiCenterPos);

			break;
		}

		case GAME_STATE_CHECK_EXIT_FROM_LOBBY_TO_WINDOWS:
		{
			RenderGuiCheckExitFromLobbyToWindows(imGuiViewPort, imGuiCenterPos);

			break;
		}

		case GAME_STATE_CHECK_EXIT_FROM_PAUSED_TO_WINDOWS:
		{
			RenderGuiCheckExitFromPausedToWindows(imGuiViewPort, imGuiCenterPos);

			break;
		}

		case GAME_STATE_CHECK_EXIT_FROM_PAUSED_TO_LOBBY:
		{
			RenderGuiCheckExitFromPausedToLobby(imGuiViewPort, imGuiCenterPos);

			break;
		}
	}
}

void Sodo::UpdateSreen()
{
	WaitAllCommandDone();

	ThrowIfFailed(m_commandAllocator->Reset());
	ThrowIfFailed(m_commandList->Reset(m_commandAllocator.Get(), nullptr));

	ID3D12DescriptorHeap* descriptorHeaps[] = { m_descriptorHeapCBVSRVUAV.Get() };
	m_commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	CD3DX12_RESOURCE_BARRIER barrierPresentToRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
		m_screenBackBuffers[m_screenBackBufferIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	m_commandList->ResourceBarrier(1, &barrierPresentToRenderTarget);

	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandleRTV = m_descriptorHeapRTVCpuStartHandle;
	cpuHandleRTV.Offset(m_screenBackBufferIndex, m_descriptorHeapRTVIncrementSize);

	m_commandList->OMSetRenderTargets(1, &cpuHandleRTV, true, &m_descriptorHeapDSVCpuStartHandle);
	m_commandList->RSSetViewports(1, &m_screenViewPort);
	m_commandList->RSSetScissorRects(1, &m_screenScissorRectangle);
		

	FLOAT sinZeroToOne = (XMScalarSin(static_cast<float>(m_timerTotal.GetTimeMilli()) / 1000) + 1) / 2;
	FLOAT testColor[4] = { sinZeroToOne, sinZeroToOne, sinZeroToOne, 1.0f };
	m_commandList->ClearRenderTargetView(cpuHandleRTV, testColor, 0, nullptr);
	m_commandList->ClearDepthStencilView(
		m_descriptorHeapDSVCpuStartHandle,
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
		1.0f,
		0,
		0,
		nullptr
	);

	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_commandList.Get());

	CD3DX12_RESOURCE_BARRIER barrierRenderTargetToPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		m_screenBackBuffers[m_screenBackBufferIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);
	m_commandList->ResourceBarrier(1, &barrierRenderTargetToPresent);

	ThrowIfFailed(m_commandList->Close());
	ID3D12CommandList* commandLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

	if (m_optionTearing.IsActive() == true)
	{
		ThrowIfFailed(m_screenSwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING));
	}
	else
	{
		ThrowIfFailed(m_screenSwapChain->Present(1, 0));
	}

	m_screenBackBufferIndex = m_screenSwapChain->GetCurrentBackBufferIndex();
}
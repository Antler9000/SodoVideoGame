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
	m_totalTimer.Update();
	m_frameTimer.Update();

	m_frameTimer.Mark();
}

void Sodo::UpdateCaption()
{
	m_captionTimer.Update();

	//NOTE : SetWindowTextW를 너무 자주 호출하면 시스템 부하로 인해 윈도우 전체가 먹통이 되니 반복에 텀을 주자
	if (m_captionTimer.GetTimeMilli() > 100.0f)
	{
		int fps = static_cast<int>(1000 / m_frameTimer.GetTimeMilli());

		wchar_t captionBuffer[256] = { };
		swprintf_s(
			captionBuffer,
			_countof(captionBuffer),
			L"%s (경과 시간 : %06.1f s / 프레임 시간 : %.4f ms / FPS : %3.d fps / 마우스 위치 : (%04d p, %04d p) / 스크롤 각도 : %04d unit)",
			m_pAppName,
			m_totalTimer.GetTimeMilli() / 1000,
			m_frameTimer.GetTimeMilli(),
			(fps > 999) ? 999 : fps,
			m_mousePositionClient.x,
			m_mousePositionClient.y,
			m_scrollDelta
		);

		SetWindowTextW(m_hWnd, captionBuffer);

		m_captionTimer.Mark();
	}
}

void Sodo::UpdateImGui()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	ImGuiViewport* viewPort	= ImGui::GetMainViewport();
	ImVec2 centerPos		= viewPort->GetCenter();
	
	switch (m_gameMode)
	{
		case GAME_STATE_IN_GAME:
		{
			RenderGuiInGame(viewPort, centerPos);

			return;
		}

		case GAME_STATE_LOBBY:
		{
			RenderGuiLobbyMenu(viewPort, centerPos);

			return;
		}

		case GAME_STATE_PAUSED:
		{
			RenderGuiPausedMenu(viewPort, centerPos);

			return;
		}

		case GAME_STATE_LOADING_TO_GAME:
		{
			RenderGuiLoadingToGame(viewPort, centerPos);

			return;
		}

		case GAME_STATE_LOADING_TO_LOBBY:
		{
			RenderGuiLoadingToLobby(viewPort, centerPos);

			return;
		}

		case GAME_STATE_OPTION_FROM_LOBBY:
		{
			RenderGuiOptionFromLobby(viewPort, centerPos);

			return;
		}

		case GAME_STATE_OPTION_FROM_PAUSED:
		{
			RenderGuiOptionFromPaused(viewPort, centerPos);

			return;
		}

		case GAME_STATE_CHECK_EXIT_FROM_LOBBY_TO_WINDOWS:
		{
			RenderGuiCheckExitFromLobbyToWindows(viewPort, centerPos);

			return;
		}

		case GAME_STATE_CHECK_EXIT_FROM_PAUSED_TO_WINDOWS:
		{
			RenderGuiCheckExitFromPausedToWindows(viewPort, centerPos);

			return;
		}

		case GAME_STATE_CHECK_EXIT_FROM_PAUSED_TO_LOBBY:
		{
			RenderGuiCheckExitFromPausedToLobby(viewPort, centerPos);

			return;
		}
	}
}

void Sodo::UpdateSreen()
{
	//TODO : 커맨드 얼로케이터를 여러개 두어서 병렬성을 높이자
	WaitAllCommandDone();

	ThrowIfFailed(m_commandAllocator->Reset());
	ThrowIfFailed(m_commandList->Reset(m_commandAllocator.Get(), nullptr));

	ID3D12DescriptorHeap* descriptorHeaps[] = { m_descriptorHeapCBVSRVUAV.Get() };
	m_commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	//TODO : PSO, 루트 시그니처는 여기서 설정하자

	CD3DX12_RESOURCE_BARRIER presentToRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
		m_backBuffers[m_currentBackBufferIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	m_commandList->ResourceBarrier(1, &presentToRenderTarget);

	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandleRTV = m_cpuStartHandleRTV;
	cpuHandleRTV.Offset(m_currentBackBufferIndex, m_incrementSizeRTV);

	m_commandList->OMSetRenderTargets(1, &cpuHandleRTV, true, &m_cpuStartHandleDSV);
	m_commandList->RSSetViewports(1, &m_viewPort);
	m_commandList->RSSetScissorRects(1, &m_scissorRectangle);

	FLOAT sinZeroToOne = (XMScalarSin(static_cast<float>(m_totalTimer.GetTimeMilli()) / 1000) + 1) / 2;
	FLOAT testColor[4] = { sinZeroToOne, sinZeroToOne, sinZeroToOne, 1.0f };
	m_commandList->ClearRenderTargetView(cpuHandleRTV, testColor, 0, nullptr);
	m_commandList->ClearDepthStencilView(
		m_cpuStartHandleDSV,
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
		1.0f,
		0,
		0,
		nullptr
	);

	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_commandList.Get());
	
	CD3DX12_RESOURCE_BARRIER renderTargetToPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		m_backBuffers[m_currentBackBufferIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);
	m_commandList->ResourceBarrier(1, &renderTargetToPresent);

	ThrowIfFailed(m_commandList->Close());
	ID3D12CommandList* commandLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

	if (m_optionTearing.IsActive() == true)
	{
		ThrowIfFailed(m_swapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING));
	}
	else
	{
		ThrowIfFailed(m_swapChain->Present(1, 0));
	}

	m_currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
}
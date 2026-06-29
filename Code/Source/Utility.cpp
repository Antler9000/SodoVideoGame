#include <windows.h>
#include <dxgi.h>
#include <stdexcept>
#include "Sodo.h"
#include "Debug.h"

void Sodo::WaitAllCommandDone()
{
	if (m_fenceEvent == nullptr || m_fence == nullptr || m_commandQueue == nullptr)
	{
		return;
	}

	m_currentFence++;

	ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_currentFence));

	ThrowIfFailed(m_fence->SetEventOnCompletion(m_currentFence, m_fenceEvent));
	auto waitResult = WaitForSingleObject(m_fenceEvent, INFINITE);

	if (waitResult != WAIT_OBJECT_0)
	{
		throw std::runtime_error("wait fence failed");
	}
}

void Sodo::ResetScreenSetting()
{
	if (m_needResetScreenMode)
	{
		if (m_optionFullScreen.userEnabled)
		{
			SetFullScreenMode();
		}
		else
		{
			SetWindowMode();
		}
	}

	if (m_swapChain != nullptr)
	{
		WaitAllCommandDone();

		for (int i = 0; i < m_backBufferCount; i++)
		{
			m_backBuffers[i].Reset();
		}

		ThrowIfFailed(
			m_swapChain->ResizeBuffers(
				0,
				0,
				0,
				m_optionHDR.IsActive() ? m_backBufferFormatHDR : m_backBufferFormatSDR,
				DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT
				| (m_optionTearing.featureSupported ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0)
			)
		);

		if (m_optionHDR.IsActive())
		{
			m_swapChain->SetColorSpace1(m_backBufferColorSpaceHDR);
		}
		else
		{
			m_swapChain->SetColorSpace1(m_backBufferColorSpaceSDR);
		}

		InitBackBuffers();
		InitViewPort();
		InitScissorRectangle();
		InitDepthStencilBuffer();
		InitRTV();
		InitDSV();
		InitImGui();
	}

	m_needResetHDR = false;
	m_needResetScreenMode = false;
}

void Sodo::SetFullScreenMode()
{
	SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

	HMONITOR monitor = MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monitorInfo{};
	monitorInfo.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(monitor, &monitorInfo);

	LONG monitorXBase = monitorInfo.rcMonitor.left;
	LONG monitorYBase = monitorInfo.rcMonitor.top;
	LONG monitorWidth = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
	LONG monitorHeight = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

	SetWindowPos(
		m_hWnd,
		HWND_TOP,
		monitorXBase,
		monitorYBase,
		monitorWidth,
		monitorHeight,
		SWP_NOOWNERZORDER | SWP_FRAMECHANGED
	);
}

void Sodo::SetWindowMode()
{
	SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);

	HMONITOR monitor = MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monitorInfo{};
	monitorInfo.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(monitor, &monitorInfo);

	LONG monitorXBase = monitorInfo.rcMonitor.left;
	LONG monitorYBase = monitorInfo.rcMonitor.top;
	LONG monitorWidth = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
	LONG monitorHeight = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

	LONG windowWidth = monitorWidth * 2 / 3;
	LONG windowHeight = monitorHeight * 2 / 3;
	LONG windowXBase = monitorXBase + (monitorWidth / 2) - (windowWidth / 2);
	LONG windowYBase = monitorYBase + (monitorHeight / 2) - (windowHeight / 2);

	SetWindowPos(
		m_hWnd,
		HWND_TOP,
		windowXBase,
		windowYBase,
		windowWidth,
		windowHeight,
		SWP_NOOWNERZORDER | SWP_FRAMECHANGED
	);
}

void Sodo::SaveOptions()
{
	
}

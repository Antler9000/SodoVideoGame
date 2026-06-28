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
	if (m_needResetFullScreen == true)
	{
		if (m_optionFullScreen.userEnabled == true)
		{
			SetBorderlessFullScreenMode();
		}
		else
		{
			SetToWindowedMode();
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

		if (m_optionHDR.IsActive() == true)
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
	m_needResetFullScreen = false;
}

void Sodo::SetBorderlessFullScreenMode()
{
	//TODO : 작성하자
}

void Sodo::SetToWindowedMode()
{
	//TODO : 작성하자
}
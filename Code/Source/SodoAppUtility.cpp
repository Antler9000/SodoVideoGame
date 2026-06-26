#include "windows.h"
#include "SodoApp.h"
#include "Debug.h"

void SodoApp::WaitAllCommandDone()
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

void SodoApp::CloseFenceEvent()
{
	CloseHandle(m_fenceEvent);
	m_fenceEvent = nullptr;
}
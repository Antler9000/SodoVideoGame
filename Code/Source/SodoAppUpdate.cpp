#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "SodoApp.h"

using std::wstring;

void SodoApp::UpdateTimers()
{
	m_totalTimer.Update();
	m_captionTimer.Update();
	m_frameTimer.Update();
}

void SodoApp::UpdateCaption()
{
	//NOTE : SetWindowTextW를 너무 자주 호출하면 시스템 부하로 인해 윈도우 전체가 먹통이 되니 반복에 텀을 주자
	if (m_captionTimer.GetTimeMilli() > 100.0f)
	{
		wchar_t captionBuffer[124] = { };
		swprintf_s(
			captionBuffer,
			_countof(captionBuffer),
			L"%s (경과 시간 : %06.1f s / 프레임 시간 : %.4f ms / 마우스 위치 : (%04d p, %04d p) / 스크롤 각도 : %04d unit)",
			m_pAppName,
			m_totalTimer.GetTimeMilli() / 1000,
			m_frameTimer.GetTimeMilli(),
			m_mousePositionClient.x,
			m_mousePositionClient.y,
			m_scrollDelta
		);

		SetWindowTextW(m_hWnd, captionBuffer);

		m_captionTimer.Reset();
	}
}

void SodoApp::UpdateSreen()
{
	//TODO : m_commandList->RSSetViewPorts(1, &m_viewPort), m_commandList->RSSsetScissorRect(1, &m_scissorRect)
	//TODO : 커맨드 리스트를 통해 현재 백버퍼의 상태를 PRESENT -> RENDER_TARGET으로 변경하자
	//TODO : 커맨드 리스트에 렌더 타겟을 설정하기

	m_frameTimer.Reset();
}
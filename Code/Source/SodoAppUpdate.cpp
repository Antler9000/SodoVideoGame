#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "SodoApp.h"

using std::wstring;

void SodoApp::UpdateTimer()
{
	m_timer.Update();
}

void SodoApp::UpdateCaption()
{
	//NOTE : SetWindowTextW를 너무 자주 호출하면 시스템 부하로 인해 윈도우 전체가 먹통이 되니 반복에 텀을 주자
	if (m_timer.GetNotCaptionedMilliSeconds() > 100.0f)
	{
		m_timer.MarkLastCaptionTime();

		wchar_t captionBuffer[124] = { };
		swprintf_s(
			captionBuffer,
			_countof(captionBuffer),
			L"%s (경과 시간 : %06.1f s / 프레임 시간 : %.4f ms / 마우스 위치 : (%04d p, %04d p) / 스크롤 각도 : %04d unit)",
			m_pAppName,
			m_timer.GetElapsedMilliSeconds() / 1000,
			m_timer.GetFrameMilliSeconds(),
			m_mousePositionClient.x,
			m_mousePositionClient.y,
			m_scrollDelta
		);

		SetWindowTextW(m_hWnd, captionBuffer);
	}
}
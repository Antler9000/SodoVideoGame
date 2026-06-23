#include <windows.h>
#include <windowsx.h>
#include <cstdio>
#include <cstdlib>
#include "SodoApp.h"

//NOTE :	WindowProc이 수행 중에는 해당 스레드의 메시지 큐에 쌓인 다른 메시지들을 처리하지 못하므로,
//			되도록 이 안에서는 짧은 로직만 수행하도록 하고, 긴 대기가 필요한 로직은 별도 스레드로 처리하자
LRESULT SodoApp::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		//TODO :	이는 픽셀 단위의 마우스 커서 변화를 감지하므로, 정확도가 떨어지고 마우스 포인터 속도 설정에 영향을 받음
		//			따라서 인게임 마우스 조작의 경우엔 본 메시지가 아니라 마우스의 이동 자체를 받아오도록 WM_INPUT을 사용하자
		//TODO :	추가로, DirectXTK12에 인풋과 관련하여 사용 가능한 기능이 있는지 둘러보자
	case WM_MOUSEMOVE:
	{
		InputMouseMove(wParam, lParam);

		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		InputMouseLeftButtonDown(wParam, lParam);

		return 0;
	}

	case WM_LBUTTONUP:
	{
		InputMouseLeftButtonUp(wParam, lParam);

		return 0;
	}

	case WM_RBUTTONDOWN:
	{
		InputMouseRightButtonDown(wParam, lParam);

		return 0;
	}

	case WM_RBUTTONUP:
	{
		InputMouseRightButtonUp(wParam, lParam);

		return 0;
	}

	case WM_MBUTTONDOWN:
	{
		InputMouseMiddleButtonDown(wParam, lParam);

		return 0;
	}

	case WM_MBUTTONUP:
	{
		InputMouseMiddleButtonUp(wParam, lParam);

		return 0;
	}

	case WM_MOUSEWHEEL:
	{
		InputMouseWheelScroll(wParam, lParam);

		return 0;
	}

	case WM_KEYDOWN:
	{
		InputKeyboardDown(wParam, lParam);

		return 0;
	}

	//NOTE : ALT+F4 혹은 우상단 창닫기 버튼을 이용한 종료를 처리함
	case WM_CLOSE:
	{
		if (MessageBoxW(m_hWnd, L"어플리케이션을 종료합니까?", L"종료 문구", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(m_hWnd);
		}

		return 0;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);

		return 0;
	}
	}

	return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}

void SodoApp::InputMouseMove(WPARAM wParam, LPARAM lParam)
{
	m_mousePositionClient.x = GET_X_LPARAM(lParam);
	m_mousePositionClient.y = GET_Y_LPARAM(lParam);
}

void SodoApp::InputMouseLeftButtonDown(WPARAM wParam, LPARAM lParam)
{
	if (m_bClicked == true)
	{
		return;
	}

	m_clickedPositionClient.x = GET_X_LPARAM(lParam);
	m_clickedPositionClient.y = GET_Y_LPARAM(lParam);
	m_bClicked = true;
}

void SodoApp::InputMouseLeftButtonUp(WPARAM wParam, LPARAM lParam)
{
	wchar_t messageBuffer[64] = { };
	swprintf_s(
		messageBuffer,
		_countof(messageBuffer),
		L"%s%s 시작 (x, y) = (%d, %d) \n 끝 (x, y) = (%d, %d)",
		wParam & MK_CONTROL ? L"[CTRL]" : L"",
		wParam & MK_SHIFT ? L"[SHIFT]" : L"",
		m_clickedPositionClient.x,
		m_clickedPositionClient.y,
		GET_X_LPARAM(lParam),
		GET_Y_LPARAM(lParam)
	);

	UINT mouseManhattanDist = abs(GET_X_LPARAM(lParam) - m_clickedPositionClient.x) + abs(GET_Y_LPARAM(lParam) - m_clickedPositionClient.y);
	bool isDrag = (mouseManhattanDist > m_dragThresholdDist);
	MessageBoxW(m_hWnd, messageBuffer, isDrag ? L"좌측 마우스 드래그" : L"좌측 마우스 클릭", MB_OK);

	m_bClicked = false;
}

void SodoApp::InputMouseRightButtonDown(WPARAM wParam, LPARAM lParam)
{
	if (m_bClicked == true)
	{
		return;
	}

	m_clickedPositionClient.x = GET_X_LPARAM(lParam);
	m_clickedPositionClient.y = GET_Y_LPARAM(lParam);
	m_bClicked = true;
}

void SodoApp::InputMouseRightButtonUp(WPARAM wParam, LPARAM lParam)
{
	wchar_t messageBuffer[64] = { };
	swprintf_s(
		messageBuffer,
		_countof(messageBuffer),
		L"%s 시작 (x, y) = (%d, %d) \n 끝 (x, y) = (%d, %d)",
		wParam & MK_SHIFT ? L"[SHIFT]" : L"",
		m_clickedPositionClient.x,
		m_clickedPositionClient.y,
		GET_X_LPARAM(lParam),
		GET_Y_LPARAM(lParam)
	);

	UINT mouseManhattanDist = abs(GET_X_LPARAM(lParam) - m_clickedPositionClient.x) + abs(GET_Y_LPARAM(lParam) - m_clickedPositionClient.y);
	bool isDragging = (mouseManhattanDist > m_dragThresholdDist);
	MessageBoxW(m_hWnd, messageBuffer, isDragging ? L"우측 마우스 드래그" : L"우측 마우스 클릭", MB_OK);

	m_bClicked = false;

}

void SodoApp::InputMouseMiddleButtonDown(WPARAM wParam, LPARAM lParam)
{
	if (m_bClicked == true)
	{
		return;
	}

	m_clickedPositionClient.x = GET_X_LPARAM(lParam);
	m_clickedPositionClient.y = GET_Y_LPARAM(lParam);
	m_bClicked = true;
}
void SodoApp::InputMouseMiddleButtonUp(WPARAM wParam, LPARAM lParam)
{
	wchar_t messageBuffer[64] = { };
	swprintf_s(
		messageBuffer,
		_countof(messageBuffer),
		L"시작 (x, y) = (%d, %d) \n 끝 (x, y) = (%d, %d)",
		m_clickedPositionClient.x,
		m_clickedPositionClient.y,
		GET_X_LPARAM(lParam),
		GET_Y_LPARAM(lParam)
	);

	UINT mouseManhattanDist = abs(GET_X_LPARAM(lParam) - m_clickedPositionClient.x) + abs(GET_Y_LPARAM(lParam) - m_clickedPositionClient.y);
	bool isDrag = (mouseManhattanDist > m_dragThresholdDist);
	MessageBoxW(m_hWnd, messageBuffer, isDrag ? L"중간 마우스 드래그" : L"중간 마우스 클릭", MB_OK);

	m_bClicked = false;
}

void SodoApp::InputMouseWheelScroll(WPARAM wParam, LPARAM lParam)
{
	m_scrollDelta += GET_WHEEL_DELTA_WPARAM(wParam);
}

void SodoApp::InputKeyboardDown(WPARAM wParam, LPARAM lParam)
{
	//ESC를 이용한 종료를 처리함
	if (wParam == VK_ESCAPE)
	{
		if (MessageBoxW(m_hWnd, L"ESC가 눌렸습니다. 어플리케이션을 종료합니까?", L"종료 문구", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(m_hWnd);
		}
	}
}
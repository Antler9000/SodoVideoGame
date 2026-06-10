#include <windows.h>
#include <windowsx.h>
#include <wchar.h>
#include "TheseusApp.h"

//NOTE : SetWindowTextW를 너무 자주 호출하면 시스템 부하로 인해 윈도우 전체가 먹통이 되니 반복문에 텀을 주자
void TheseusApp::PrintCaption()
{
	if (m_timer.GetNotCaptionedMilliSeconds() > 100.0f)
	{
		m_timer.Captioned();

		wchar_t captionBuffer[124] = { };
		swprintf(
			captionBuffer,
			_countof(captionBuffer),
			L"%s (경과 시간 : %06.1f s / 프레임 시간 : %.4f ms / 마우스 위치 : (%04d p, %04d p) / 스크롤 각도 : %04d unit)",
			m_pAppName,
			m_timer.GetEleapsedMilliSeconds() / 1000,
			m_timer.GetFrameMilliSeconds(),
			m_mousePosClientX,
			m_mousePosClientY,
			m_scrollDelta
		);

		SetWindowTextW(m_hWnd, captionBuffer);
	}
}

//TODO :	각 메시지 처리를 함수로 만들어서 가독성을 높이자
//NOTE :	WindowProc이 수행 중에는 해당 스레드의 메시지 큐에 있는 다른 메시지를 처리하지 못하므로,
//			되도록 WindowProcedure 안에서는 짧은 로직만 수행하도록 하고, 긴 대기가 필요한 로직은 별도 스레드로 처리하자
LRESULT TheseusApp::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		//TODO :	이는 픽셀 단위의 마우스 커서 변화를 감지하므로, 정확도가 떨어지고 마우스 포인터 속도 설정에 영향을 받음
		//			따라서 인게임 마우스 조작의 경우엔 본 메시지가 아니라 마우스의 이동 자체를 받아오도록 WM_INPUT을 사용하자
		//TODO :	idle 루프에서 아래 로직을 수행할 수 있도록 필요한 정보를 전달해주기만 하자
		//			-좌클릭 드래그 도중엔 유닛 선택 직사각형 영역이나 일자 스킬의 방향 표시 영역을 업데이트해야 한다
		//			-우클릭 드래그 도중엔 차량 회전 지시 방향의 화살표 표시를 업데이트해야 한다
		case WM_MOUSEMOVE:
		{
			m_mousePosClientX = GET_X_LPARAM(lParam);
			m_mousePosClientY = GET_Y_LPARAM(lParam);

			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			if (m_isClicked == true)
			{
				return 0;
			}

			m_clickedPosClientX = GET_X_LPARAM(lParam);
			m_clickedPosClientY = GET_Y_LPARAM(lParam);
			m_isClicked = true;

			return 0;
		}

		//TODO :	idle 루프에서 아래 로직을 수행할 수 있도록 필요한 정보를 전달해주기만 하자
		//			-클릭은 레이 케스팅으로 가장 가까운 유닛 하나를 선택하고, 드래그는 해당 드래그 영역에 대응되는 절두체의 내부에 위치한 유닛을 전부 선택함
		//			-CTRL이 같이 눌려있다면, 화면 공간 안의 유닛 중에서 선택된 유닛과 동종인 유닛을 모두 선택함
		//			-SHIFT가 같이 눌려있다면, 이전의 클릭 혹은 드래그로 이미 선택된 유닛들을 유지하며 새롭게 선택된 유닛들을 선택 대상에 추가함
		case WM_LBUTTONUP:
		{
			wchar_t messageBuffer[64] = { };
			swprintf_s(
				messageBuffer,
				_countof(messageBuffer),
				L"%s%s start (x, y) = (%d, %d) \n end (x, y) = (%d, %d)",
				wParam & MK_CONTROL ? L"[CTRL]" : L"",
				wParam & MK_SHIFT ? L"[SHIFT]" : L"",
				m_clickedPosClientX,
				m_clickedPosClientY,
				GET_X_LPARAM(lParam),
				GET_Y_LPARAM(lParam)
			);

			int manhattanDist = abs(GET_X_LPARAM(lParam) - m_clickedPosClientX) + abs(GET_Y_LPARAM(lParam) - m_clickedPosClientY);
			bool isDrag = (manhattanDist > dragThresholdDist);
			MessageBoxW(m_hWnd, messageBuffer, isDrag ? L"좌측 마우스 드래그" : L"좌측 마우스 클릭", MB_OK);

			m_isClicked = false;

			return 0;
		}

		case WM_RBUTTONDOWN:
		{
			if (m_isClicked == true)
			{
				return 0;
			}

			m_clickedPosClientX = GET_X_LPARAM(lParam);
			m_clickedPosClientY = GET_Y_LPARAM(lParam);
			m_isClicked = true;

			return 0;
		}

		//TODO :	idle 루프에서 아래 로직을 수행할 수 있도록 필요한 정보를 전달해주기만 하자
		//			-클릭은 레이 케스팅으로 가장 가까운 지면 지점을 선택하고, 드래그는 해당 드래그 시작 점으로부터 도착 점으로의 방향 벡터를 구하도록 하자
		//			-SHIFT가 같이 눌려있다면, 이전의 클릭 혹은 드래그로 이미 내려진 이동 혹은 작업 명령들을 유지하며 새로운 명령을 큐잉하도록 하자
		case WM_RBUTTONUP:
		{
			wchar_t messageBuffer[64] = { };
			swprintf_s(
				messageBuffer,
				_countof(messageBuffer),
				L"%s start (x, y) = (%d, %d) \n end (x, y) = (%d, %d)",
				wParam & MK_SHIFT ? L"[SHIFT]" : L"",
				m_clickedPosClientX,
				m_clickedPosClientY,
				GET_X_LPARAM(lParam),
				GET_Y_LPARAM(lParam)
			);

			int manhattanDist = abs(GET_X_LPARAM(lParam) - m_clickedPosClientX) + abs(GET_Y_LPARAM(lParam) - m_clickedPosClientY);
			bool isDrag = (manhattanDist > dragThresholdDist);
			MessageBoxW(m_hWnd, messageBuffer, isDrag ? L"우측 마우스 드래그" : L"우측 마우스 클릭", MB_OK);

			m_isClicked = false;

			return 0;
		}

		case WM_MBUTTONDOWN:
		{
			if (m_isClicked == true)
			{
				return 0;
			}

			m_clickedPosClientX = GET_X_LPARAM(lParam);
			m_clickedPosClientY = GET_Y_LPARAM(lParam);
			m_isClicked = true;

			return 0;
		}

		//TODO :	idle 루프에서 아래 로직을 수행할 수 있도록 필요한 정보를 전달해주기만 하자
		//			-중간 버튼을 누른 채 드래그시, 시점을 회전하도록 함
		case WM_MBUTTONUP:
		{
			wchar_t messageBuffer[64] = { };
			swprintf_s(
				messageBuffer,
				_countof(messageBuffer),
				L"start (x, y) = (%d, %d) \n end (x, y) = (%d, %d)",
				m_clickedPosClientX,
				m_clickedPosClientY,
				GET_X_LPARAM(lParam),
				GET_Y_LPARAM(lParam)
			);

			int manhattanDist = abs(GET_X_LPARAM(lParam) - m_clickedPosClientX) + abs(GET_Y_LPARAM(lParam) - m_clickedPosClientY);
			bool isDrag = (manhattanDist > dragThresholdDist);
			MessageBoxW(m_hWnd, messageBuffer, isDrag ? L"중간 마우스 드래그" : L"중간 마우스 클릭", MB_OK);

			m_isClicked = false;

			return 0;
		}

		//TODO :	idle 루프에서 아래 로직을 수행할 수 있도록 필요한 정보를 전달해주기만 하자
		//			-마우스 휠 스크롤시 현재 카메라가 바라보는 방향으로 카메라를 전진 혹은 후진하도록 함
		case WM_MOUSEWHEEL:
		{
			m_scrollDelta += GET_WHEEL_DELTA_WPARAM(wParam);

			return 0;
		}

		//NOTE : ESC를 이용한 종료를 처리함
		//TODO : 후진 이동 명령, 수리, 능력 사용 등의 단축키 입력을 감지해서 idle 루프에 전달해주기만 하자 
		case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE)
			{
				if (MessageBoxW(m_hWnd, L"ESC가 눌렸습니다. 어플리케이션을 종료합니까?", L"종료 문구", MB_OKCANCEL) == IDOK)
				{
					DestroyWindow(m_hWnd);
				}
			}

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
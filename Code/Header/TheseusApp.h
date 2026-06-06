#pragma once
#include <windows.h>
#include "BaseApp.h"
#include "Timer.h"

class TheseusApp : public BaseApp<TheseusApp>
{
	//SPECIFIER : 부모 클래스의 WindowProcedure 정적 메소드에서 본 자식 클래스의 HandleMessage를 직접 호출할 수 있도록 친구 선언을 해줌
	friend BaseApp<TheseusApp>;

public:

	TheseusApp() : BaseApp(L"Theseus At Last")
	{
		m_isClicked = false;

		m_mousePosClientX = 0;
		m_mousePosClientY = 0;

		m_clickedPosClientX = 0;
		m_clickedPosClientY = 0;

		m_scrollDelta = 0;
	}

	~TheseusApp()
	{

	}

	void RunMessageLoop()
	{
		MSG msg = { };

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//TODO : 여기서 idle 로직을 수행하기
				m_timer.Update();
				PrintCaption();
			}
		}
	}

private:

	void PrintCaption();

	//NOTE : 이 메소드는 BaseApp에 작석된 WindowProc 정적 메소드에서 호출함
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

	const int dragThresholdDist = 20;

	bool m_isClicked;

	int m_mousePosClientX;
	int m_mousePosClientY;

	int m_clickedPosClientX;
	int m_clickedPosClientY;

	int m_scrollDelta;

	Timer m_timer;
};
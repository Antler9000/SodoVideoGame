#include <windows.h>
#include "BaseApp.h"
#include "Timer.h"

class TheseusApp : public BaseApp<TheseusApp>
{
	//Note : WindowProc static 메소드에서 본 클래스의 HandleMessage를 직접 호출하려면 친구 선언을 해줘야 함
	friend BaseApp<TheseusApp>;

public:

	TheseusApp() : BaseApp()
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
				PrintCaptionForDebug();
			}
		}
	}

private:

	void PrintCaptionForDebug();

	//NOTE :	이 메소드는 BaseApp에 작석된 static WindowProc에서 호출함
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

	static constexpr int dragThresholdDist = 20;

private:

	bool m_isClicked;

	int m_mousePosClientX;
	int m_mousePosClientY;

	int m_clickedPosClientX;
	int m_clickedPosClientY;

	int m_scrollDelta;

	Timer m_timer;
};
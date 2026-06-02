#include <windows.h>	//Win32 API 사용

template <class DerievedApp>
class TheseusBaseApp
{
	//NOTE :	static 메소드는 객체에 얽힌 메소드가 아니기에 this가 없으므로, 멤버 변수에 접근할 수가 없다는 문제가 생김
	//			이를 해결하기 위해 CreatWindowEx의 마지막 매개변수, SetWindowLongPtr 함수, GetWindowLongPtr 함수를 이용함
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DerievedApp* pThis = nullptr;
		
		if (uMsg == WM_CREATE)
		{
			CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
			pThis = reinterpret_cast<DerievedApp*>(pCreateStruct->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
		}
		else
		{
			LONG_PTR pUserData = GetWindowLongPtr(hWnd, GWLP_USERDATA);
			pThis = reinterpret_cast<DerievedApp*>(pUserData);
		}

		if (pThis != nullptr)
		{
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

public:

	TheseusBaseApp() : m_isCreated(false), m_hInstance(NULL), m_hWnd(NULL)
	{

	}

	//RETURN : 창 생성 실패시 false를 반환함
	bool Create(HINSTANCE hInstance, int show)
	{
		if (m_isCreated == true)
		{
			return false;
		}

		m_hInstance = hInstance;

		WNDCLASS wc = { };
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = TheseusBaseApp::WindowProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = m_wndClassName;

		if (RegisterClass(&wc) == false)
		{
			MessageBoxW(0, L"RegisterClass 실패함", L"에러", 0);

			return false;
		}

		m_hWnd = CreateWindowEx(
			0,
			m_wndClassName,
			m_appName,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL,
			NULL,
			hInstance,
			(LPVOID)this	//NOTE : WindowProc에서 이 객체에 접근할 수 있도록 하기 위한 단계 중 하나임
		);

		if (m_hWnd == NULL)
		{
			MessageBoxW(0, L"CreateWindow 실패함", L"에러", 0);

			return false;
		}

		m_isCreated = true;

		ShowWindow(m_hWnd, show);

		return true;
	}

protected:

	bool			m_isCreated;
	HINSTANCE		m_hInstance;
	HWND			m_hWnd;
	const wchar_t*	m_wndClassName	= L"Window Class";
	const wchar_t*	m_appName		= L"Theseus At Last";
};

class TheseusApp : public TheseusBaseApp<TheseusApp>
{
	//Note : WindowProc static 메소드에서 본 클래스의 HandleMessage를 직접 호출하려면 친구 선언을 해줘야 함
	friend TheseusBaseApp<TheseusApp>;

public:

	void RunMessageLoop()
	{
		MSG msg = { };

		while (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

private:

	//MEMO :	WindowProc이 수행 중에는 해당 스레드의 메시지 큐에 있는 다른 메시지를 처리하지 못하므로,
	//			되도록 WindowProc 안에서는 짧은 로직만 수행하도록 하고, 긴 대기가 필요한 로직은 별도 스레드로 처리하자
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
			MessageBoxW(NULL, L"좌측 마우스 버튼 눌림", L"입력 확인", MB_OK);

			return 0;

		case WM_RBUTTONDOWN:
			MessageBoxW(NULL, L"우측 마우스 버튼 눌림", L"입력 확인", MB_OK);

			return 0;

		//NOTE : ESC를 이용한 종료를 처리
		//TODO : 키 눌림은 나중에 GetAsyncKeyState를 이용하도록 교체해야함
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE)
			{
				if (MessageBoxW(NULL, L"ESC가 눌렸습니다. 어플리케이션을 종료합니까?", L"안내 문구", MB_OKCANCEL) == IDOK)
				{
					DestroyWindow(m_hWnd);
				}
			}

			return 0;

		//NOTE : ALT+F4 혹은 우상단 창닫기 버튼을 이용한 종료를 처리
		case WM_CLOSE:
			if (MessageBoxW(NULL, L"어플리케이션을 종료합니까?", L"안내 문구", MB_OKCANCEL) == IDOK)
			{
				DestroyWindow(m_hWnd);
			}

			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);

			return 0;
		}

		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
};

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(pCmdLine);

	TheseusApp appInstance;
	
	if (appInstance.Create(hInstance, nCmdShow) == false)
	{
		return 0;
	}

	appInstance.RunMessageLoop();

	return 0;
}
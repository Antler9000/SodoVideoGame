#pragma once
#include <windows.h>

//NOTE : WindowProc에서 생성할 pThis 객체의 클래스가 자식 클래스일 수 있도록 템플릿 타입 매개변수를 사용함
template <class DerievedApp>
class BaseApp
{
	//NOTE :	static 메소드는 객체에 얽힌 메소드가 아니기에 this가 없으므로, 멤버 변수에 접근할 수가 없다는 문제가 생김
	//			이를 해결하기 위해 CreatWindowEx의 마지막 매개변수, SetWindowLongPtr 함수, GetWindowLongPtr 함수를 이용함
	static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

	BaseApp(const wchar_t* pAppName) : m_pAppName(pAppName), m_isCreated(false), m_hInstance(NULL), m_hWnd(NULL)
	{

	}

	~BaseApp()
	{

	}

	//NOTE : 기본 생성자, 앱 객체의 복사 및 이동을 허용하지 않도록 한다
	BaseApp() = delete;
	BaseApp(const BaseApp& sourceApp) = delete;
	BaseApp(BaseApp&& sourceApp) noexcept = delete;
	BaseApp& operator = (const BaseApp& sourceApp) = delete;
	BaseApp& operator = (BaseApp&& sourceApp) = delete;

	//RETURN : 창 생성 실패시 false를 반환함
	bool Create(HINSTANCE hInstance, int show)
	{
		if (m_isCreated == true)
		{
			return false;
		}

		m_hInstance = hInstance;

		const wchar_t* m_pClassName = L"Window Class";

		WNDCLASS wc = { };
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = BaseApp::WindowProcedure;
		wc.hInstance = hInstance;
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszClassName = m_pClassName;

		if (RegisterClass(&wc) == false)
		{
			MessageBoxW(0, L"RegisterClass 실패함", L"에러", 0);

			return false;
		}

		m_hWnd = CreateWindowEx(
			0,
			m_pClassName,
			m_pAppName,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL,
			NULL,
			hInstance,
			(LPVOID)this	//NOTE : WindowProcedure에서 이 객체에 접근할 수 있도록 하기 위한 단계 중 하나임
		);

		if (m_hWnd == NULL)
		{
			MessageBoxW(0, L"CreateWindow 실패함", L"에러", 0);

			return false;
		}

		m_isCreated = true;

		ShowWindow(m_hWnd, show);
		UpdateWindow(m_hWnd);

		return true;
	}

protected:
	
	const wchar_t*	m_pAppName;

	bool			m_isCreated;
	
	HINSTANCE		m_hInstance;
	HWND			m_hWnd;
};
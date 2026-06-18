#pragma once
#include <windows.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "BaseApp.h"
#include "Timer.h"

class SodoApp : public BaseApp<SodoApp>
{
	//NOTE : 부모 클래스의 WindowProcedure 정적 메소드에서 본 자식 클래스의 HandleMessage를 직접 호출할 수 있도록 친구 선언을 해줌
	friend BaseApp<SodoApp>;

public:

	SodoApp() :
		BaseApp(L"Sodo Video Game"),
		m_dxgiFactory(nullptr),
		m_backBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB),
		m_mousePosClient(0, 0),
		m_clickedPosClient(0, 0),
		m_isClicked(false),
		m_scrollDelta(0)
	{

	}

	~SodoApp()
	{

	}

	//RETURN : 초기화에 실패할 시 false를 반환함
	bool InitApp()
	{
		//TODO : 어댑터 나열을 위해 작성한 바로 아래의 임시 factory 생성 코드를 점검하자
		UINT dxgiFactoryFlags = 0;
		CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_dxgiFactory));

		PrintAdapterInfo();

		return true;
	}

	int RunMessageLoop()
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
				m_timer.Update();
				UpdateCaption();
			}
		}

		return (int)msg.wParam;
	}

private:

	void PrintAdapterInfo() const;

	void PrintOutputInfo(IDXGIAdapter* dxgiAdapter) const;

	void PrintDisplayInfo(IDXGIOutput* dxgiOutput) const;

	void UpdateCaption();

	//NOTE : 이 메소드는 BaseApp에 작석된 WindowProcedure 정적 메소드에서 호출함
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

	const int dragThresholdDist = 20;

private:

	Microsoft::WRL::ComPtr<IDXGIFactory> m_dxgiFactory;
	DXGI_FORMAT m_backBufferFormat;

	POINT m_mousePosClient;
	POINT m_clickedPosClient;
	bool m_isClicked;
	int m_scrollDelta;

	Timer m_timer;
};
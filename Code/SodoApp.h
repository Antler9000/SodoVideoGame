#pragma once
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <stdexcept>
#include "BaseApp.h"
#include "Timer.h"

inline void ThrowIfFailed(HRESULT hResult)
{
	if (FAILED(hResult))
	{
		throw std::runtime_error("HRESULT Failed");
	}
}

class SodoApp : public BaseApp<SodoApp>
{
	//NOTE : 부모 클래스의 WindowProcedure 정적 메소드에서 본 자식 클래스의 HandleMessage를 직접 호출할 수 있도록 친구 선언을 해줌
	friend BaseApp<SodoApp>;

public:

	SodoApp() :
		BaseApp(L"Sodo Video Game"),
		m_deviceSupportRayTracing(false),
		m_featureSupportRayTracing(false),
		m_dxgiFactory(nullptr),
		m_dxgiAdapter(nullptr),
		m_d3d12Device(nullptr),
		m_d3d12Device5(nullptr),
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
		CreateDevice();

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

	void CreateDevice();

	void PrintAdapterInfo() const;

	void PrintOutputInfo(IDXGIAdapter* dxgiAdapter) const;

	void PrintDisplayInfo(IDXGIOutput* dxgiOutput) const;

	void UpdateCaption();

	//NOTE : 이 메소드는 BaseApp에 작석된 WindowProcedure 정적 메소드에서 호출함
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

	const int dragThresholdDist = 20;

private:

	bool m_deviceSupportRayTracing;
	bool m_featureSupportRayTracing;
	Microsoft::WRL::ComPtr<IDXGIFactory6> m_dxgiFactory;		//NOTE : EnumAdapterByGpuPreference(..)를 위해 6 버전 기본 사용
	Microsoft::WRL::ComPtr<IDXGIAdapter3> m_dxgiAdapter;		//NOTE : 자원의 메모리 상주성 관리를 위해 3 버전 기본 사용
	Microsoft::WRL::ComPtr<ID3D12Device> m_d3d12Device;			//NOTE : 메인 그래픽 기능을 위해 0 버전 기본 사용
	Microsoft::WRL::ComPtr<ID3D12Device5> m_d3d12Device5;		//NOTE : 레이 트레이싱 옵션 기능을 위해 5 버전 추가 사용
	DXGI_FORMAT m_backBufferFormat;

	POINT m_mousePosClient;
	POINT m_clickedPosClient;
	bool m_isClicked;
	int m_scrollDelta;

	Timer m_timer;
};
#pragma once
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgi1_4.h>
#include <dxgitype.h>
#include <dxgiformat.h>
#include <wrl/client.h>
#include "BaseApp.h"
#include "Timer.h"

class SodoApp : public BaseApp<SodoApp>
{
	//NOTE : 부모 클래스의 WindowProcedure 정적 메소드에서 본 자식 클래스의 HandleMessage를 직접 호출할 수 있도록 친구 선언을 해줌
	friend BaseApp<SodoApp>;

public:

	SodoApp() :
		BaseApp(L"Sodo Video Game"),
		m_bDeviceSupportRayTracing(false),
		m_bFeatureSupportRayTracing(false),
		m_bCommandListSupportRayTracing(false),
		m_bDeviceSupportMeshShader(false),
		m_bFeatureSupportMeshShader(false),
		m_bCommandListSupportMeshShader(false),
		m_dxgiFactory(nullptr),
		m_dxgiAdapter(nullptr),
		m_dxgiOutput(nullptr),
		m_d3d12Device(nullptr),
		m_d3d12Device5(nullptr),
		m_d3d12Fence(nullptr),
		m_d3d12CommandQueue(nullptr),
		m_d3d12CommandAllocator(nullptr),
		m_d3d12CommandList(nullptr),
		m_backBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM),
		m_displayModeDesc(),
		m_mousePositionClient(0, 0),
		m_clickedPositionClient(0, 0),
		m_bClicked(false),
		m_scrollDelta(0)
	{

	}

	~SodoApp()
	{

	}

	void InitializeApp()
	{
		InitializeFactory();
		InitializeAdapter();
		InitializeOutput();
		InitializeDisplayMode();
		InitializeDevice();
		InitializeFence();
		InitializeCommandQueue();
		InitializeCommandAllocator();
		InitializeCommandList();
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
				UpdateTimer();
				UpdateCaption();
			}
		}

		return (int)msg.wParam;
	}

private:

	void InitializeFactory();
	void InitializeAdapter();
	void InitializeOutput();
	void InitializeDisplayMode();
	void InitializeDevice();
	void InitializeFence();
	void InitializeCommandQueue();
	void InitializeCommandAllocator();
	void InitializeCommandList();

	void UpdateTimer();
	void UpdateCaption();

	//NOTE : 이 메소드는 BaseApp에 작석된 WindowProcedure 정적 메소드에서 호출함
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void InputMouseMove(WPARAM wParam, LPARAM lParam);
	void InputMouseLeftButtonDown(WPARAM wParam, LPARAM lParam);
	void InputMouseLeftButtonUp(WPARAM wParam, LPARAM lParam);
	void InputMouseRightButtonDown(WPARAM wParam, LPARAM lParam);
	void InputMouseRightButtonUp(WPARAM wParam, LPARAM lParam);
	void InputMouseMiddleButtonDown(WPARAM wParam, LPARAM lParam);
	void InputMouseMiddleButtonUp(WPARAM wParam, LPARAM lParam);
	void InputMouseWheelScroll(WPARAM wParam, LPARAM lParam);
	void InputKeyboardDown(WPARAM wParam, LPARAM lParam);

private:

	const int dragThresholdDist = 20;

private:

	bool m_bDeviceSupportRayTracing;
	bool m_bFeatureSupportRayTracing;
	bool m_bCommandListSupportRayTracing;
	bool m_bDeviceSupportMeshShader;
	bool m_bFeatureSupportMeshShader;
	bool m_bCommandListSupportMeshShader;
	Microsoft::WRL::ComPtr<IDXGIFactory6>				m_dxgiFactory;				//NOTE : 성능순 어댑터 획득을 위함
	Microsoft::WRL::ComPtr<IDXGIAdapter3>				m_dxgiAdapter;				//NOTE : 자원의 메모리 상주성 관리를 위함
	Microsoft::WRL::ComPtr<IDXGIOutput6>				m_dxgiOutput;				//NOTE : HDR 모니터 정보 획득을 위함
	Microsoft::WRL::ComPtr<ID3D12Device>				m_d3d12Device;
	Microsoft::WRL::ComPtr<ID3D12Device2>				m_d3d12Device2;				//NOTE : 메시 셰이더 옵션을 위함
	Microsoft::WRL::ComPtr<ID3D12Device5>				m_d3d12Device5;				//NOTE : 레이 트레이싱 옵션을 위함
	Microsoft::WRL::ComPtr<ID3D12Fence>					m_d3d12Fence;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue>			m_d3d12CommandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>		m_d3d12CommandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	m_d3d12CommandList;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4>	m_d3d12CommandList4;		//NOTE : 레이 트레이싱 옵션을 위함
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6>	m_d3d12CommandList6;		//NOTE : 메시 셰이더 옵션을 위함
	DXGI_FORMAT		m_backBufferFormat;
	DXGI_MODE_DESC	m_displayModeDesc;

	Timer m_timer;

	POINT m_mousePositionClient;
	POINT m_clickedPositionClient;
	bool m_bClicked;
	int m_scrollDelta;
};
#pragma once
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgi1_5.h>
#include <dxgi1_4.h>
#include <dxgi1_2.h>
#include <dxgi.h>
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
		m_bOutputSupportHDR(false),
		m_bFormatSupportHDR(false),
		m_bSwapChainSupportHDR(false),
		m_bColorSpaceSupportHDR(false),
		m_bUserEnabledHDR(false),
		m_bFeatureSupportTearing(false),
		m_bUserEnabledTearing(false),
		m_bDeviceSupportRayTracing(false),
		m_bFeatureSupportRayTracing(false),
		m_bCommandListSupportRayTracing(false),
		m_bUserEnabledRayTracing(false),
		m_bDeviceSupportMeshShader(false),
		m_bFeatureSupportMeshShader(false),
		m_bCommandListSupportMeshShader(false),
		m_bUserEnabledMeshShader(false),
		m_dxgiFactory(nullptr),
		m_dxgiAdapter(nullptr),
		m_dxgiOutput(nullptr),
		m_d3d12Device(nullptr),
		m_d3d12Device5(nullptr),
		m_d3d12Fence(nullptr),
		m_d3d12CommandQueue(nullptr),
		m_d3d12CommandAllocator(nullptr),
		m_d3d12CommandList(nullptr),
		m_outputDesc(),
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
		InitializeSwapChain();			//NOTE :	HDR 색공간 제시 가능 여부가 스왑 체인의 생성 후에나 알 수 있어서,
		InitializeSwapChain();			//			해당 정보를 통해 HDR 활성화 여부를 결정 후 HDR 포맷으로 스왑 체인을 재생성함
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
	void InitializeSwapChain();

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

	bool isSupportHDR() 
	{ 
		return m_bOutputSupportHDR && m_bFormatSupportHDR && m_bSwapChainSupportHDR && m_bColorSpaceSupportHDR && m_bUserEnabledHDR;
	}
	bool isSupportTearing()
	{
		return m_bFeatureSupportTearing && m_bUserEnabledTearing;
	}
	bool isSupportRayTracing()
	{
		return m_bDeviceSupportRayTracing && m_bFeatureSupportRayTracing && m_bCommandListSupportRayTracing && m_bUserEnabledRayTracing;
	}
	bool isSupportMeshShader()
	{
		return m_bDeviceSupportMeshShader && m_bFeatureSupportMeshShader && m_bCommandListSupportMeshShader && m_bUserEnabledMeshShader;
	}

private:

	const int m_backBufferCount = 2;	//TODO : 3이랑 둘 중 선택하기
	const DXGI_FORMAT m_backBufferFormatSDR	= DXGI_FORMAT_R8G8B8A8_UNORM;
	const DXGI_FORMAT m_backBufferFormatHDR	= DXGI_FORMAT_R16G16B16A16_FLOAT;	//TODO : DXGI_FORMAT_R10G10B10A2_UNORM이랑 둘 중 선택하기
	const DXGI_COLOR_SPACE_TYPE	m_backBufferColorSpaceSDR = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;
	const DXGI_COLOR_SPACE_TYPE	m_backBufferColorSpaceHDR = DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;	//TODO : 위와 같이 DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020 둘 중 선택하기

	const int dragThresholdDist = 20;

private:

	bool m_bOutputSupportHDR;
	bool m_bFormatSupportHDR;
	bool m_bSwapChainSupportHDR;
	bool m_bColorSpaceSupportHDR;
	bool m_bUserEnabledHDR;
	bool m_bFeatureSupportTearing;
	bool m_bUserEnabledTearing;
	bool m_bDeviceSupportRayTracing;
	bool m_bFeatureSupportRayTracing;
	bool m_bCommandListSupportRayTracing;
	bool m_bUserEnabledRayTracing;
	bool m_bDeviceSupportMeshShader;
	bool m_bFeatureSupportMeshShader;
	bool m_bCommandListSupportMeshShader;
	bool m_bUserEnabledMeshShader;
	Microsoft::WRL::ComPtr<IDXGIFactory6>				m_dxgiFactory;				//NOTE : (기본) 성능순 어댑터 획득
	Microsoft::WRL::ComPtr<IDXGIAdapter3>				m_dxgiAdapter;				//NOTE : (기본) 자원의 메모리 상주성 관리
	Microsoft::WRL::ComPtr<IDXGIOutput>					m_dxgiOutput;
	Microsoft::WRL::ComPtr<IDXGIOutput6>				m_dxgiOutput6;				//NOTE : (옵션) HDR 모니터 정보 획득
	Microsoft::WRL::ComPtr<ID3D12Device>				m_d3d12Device;
	Microsoft::WRL::ComPtr<ID3D12Device2>				m_d3d12Device2;				//NOTE : (옵션) 메시 셰이더
	Microsoft::WRL::ComPtr<ID3D12Device5>				m_d3d12Device5;				//NOTE : (옵션) 레이 트레이싱
	Microsoft::WRL::ComPtr<ID3D12Fence>					m_d3d12Fence;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue>			m_d3d12CommandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>		m_d3d12CommandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	m_d3d12CommandList;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4>	m_d3d12CommandList4;		//NOTE : (옵션) 레이 트레이싱
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6>	m_d3d12CommandList6;		//NOTE : (옵션) 메시 셰이더
	Microsoft::WRL::ComPtr<IDXGISwapChain1>				m_dxgiSwapChain;			//NOTE : (기본) HWND 타겟 스왑 체인 생성
	Microsoft::WRL::ComPtr<IDXGISwapChain3>				m_dxgiSwapChain3;			//NOTE : (옵션) HDR 제시
	DXGI_OUTPUT_DESC1	m_outputDesc;												//NOTE : (옵션) HDR 모니터 정보 획득
	DXGI_MODE_DESC		m_displayModeDesc;

	Timer m_timer;

	POINT m_mousePositionClient;
	POINT m_clickedPositionClient;
	bool m_bClicked;
	int m_scrollDelta;
};
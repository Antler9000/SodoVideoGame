#pragma once
#include <windows.h>
#include <d3dx12_root_signature.h>
#include <d3d12.h>
#include <dxgicommon.h>
#include <dxgi1_6.h>
#include <dxgi1_4.h>
#include <dxgi1_2.h>
#include <dxgi.h>
#include <dxgitype.h>
#include <dxgiformat.h>
#include <wrl/client.h>
#include "BaseApp.h"
#include "Option.h"
#include "Timer.h"

class SodoApp : public BaseApp<SodoApp>
{
	//NOTE : 부모 클래스의 WindowProcedure 정적 메소드에서 본 자식 클래스의 HandleMessage를 직접 호출할 수 있도록 친구 선언을 해줌
	friend BaseApp<SodoApp>;

public:

	SodoApp() : BaseApp(L"Sodo Video Game")
	{

	}

	~SodoApp() = default;

	void InitApp()
	{
		InitFactory();
		InitAdapter();
		InitOutput();
		InitDisplayMode();
		InitDevice();
		InitFence();
		InitCommandQueue();
		InitCommandAllocator();
		InitCommandList();
		InitSDRSwapChain();
		InitHDRSwapChain();
		InitDescriptorHeapCBVSRVUAV();
		InitDescriptorHeapRTV();
		InitDescriptorHeapDSV();

		m_optionHDR.DebugPrint();
		m_optionTearing.DebugPrint();
		m_optionRayTracing.DebugPrint();
		m_optionMeshShader.DebugPrint();
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

	void InitFactory();
	void InitAdapter();
	void InitOutput();
	void InitDisplayMode();
	void InitDevice();
	void InitFence();
	void InitCommandQueue();
	void InitCommandAllocator();
	void InitCommandList();
	void InitSDRSwapChain();
	void InitHDRSwapChain();	
	void InitDescriptorHeapCBVSRVUAV();
	void InitDescriptorHeapRTV();
	void InitDescriptorHeapDSV();

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

	static constexpr UINT m_backBufferCount = 2;															//TODO : 3이랑 둘 중 선택하기
	static constexpr DXGI_FORMAT m_backBufferFormatSDR	= DXGI_FORMAT_R8G8B8A8_UNORM;
	static constexpr DXGI_FORMAT m_backBufferFormatHDR	= DXGI_FORMAT_R16G16B16A16_FLOAT;							//TODO : DXGI_FORMAT_R10G10B10A2_UNORM이랑 둘 중 선택하기
	static constexpr DXGI_COLOR_SPACE_TYPE	m_backBufferColorSpaceSDR = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;
	static constexpr DXGI_COLOR_SPACE_TYPE	m_backBufferColorSpaceHDR = DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;	//TODO : DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020 둘 중 선택하기

	static constexpr UINT m_dragThresholdDist = 20;

private:

	template <typename Interface>
	using ComPtr = Microsoft::WRL::ComPtr<Interface>;

	OptionHDR			m_optionHDR;
	OptionTearing		m_optionTearing;
	OptionRayTracing	m_optionRayTracing;
	OptionMeshShader	m_optionMeshShader;
	ComPtr<IDXGIFactory6>				m_factory					= nullptr;	//NOTE : (기본) 성능순 어댑터 획득
	ComPtr<IDXGIAdapter3>				m_adapter					= nullptr;	//NOTE : (기본) 자원의 메모리 상주성 관리
	ComPtr<IDXGIOutput>					m_output					= nullptr;
	ComPtr<IDXGIOutput6>				m_output6					= nullptr;	//NOTE : (옵션) HDR 모니터 정보 획득
	ComPtr<ID3D12Device>				m_device					= nullptr;
	ComPtr<ID3D12Device2>				m_device2					= nullptr;	//NOTE : (옵션) 메시 셰이더
	ComPtr<ID3D12Device5>				m_device5					= nullptr;	//NOTE : (옵션) 레이 트레이싱
	ComPtr<ID3D12Fence>					m_fence						= nullptr;
	ComPtr<ID3D12CommandQueue>			m_commandQueue				= nullptr;
	ComPtr<ID3D12CommandAllocator>		m_commandAllocator			= nullptr;
	ComPtr<ID3D12GraphicsCommandList>	m_commandList				= nullptr;
	ComPtr<ID3D12GraphicsCommandList4>	m_commandList4				= nullptr;	//NOTE : (옵션) 레이 트레이싱
	ComPtr<ID3D12GraphicsCommandList6>	m_commandList6				= nullptr;	//NOTE : (옵션) 메시 셰이더
	ComPtr<IDXGISwapChain1>				m_swapChain					= nullptr;	//NOTE : (기본) HWND 타겟 스왑 체인 생성
	ComPtr<IDXGISwapChain3>				m_swapChain3				= nullptr;	//NOTE : (옵션) HDR 제시
	ComPtr<ID3D12DescriptorHeap>		m_descriptorHeapCBVSRVUAV	= nullptr;
	ComPtr<ID3D12DescriptorHeap>		m_descriptorHeapRTV			= nullptr;
	ComPtr<ID3D12DescriptorHeap>		m_descriptorHeapDSV			= nullptr;
	DXGI_OUTPUT_DESC1	m_outputDesc				= {};						//NOTE : (옵션) HDR 모니터 정보 획득
	DXGI_MODE_DESC		m_displayModeDesc			= {};
	UINT				m_currentBackBufferIndex	= 0;
	UINT				m_countCBV					= 0;
	UINT				m_countSRV					= 0;
	UINT				m_countUAV					= 0;
	UINT				m_incrementSizeCBVSRVUAV	= 0;
	UINT				m_incrementSizeRTV			= 0;
	UINT				m_incrementSizeDSV			= 0;
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_cpuStartHandleCBVSRVUAV;
	CD3DX12_GPU_DESCRIPTOR_HANDLE m_gpuStartHandleCBVSRVUAV;
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_cpuStartHandleRTV;
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_cpuStartHandleDSV;

	Timer m_timer;

	POINT m_mousePositionClient		= { 0, 0 };
	POINT m_clickedPositionClient	= { 0, 0 };
	bool m_bClicked					= false;
	int m_scrollDelta				= 0;
};
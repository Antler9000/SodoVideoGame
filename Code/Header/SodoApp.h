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

	~SodoApp()
	{
		WaitAllCommandDone();
		CloseFenceEvent();
	}

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
		InitBackBuffers();
		InitViewPort();
		InitScissorRectangle();
		InitDepthStencilBuffer();
		InitDescriptorHeapRTV();
		InitDescriptorHeapDSV();
		InitDescriptorHeapCBVSRVUAV();
		InitRTV();
		InitDSV();
		InitCBVSRVUAV();
		InitFenceEvent();
		InitTimer();

		m_optionSound.DebugPrint();
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
				if (IsStopped() == true)
				{
					Sleep(100);
				}
				else
				{
					UpdateTimers();
					UpdateCaption();
					UpdateSreen();
				}
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
	void InitBackBuffers();
	void InitViewPort();
	void InitScissorRectangle();
	void InitDepthStencilBuffer();
	void InitDescriptorHeapRTV();
	void InitDescriptorHeapDSV();
	void InitDescriptorHeapCBVSRVUAV();
	void InitRTV();
	void InitDSV();
	void InitCBVSRVUAV();
	void InitFenceEvent();
	void InitTimer();

	void UpdateTimers();
	void UpdateCaption();
	void UpdateSreen();

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

	void WaitAllCommandDone();
	void CloseFenceEvent();

private:

	static constexpr UINT m_backBufferCount								= 2;										//TODO : 3으로 바꿀지 결정하자
	static constexpr DXGI_FORMAT m_backBufferFormatSDR					= DXGI_FORMAT_R8G8B8A8_UNORM;
	static constexpr DXGI_FORMAT m_backBufferFormatHDR					= DXGI_FORMAT_R16G16B16A16_FLOAT;			//TODO : DXGI_FORMAT_R10G10B10A2_UNORM으로 바꿀지 결정하자
	static constexpr DXGI_COLOR_SPACE_TYPE	m_backBufferColorSpaceSDR	= DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;
	static constexpr DXGI_COLOR_SPACE_TYPE	m_backBufferColorSpaceHDR	= DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;	//TODO : DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020으로 바꿀지 결정하자 (위와 세트)
	static constexpr DXGI_FORMAT m_depthStencilBufferFormat				= DXGI_FORMAT_D24_UNORM_S8_UINT;

	static constexpr UINT m_dragThresholdDist = 20;

private:

	template <typename Interface>
	using ComPtr = Microsoft::WRL::ComPtr<Interface>;

	OptionSound			m_optionSound;
	OptionHDR			m_optionHDR;
	OptionTearing		m_optionTearing;
	OptionRayTracing	m_optionRayTracing;
	OptionMeshShader	m_optionMeshShader;

	ComPtr<IDXGIFactory6>				m_factory;								//NOTE : (기본) 성능순 어댑터 획득
	ComPtr<IDXGIAdapter3>				m_adapter;								//NOTE : (기본) 자원의 메모리 상주성 관리
	ComPtr<IDXGIOutput>					m_output;
	ComPtr<IDXGIOutput6>				m_output6;								//NOTE : (옵션) HDR 모니터 정보 획득
	ComPtr<ID3D12Device>				m_device;
	ComPtr<ID3D12Device2>				m_device2;								//NOTE : (옵션) 메시 셰이더
	ComPtr<ID3D12Device5>				m_device5;								//NOTE : (옵션) 레이 트레이싱
	ComPtr<ID3D12Fence>					m_fence;
	ComPtr<ID3D12CommandQueue>			m_commandQueue;
	ComPtr<ID3D12CommandAllocator>		m_commandAllocator;
	ComPtr<ID3D12GraphicsCommandList>	m_commandList;
	ComPtr<ID3D12GraphicsCommandList4>	m_commandList4;							//NOTE : (옵션) 레이 트레이싱
	ComPtr<ID3D12GraphicsCommandList6>	m_commandList6;							//NOTE : (옵션) 메시 셰이더 생성
	ComPtr<IDXGISwapChain3>				m_swapChain;							//NOTE : (기본) 백버퍼 인덱스 추적
	ComPtr<ID3D12Resource>				m_backBuffers[m_backBufferCount];
	ComPtr<ID3D12Resource>				m_depthStencilBuffer;
	ComPtr<ID3D12DescriptorHeap>		m_descriptorHeapCBVSRVUAV;
	ComPtr<ID3D12DescriptorHeap>		m_descriptorHeapRTV;
	ComPtr<ID3D12DescriptorHeap>		m_descriptorHeapDSV;
	DXGI_OUTPUT_DESC1	m_outputDesc				= {};						//NOTE : (옵션) HDR 모니터 정보 획득
	DXGI_MODE_DESC		m_displayModeDesc			= {};
	UINT64				m_currentFence				= 0;
	UINT				m_currentBackBufferIndex	= 0;
	UINT				m_backBufferWidth			= 0;
	UINT				m_backBufferHeight			= 0;
	float				m_backBufferAspectRatio		= 0.0f;
	D3D12_VIEWPORT		m_viewPort					= {};
	D3D12_RECT			m_scissorRectangle			= {};
	UINT				m_countCBV					= 0;
	UINT				m_countSRV					= 0;
	UINT				m_countUAV					= 0;
	UINT				m_incrementSizeRTV			= 0;
	UINT				m_incrementSizeDSV			= 0;
	UINT				m_incrementSizeCBVSRVUAV	= 0;
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_cpuStartHandleRTV;
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_cpuStartHandleDSV;
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_cpuStartHandleCBVSRVUAV;
	CD3DX12_GPU_DESCRIPTOR_HANDLE m_gpuStartHandleCBVSRVUAV;
	 
	bool IsStopped() { return m_isResizing || m_isInActive; }
	bool m_isResizing = false;
	bool m_isInActive = false;
	void ResetTimers() { m_totalTimer.Reset(); m_captionTimer.Reset(); m_frameTimer.Reset(); }
	void StopTimers() { if (IsStopped() == true) { m_totalTimer.Stop(); m_captionTimer.Stop(); m_frameTimer.Stop(); } }
	void StartTimers() { if (IsStopped() == false) { m_totalTimer.Start(); m_captionTimer.Start(); m_frameTimer.Start(); } }
	Timer m_totalTimer;
	Timer m_captionTimer;
	Timer m_frameTimer;
	HANDLE m_fenceEvent = nullptr;

	POINT m_mousePositionClient		= { 0, 0 };
	POINT m_clickedPositionClient	= { 0, 0 };
	bool m_bClicked					= false;
	int m_scrollDelta				= 0;
};
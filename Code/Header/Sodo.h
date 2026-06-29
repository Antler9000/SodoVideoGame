#pragma once
#include <windows.h>
#include <d3dx12_root_signature.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgi1_4.h>
#include <dxgi.h>
#include <dxgicommon.h>
#include <dxgitype.h>
#include <dxgiformat.h>
#include <wrl/client.h>
#include "imgui.h"
#include "BaseApp.h"
#include "Game.h"
#include "Option.h"
#include "Timer.h"
#include "Alloc.h"

class Sodo : public BaseApp<Sodo>
{
	//NOTE : 부모 클래스의 WindowProcedure 정적 메소드에서 본 자식 클래스의 HandleMessage를 직접 호출할 수 있도록 친구 선언을 해줌
	friend BaseApp<Sodo>;

public:

	Sodo() : BaseApp(L"Sodo Video Game")
	{

	}

	~Sodo()
	{
		WaitAllCommandDone();

		CloseFenceEvent();
		CloseImGui();
	}

	void InitApp()
	{
		InitSavedOptions();
		InitScreenMode();
		InitFactory();
		InitAdapter();
		InitOutput();
		InitDisplayMode();
		InitDevice();
		InitFormatSupport();
		InitFence();
		InitCommandQueue();
		InitCommandAllocator();
		InitCommandList();
		InitHDRSwapChainSupport();
		InitSwapChain();
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
		InitImGui();
		InitTimer();

		m_optionFullScreen.DebugPrint();
		m_optionHDR.DebugPrint();
		m_optionTearing.DebugPrint();
		m_optionRayTracing.DebugPrint();
		m_optionMeshShader.DebugPrint();
		m_optionSound.DebugPrint();
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
			else if (m_needResetScreenMode || m_needResetHDR)
			{
				ResetScreenSetting();
			}
			else
			{
				if (IsStopped())
				{
					Sleep(100);
				}
				else
				{
					UpdateTimers();
					UpdateCaption();
					UpdateImGui();
					UpdateSreen();
				}
			}
		}

		return (int)msg.wParam;
	}

private:

	void CloseFenceEvent();
	void CloseImGui();

	void InitSavedOptions();
	void InitScreenMode();
	void InitFactory();
	void InitAdapter();
	void InitOutput();
	void InitDisplayMode();
	void InitDevice();
	void InitFormatSupport();
	void InitFence();
	void InitCommandQueue();
	void InitCommandAllocator();
	void InitCommandList();
	void InitHDRSwapChainSupport();
	void InitSwapChain();
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
	void InitImGui();
	void InitTimer();

	void UpdateTimers();
	void UpdateCaption();
	void UpdateImGui();
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
	void ResetScreenSetting();
	void SetFullScreenMode();
	void SetWindowMode();
	void SaveOptions();

	void RenderGuiInGame(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos);
	void RenderGuiLobbyMenu(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos);
	void RenderGuiPausedMenu(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos);
	void RenderGuiLoadingToGame(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos);
	void RenderGuiLoadingToLobby(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos);
	void RenderGuiOptionFromLobby(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos);
	void RenderGuiOptionFromPaused(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos);
	void RenderGuiCheckExitFromLobbyToWindows(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos);
	void RenderGuiCheckExitFromPausedToWindows(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos);
	void RenderGuiCheckExitFromPausedToLobby(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos);
	void CommonRenderGuiOption();
	void CommonRenderGuiCheckExit(GameState from, GameState to);

public:

	//NOTE : ImGui의 버튼, 체크 박스 등이 수정할 수 있어야 하므로 static으로 둠
	static inline OptionFullScreen	m_optionFullScreen;
	static inline OptionHDR			m_optionHDR;
	static inline OptionTearing		m_optionTearing;
	static inline OptionRayTracing	m_optionRayTracing;
	static inline OptionMeshShader	m_optionMeshShader;
	static inline OptionSound		m_optionSound;

	//NOTE : ImGui에 넘겨주는 콜백 함수 속에서 기능해야 하므로 static으로 둠
	static inline ImGuiDescriptorHeapAllocator m_imGuiDescriptorHeapAllocator = {};

private:
	static constexpr UINT m_backBufferCount								= 2;
	static constexpr DXGI_FORMAT m_backBufferFormatSDR					= DXGI_FORMAT_R8G8B8A8_UNORM;
	static constexpr DXGI_FORMAT m_backBufferFormatHDR					= DXGI_FORMAT_R16G16B16A16_FLOAT;
	static constexpr DXGI_COLOR_SPACE_TYPE	m_backBufferColorSpaceSDR	= DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;
	static constexpr DXGI_COLOR_SPACE_TYPE	m_backBufferColorSpaceHDR	= DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;
	static constexpr DXGI_FORMAT m_depthStencilBufferFormat				= DXGI_FORMAT_D24_UNORM_S8_UINT;

	static constexpr UINT m_dragThresholdDist = 20;

	static constexpr float m_imGuiScale						= 2.0f;
	static constexpr ImVec2 m_imGuiSpacingSize				= ImVec2(0.0f, 10.0f);
	static constexpr ImVec2 m_imGuiSmallButtonSize			= ImVec2(120.0f, 40.0f);
	static constexpr ImVec2 m_imGuiMediumButtonSize			= ImVec2(240.0f, 40.0f);
	static constexpr ImVec2 m_imguiLargeButtonSize			= ImVec2(360.0f, 40.0f);
	static constexpr ImGuiWindowFlags m_imGuiBasicFlag		= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;
	static constexpr UINT m_imGuiDescriptorHeapCapacity		= 64;

private:

	template <typename Interface>
	using ComPtr = Microsoft::WRL::ComPtr<Interface>;

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
	bool				m_needResetScreenMode		= false;
	bool				m_needResetHDR				= false;
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
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_cpuStartHandleCBVSRVUAVForImGui;
	CD3DX12_GPU_DESCRIPTOR_HANDLE m_gpuStartHandleCBVSRVUAVForImGui;
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_cpuStartHandleCBVSRVUAVForGame;
	CD3DX12_GPU_DESCRIPTOR_HANDLE m_gpuStartHandleCBVSRVUAVForGame;
	
	bool IsStopped() const { return m_isResizing || m_isInActive; }
	bool m_isResizing = false;
	bool m_isInActive = false;
	void ResetTimers() { m_totalTimer.Reset(); m_captionTimer.Reset(); m_frameTimer.Reset(); }
	void StopTimers() { if (IsStopped()) { m_totalTimer.Stop(); m_captionTimer.Stop(); m_frameTimer.Stop(); } }
	void StartTimers() { if (IsStopped() == false) { m_totalTimer.Start(); m_captionTimer.Start(); m_frameTimer.Start(); } }
	Timer m_totalTimer;
	Timer m_captionTimer;
	Timer m_frameTimer;
	HANDLE m_fenceEvent = nullptr;
	GameState m_gameMode = GAME_STATE_LOBBY;

	POINT m_mousePositionClient		= { 0, 0 };
	POINT m_clickedPositionClient	= { 0, 0 };
	bool m_isClicked				= false;
	int m_scrollDelta				= 0;

	bool m_imGuiInitialized = false;
};
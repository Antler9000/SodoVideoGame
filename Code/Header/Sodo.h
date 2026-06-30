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
#include <string>
#include <fstream>
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
		InitFactory();
		InitAdapter();
		InitOutput();
		InitDisplayMode();
		InitDevice();
		InitFence();
		InitFenceEvent();
		InitCommandQueue();
		InitCommandAllocator();
		InitCommandList();
		InitFormatSupport();
		InitHDRSwapChainSupport();
		InitSavedOptions();
		InitScreenMode();
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
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else if (NeedResetScreenSetting() == true)
			{
				ResetScreenSetting();
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
	void InitFence();
	void InitFenceEvent();
	void InitCommandQueue();
	void InitCommandAllocator();
	void InitCommandList();
	void InitFormatSupport();
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
	void RestoreOptions();
	bool ReadOptionBool(std::ifstream& fin, std::string optionName, bool& outOptionEnabled);
	bool ReadOptionInt(std::ifstream& fin, std::string optionName, int& outOptionEnabled);

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
	void RenderGuiOptionCommon();
	void RenderGuiCheckExitCommon(GameState from, GameState to);

public:

	//NOTE : ImGui의 버튼, 체크 박스 등이 수정할 수 있어야 하므로 static으로 둠
	static inline OptionFullScreen				m_optionFullScreen;
	static inline OptionHDR						m_optionHDR;
	static inline OptionTearing					m_optionTearing;
	static inline OptionRayTracing				m_optionRayTracing;
	static inline OptionMeshShader				m_optionMeshShader;
	static inline OptionSound					m_optionSound;

	//NOTE : ImGui에 넘겨주는 콜백 함수 속에서 기능해야 하므로 static으로 둠
	static inline ImGuiDescriptorHeapAllocator	m_imGuiDescriptorHeapAllocator		= {};

private:
	static constexpr UINT						m_screenBackBufferCount				= 2;
	static constexpr DXGI_FORMAT				m_screenBackBufferFormatSDR			= DXGI_FORMAT_R8G8B8A8_UNORM;
	static constexpr DXGI_FORMAT				m_screenBackBufferFormatHDR			= DXGI_FORMAT_R16G16B16A16_FLOAT;
	static constexpr DXGI_COLOR_SPACE_TYPE		m_screenBackBufferColorSpaceSDR		= DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;
	static constexpr DXGI_COLOR_SPACE_TYPE		m_screenBackBufferColorSpaceHDR		= DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;
	static constexpr DXGI_FORMAT				m_screenDepthStencilBufferFormat	= DXGI_FORMAT_D24_UNORM_S8_UINT;

	static constexpr UINT						m_inputDragThresholdDist			= 20;

	static constexpr float						m_imGuiScale						= 2.0f;
	static constexpr ImVec2						m_imGuiSpacingSize					= ImVec2(0.0f, 10.0f);
	static constexpr ImVec2						m_imGuiSmallButtonSize				= ImVec2(120.0f, 40.0f);
	static constexpr ImVec2						m_imGuiMediumButtonSize				= ImVec2(240.0f, 40.0f);
	static constexpr ImVec2						m_imguiLargeButtonSize				= ImVec2(360.0f, 40.0f);
	static constexpr UINT						m_imGuiDescriptorHeapCapacity		= 64;
	static constexpr ImGuiWindowFlags			m_imGuiBasicFlag					= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
																					| ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;

private:

	template <typename Interface>
	using ComPtr = Microsoft::WRL::ComPtr<Interface>;

	ComPtr<IDXGIFactory6>				m_dxgiFactory;													//NOTE : (기본) 성능순 어댑터 획득
	ComPtr<IDXGIAdapter3>				m_dxgiAdapter;													//NOTE : (기본) 자원의 메모리 상주성 관리
	ComPtr<IDXGIOutput>					m_dxgiOutput;
	ComPtr<IDXGIOutput6>				m_dxgiOutput6;													//NOTE : (옵션) HDR 모니터 정보 획득
	DXGI_OUTPUT_DESC1					m_dxgiOutputDesc									= {};		//NOTE : (옵션) HDR 모니터 정보 획득
	DXGI_MODE_DESC						m_dxgiDisplayModeDesc								= {};
	
	ComPtr<ID3D12Device>				m_device;
	ComPtr<ID3D12Device2>				m_device2;														//NOTE : (옵션) 메시 셰이더
	ComPtr<ID3D12Device5>				m_device5;														//NOTE : (옵션) 레이 트레이싱
	
	ComPtr<ID3D12Fence>					m_fence;
	UINT64								m_fenceCurrent										= 0;
	HANDLE								m_fenceEvent										= nullptr;
	
	ComPtr<ID3D12CommandQueue>			m_commandQueue;
	ComPtr<ID3D12CommandAllocator>		m_commandAllocator;
	ComPtr<ID3D12GraphicsCommandList>	m_commandList;
	ComPtr<ID3D12GraphicsCommandList4>	m_commandList4;													//NOTE : (옵션) 레이 트레이싱
	ComPtr<ID3D12GraphicsCommandList6>	m_commandList6;													//NOTE : (옵션) 메시 셰이더 생성
	
	ComPtr<IDXGISwapChain3>				m_screenSwapChain;												//NOTE : (기본) 백버퍼 인덱스 추적
	ComPtr<ID3D12Resource>				m_screenBackBuffers[m_screenBackBufferCount];
	UINT								m_screenBackBufferIndex								= 0;
	UINT								m_screenBackBufferWidth								= 0;
	UINT								m_screenBackBufferHeight							= 0;
	float								m_screenBackBufferAspectRatio						= 0.0f;
	D3D12_VIEWPORT						m_screenViewPort									= {};
	D3D12_RECT							m_screenScissorRectangle							= {};
	ComPtr<ID3D12Resource>				m_screenDepthStencilBuffer;
	
	UINT								m_descriptorHeapCBVCount							= 0;
	UINT								m_descriptorHeapSRVCount							= 0;
	UINT								m_descriptorHeapUAVCount							= 0;
	UINT								m_descriptorHeapRTVIncrementSize					= 0;
	UINT								m_descriptorHeapDSVIncrementSize					= 0;
	UINT								m_descriptorHeapCBVSRVUAVIncrementSize				= 0;
	ComPtr<ID3D12DescriptorHeap>		m_descriptorHeapRTV;
	ComPtr<ID3D12DescriptorHeap>		m_descriptorHeapDSV;
	ComPtr<ID3D12DescriptorHeap>		m_descriptorHeapCBVSRVUAV;
	CD3DX12_CPU_DESCRIPTOR_HANDLE		m_descriptorHeapRTVCpuStartHandle;
	CD3DX12_CPU_DESCRIPTOR_HANDLE		m_descriptorHeapDSVCpuStartHandle;
	CD3DX12_CPU_DESCRIPTOR_HANDLE		m_descriptorHeapCBVSRVUAVSCpuStartHandleForImGui;
	CD3DX12_GPU_DESCRIPTOR_HANDLE		m_descriptorHeapCBVSRVUAVSGpuStartHandleForImGui;
	CD3DX12_CPU_DESCRIPTOR_HANDLE		m_descriptorHeapCBVSRVUAVSCpuStartHandleForGame;
	CD3DX12_GPU_DESCRIPTOR_HANDLE		m_descriptorHeapCBVSRVUAVSGpuStartHandleForGame;
	
	void		TimersReset()						{ m_timerTotal.Reset(); m_timerCaption.Reset(); m_timerFrame.Reset(); }
	void		TimersStop()						{ if (IsStopped() == true) { m_timerTotal.Stop(); m_timerCaption.Stop(); m_timerFrame.Stop(); } }
	void		TimersStart()						{ if (IsStopped() == false) { m_timerTotal.Start(); m_timerCaption.Start(); m_timerFrame.Start(); } }
	Timer		m_timerTotal;
	Timer		m_timerCaption;
	Timer		m_timerFrame;
	
	bool		IsStopped()	const					{ return m_isResizing || m_isInActive; }
	bool		m_isResizing						= false;
	bool		m_isInActive						= false;

	bool		NeedResetScreenSetting() const		{ return m_needResetScreenMode || m_needResetHDR; }
	bool		m_needResetScreenMode				= false;
	bool		m_needResetHDR						= false;

	POINT		m_inputMousePositionClient			= { 0, 0 };
	POINT		m_inputMouseClickedPositionClient	= { 0, 0 };
	bool		m_inputIsClicked					= false;
	int			m_inputScrollDelta					= 0;

	GameState	m_gameState							= GAME_STATE_LOBBY;

	bool		m_imGuiInitialized					= false;
};
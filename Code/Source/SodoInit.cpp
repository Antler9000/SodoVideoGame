#include <windows.h>
#include <d3dx12_root_signature.h>
#include <d3dx12_core.h>
#include <d3d12.h>
#include <d3dcommon.h>
#include <dxgi1_6.h>
#include <dxgi1_5.h>
#include <dxgi1_4.h>
#include <dxgi1_3.h>
#include <dxgi1_2.h>
#include <dxgi.h>
#include <dxgicommon.h>
#include <dxgitype.h>
#include <wrl/client.h>
#include <algorithm>
#include <vector>
#include <string>
#include <stdexcept>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "Sodo.h"
#include "Option.h"
#include "Alloc.h"
#include "Debug.h"

using Microsoft::WRL::ComPtr;
using std::wstring;

void Sodo::InitSavedOptions()
{

}

void Sodo::InitScreenMode()
{
	if(m_optionFullScreen.IsActive())
	{
		SetFullScreenMode();
	}
	else
	{
		SetWindowMode();
	}
}

void Sodo::InitFactory()
{
	UINT factoryFlags = 0;

#ifdef _DEBUG
	factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	ThrowIfFailed(CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(m_factory.ReleaseAndGetAddressOf())));

	//NOTE : 쿼리 출력 매개변수는 BOOL 타입이므로, bool 타입인 m_optionTearing.featureSupported를 매개변수로 사용하면 안 됨
	BOOL tearingQuery = FALSE;
	m_factory->CheckFeatureSupport(
		DXGI_FEATURE_PRESENT_ALLOW_TEARING, 
		&tearingQuery,
		sizeof(tearingQuery)
	);
	m_optionTearing.featureSupported = tearingQuery;
}

void Sodo::InitAdapter()
{
	ThrowIfFailed(
		m_factory->EnumAdapterByGpuPreference(
			0,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(m_adapter.ReleaseAndGetAddressOf())
		)
	);

#ifdef _DEBUG
	DXGI_ADAPTER_DESC adapterDesc;
	m_adapter->GetDesc(&adapterDesc);

	wstring adapterText = L"[SODO DEBUG] 어댑터 : ";
	adapterText += adapterDesc.Description;
	adapterText += L"\n";

	OutputDebugStringW(adapterText.c_str());
#endif
}

void Sodo::InitOutput()
{
	m_output.Reset();
	m_output6.Reset();

	ThrowIfFailed(m_adapter->EnumOutputs(0, m_output.ReleaseAndGetAddressOf()));

	m_optionHDR.outputSupported = SUCCEEDED(m_output.As(&m_output6));

	if (m_optionHDR.outputSupported)
	{
		m_output6->GetDesc1(&m_outputDesc);
	}

#ifdef _DEBUG
	DXGI_OUTPUT_DESC outputDesc;
	m_output->GetDesc(&outputDesc);

	wstring outputText = L"[SODO DEBUG] 아웃풋 : ";
	outputText += outputDesc.DeviceName;
	outputText += L"\n";

	OutputDebugStringW(outputText.c_str());
#endif
}

void Sodo::InitDisplayMode()
{
	UINT modeCount = 0;
	ThrowIfFailed(m_output->GetDisplayModeList(m_backBufferFormatSDR, 0, &modeCount, nullptr));

	if (modeCount <= 0)
	{
		throw std::runtime_error("there is no supported display mode");
	}

	std::vector<DXGI_MODE_DESC> modeList(modeCount);
	ThrowIfFailed(m_output->GetDisplayModeList(m_backBufferFormatSDR, 0, &modeCount, &modeList[0]));

	m_displayModeDesc = *std::max_element(
		modeList.begin(),
		modeList.end(),
		[](const DXGI_MODE_DESC& lhs, const DXGI_MODE_DESC& rhs)
		{
			UINT lhsArea = lhs.Width * lhs.Height;
			UINT rhsArea = rhs.Width * rhs.Height;
			if (lhsArea != rhsArea)
			{
				return lhsArea < rhsArea;
			}

			if (lhs.RefreshRate.Denominator == 0)
			{
				return true;
			}

			if (rhs.RefreshRate.Denominator == 0)
			{
				return false;
			}

			float lhsRefreshRate = float(lhs.RefreshRate.Numerator) / lhs.RefreshRate.Denominator;
			float rhsRefreshRate = float(rhs.RefreshRate.Numerator) / rhs.RefreshRate.Denominator;
			return lhsRefreshRate < rhsRefreshRate;
		}
	);

#ifdef _DEBUG
	UINT refreshRateNumerator = m_displayModeDesc.RefreshRate.Numerator;
	UINT refreshRateDenominator = m_displayModeDesc.RefreshRate.Denominator;

	wchar_t displayInfoBuffer[124] = { };
	swprintf(
		displayInfoBuffer,
		_countof(displayInfoBuffer),
		L"[SODO DEBUG] 디스플레이 : %u x %u (%u/%u hz)\n",
		m_displayModeDesc.Width,
		m_displayModeDesc.Height,
		refreshRateNumerator,
		refreshRateDenominator
	);

	OutputDebugStringW(displayInfoBuffer);
#endif
}

void Sodo::InitDevice()
{
	m_device5.Reset();
	m_device2.Reset();
	m_device.Reset();

#ifdef _DEBUG
	ComPtr<ID3D12Debug1> debug;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(debug.GetAddressOf())));
	debug->EnableDebugLayer();
	debug->SetEnableGPUBasedValidation(true);
#endif

	ThrowIfFailed(
		D3D12CreateDevice(m_adapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(m_device.ReleaseAndGetAddressOf()))
	);

	m_optionMeshShader.deviceSupported = SUCCEEDED(m_device.As(&m_device2));
	m_optionRayTracing.deviceSupported = SUCCEEDED(m_device.As(&m_device5));

	if (m_optionMeshShader.deviceSupported)
	{	
		D3D12_FEATURE_DATA_D3D12_OPTIONS7 meshShaderFeatureQuery = {};
		m_device2->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS7, &meshShaderFeatureQuery, sizeof(meshShaderFeatureQuery));
		m_optionMeshShader.featureSupported = (meshShaderFeatureQuery.MeshShaderTier != D3D12_MESH_SHADER_TIER_NOT_SUPPORTED);
	}
	if (m_optionRayTracing.deviceSupported)
	{
		D3D12_FEATURE_DATA_D3D12_OPTIONS5 rayTracingFeatureQuery = {};
		m_device5->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &rayTracingFeatureQuery, sizeof(rayTracingFeatureQuery));
		m_optionRayTracing.featureSupported = (rayTracingFeatureQuery.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED);
	}
}

void Sodo::InitFormatSupport()
{
	D3D12_FEATURE_DATA_FORMAT_SUPPORT backBufferFormatSDRQuery = {
		m_backBufferFormatSDR,
		D3D12_FORMAT_SUPPORT1_NONE,
		D3D12_FORMAT_SUPPORT2_NONE
	};
	D3D12_FEATURE_DATA_FORMAT_SUPPORT backBufferFormatHDRQuery = {
		m_backBufferFormatHDR,
		D3D12_FORMAT_SUPPORT1_NONE,
		D3D12_FORMAT_SUPPORT2_NONE
	};
	D3D12_FEATURE_DATA_FORMAT_SUPPORT depthStencilFormatQuery = {
		m_depthStencilBufferFormat,
		D3D12_FORMAT_SUPPORT1_NONE,
		D3D12_FORMAT_SUPPORT2_NONE
	};

	ThrowIfFailed(
		m_device->CheckFeatureSupport(
			D3D12_FEATURE_FORMAT_SUPPORT, &backBufferFormatSDRQuery, sizeof(backBufferFormatSDRQuery)
		)
	);
	HRESULT hdrQueryResult = m_device->CheckFeatureSupport(
		D3D12_FEATURE_FORMAT_SUPPORT, &backBufferFormatHDRQuery, sizeof(backBufferFormatHDRQuery)
	);
	ThrowIfFailed(
		m_device->CheckFeatureSupport(
			D3D12_FEATURE_FORMAT_SUPPORT, &depthStencilFormatQuery, sizeof(depthStencilFormatQuery)
		)
	);

	ThrowIfFalse(backBufferFormatSDRQuery.Support1 & D3D12_FORMAT_SUPPORT1_RENDER_TARGET);
	if (SUCCEEDED(hdrQueryResult))
	{
		m_optionHDR.formatSupported = ((backBufferFormatHDRQuery.Support1 & D3D12_FORMAT_SUPPORT1_RENDER_TARGET) != 0);
	}
	ThrowIfFalse(depthStencilFormatQuery.Support1 & D3D12_FORMAT_SUPPORT1_DEPTH_STENCIL);
}

void Sodo::InitFence()
{
	ThrowIfFailed(m_device->CreateFence(m_currentFence, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.ReleaseAndGetAddressOf())));
}

void Sodo::InitCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
	commandQueueDesc.Type		= D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandQueueDesc.Priority	= D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandQueueDesc.Flags		= D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.NodeMask	= 0;
	ThrowIfFailed(
		m_device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(m_commandQueue.ReleaseAndGetAddressOf()))
	);
}

void Sodo::InitCommandAllocator()
{
	ThrowIfFailed(
		m_device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT, 
			IID_PPV_ARGS(m_commandAllocator.ReleaseAndGetAddressOf())
		)
	);
}

void Sodo::InitCommandList()
{
	m_commandList6.Reset();
	m_commandList4.Reset();
	m_commandList.Reset();

	ThrowIfFailed(
		m_device->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_commandAllocator.Get(),
			nullptr,
			IID_PPV_ARGS(m_commandList.ReleaseAndGetAddressOf())
		)
	);

	m_optionRayTracing.commandListSupported = SUCCEEDED(m_commandList.As(&m_commandList4));
	m_optionMeshShader.commandListSupported = SUCCEEDED(m_commandList.As(&m_commandList6));
 
	m_commandList->Close();
}

void Sodo::InitHDRSwapChainSupport()
{
	m_swapChain.Reset();

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc	= {};
	swapChainDesc.Width					= 0;
	swapChainDesc.Height				= 0;
	swapChainDesc.Format				= m_backBufferFormatHDR;
	swapChainDesc.Stereo				= false;
	swapChainDesc.SampleDesc.Count		= 1;
	swapChainDesc.SampleDesc.Quality	= 0;
	swapChainDesc.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount			= m_backBufferCount;
	swapChainDesc.Scaling				= DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect			= DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode				= DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags					= DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT
										| (m_optionTearing.featureSupported ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0);

	ComPtr<IDXGISwapChain1> tempSwapChain = nullptr;

	ThrowIfFailed(
		m_factory->CreateSwapChainForHwnd(
			m_commandQueue.Get(),
			m_hWnd,
			&swapChainDesc,
			nullptr,
			nullptr,
			tempSwapChain.ReleaseAndGetAddressOf()
		)
	);

	ComPtr<IDXGISwapChain3> tempSwapChain3 = nullptr;

	ThrowIfFailed((tempSwapChain.As(&tempSwapChain3)));

	UINT colorSpaceHDRQuery = 0;
	HRESULT queryResult = tempSwapChain3->CheckColorSpaceSupport(m_backBufferColorSpaceHDR, &colorSpaceHDRQuery);
	if (SUCCEEDED(queryResult))
	{
		m_optionHDR.colorSpaceSupported = ((colorSpaceHDRQuery & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT) != 0);
	}
}

void Sodo::InitSwapChain()
{
	m_swapChain.Reset();

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width					= 0;
	swapChainDesc.Height				= 0;
	swapChainDesc.Format				= m_optionHDR.IsActive() ? m_backBufferFormatHDR : m_backBufferFormatSDR;
	swapChainDesc.Stereo				= false;
	swapChainDesc.SampleDesc.Count		= 1;
	swapChainDesc.SampleDesc.Quality	= 0;
	swapChainDesc.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount			= m_backBufferCount;
	swapChainDesc.Scaling				= DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect			= DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode				= DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags					= DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT
										| (m_optionTearing.featureSupported ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0);

	ComPtr<IDXGISwapChain1> tempSwapChain = nullptr;

	ThrowIfFailed(
		m_factory->CreateSwapChainForHwnd(
			m_commandQueue.Get(),
			m_hWnd,
			&swapChainDesc,
			nullptr,
			nullptr,
			tempSwapChain.ReleaseAndGetAddressOf()
		)
	);

	ThrowIfFailed(tempSwapChain.As(&m_swapChain));

	if (m_optionHDR.IsActive())
	{
		m_swapChain->SetColorSpace1(m_backBufferColorSpaceHDR);
	}
	else
	{
		m_swapChain->SetColorSpace1(m_backBufferColorSpaceSDR);
	}
}

void Sodo::InitBackBuffers()
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

	m_swapChain->GetDesc1(&swapChainDesc);
	m_backBufferWidth = swapChainDesc.Width;
	m_backBufferHeight = swapChainDesc.Height;
	m_backBufferAspectRatio = (m_backBufferHeight ? (static_cast<float>(m_backBufferWidth) / m_backBufferHeight) : 0);

	m_currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

	for (int i = 0; i < m_backBufferCount; i++)
	{
		ThrowIfFailed(m_swapChain->GetBuffer(i, IID_PPV_ARGS(m_backBuffers[i].ReleaseAndGetAddressOf())));
	}
}

void Sodo::InitViewPort()
{
	m_viewPort.TopLeftX = 0.0f;
	m_viewPort.TopLeftY = 0.0f;
	m_viewPort.Width = FLOAT(m_backBufferWidth);
	m_viewPort.Height = FLOAT(m_backBufferHeight);
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
}

void Sodo::InitScissorRectangle()
{
	m_scissorRectangle.left = 0;
	m_scissorRectangle.top = 0;
	m_scissorRectangle.right = m_backBufferWidth;
	m_scissorRectangle.bottom = m_backBufferHeight;
}

void Sodo::InitDepthStencilBuffer()
{
	D3D12_RESOURCE_DESC depthStencilBufferDesc = {};
	depthStencilBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilBufferDesc.Alignment = 0;
	depthStencilBufferDesc.Width = m_backBufferWidth;
	depthStencilBufferDesc.Height = m_backBufferHeight;
	depthStencilBufferDesc.DepthOrArraySize = 1;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.Format = m_depthStencilBufferFormat;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = m_depthStencilBufferFormat;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;

	D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	ThrowIfFailed(
		m_device->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&depthStencilBufferDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(m_depthStencilBuffer.ReleaseAndGetAddressOf())
		)
	);
}

void Sodo::InitDescriptorHeapRTV()
{
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
	descriptorHeapDesc.NumDescriptors = m_backBufferCount;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	descriptorHeapDesc.NodeMask = 0;

	ThrowIfFailed(
		m_device->CreateDescriptorHeap(
			&descriptorHeapDesc,
			IID_PPV_ARGS(m_descriptorHeapRTV.ReleaseAndGetAddressOf())
		)
	);

	m_incrementSizeRTV = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_cpuStartHandleRTV = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descriptorHeapRTV->GetCPUDescriptorHandleForHeapStart());
}

void Sodo::InitDescriptorHeapDSV()
{
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
	descriptorHeapDesc.NumDescriptors = 1;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	descriptorHeapDesc.NodeMask = 0;

	ThrowIfFailed(
		m_device->CreateDescriptorHeap(
			&descriptorHeapDesc,
			IID_PPV_ARGS(m_descriptorHeapDSV.ReleaseAndGetAddressOf())
		)
	);

	m_incrementSizeDSV = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_cpuStartHandleDSV = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descriptorHeapDSV->GetCPUDescriptorHandleForHeapStart());
}

void Sodo::InitDescriptorHeapCBVSRVUAV()
{
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
	UINT totalCount = m_countCBV + m_countSRV + m_countUAV;
	descriptorHeapDesc.NumDescriptors = (totalCount ? totalCount : 128);
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descriptorHeapDesc.NodeMask = 0;

	ThrowIfFailed(
		m_device->CreateDescriptorHeap(
			&descriptorHeapDesc,
			IID_PPV_ARGS(m_descriptorHeapCBVSRVUAV.ReleaseAndGetAddressOf())
		)
	);

	//NOTE : 우선 ImGui가 SRV를 둘 곳을 고정적으로 남기고, 그 뒷부분부터 사용하기로 함
	m_incrementSizeCBVSRVUAV = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_cpuStartHandleCBVSRVUAVForImGui = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descriptorHeapCBVSRVUAV->GetCPUDescriptorHandleForHeapStart());
	m_gpuStartHandleCBVSRVUAVForImGui = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descriptorHeapCBVSRVUAV->GetGPUDescriptorHandleForHeapStart());

	m_cpuStartHandleCBVSRVUAVForGame = m_cpuStartHandleCBVSRVUAVForImGui;
	m_cpuStartHandleCBVSRVUAVForGame.Offset(m_imGuiDescriptorHeapCapacity, m_incrementSizeCBVSRVUAV);
	m_gpuStartHandleCBVSRVUAVForGame = m_gpuStartHandleCBVSRVUAVForImGui;
	m_gpuStartHandleCBVSRVUAVForGame.Offset(m_imGuiDescriptorHeapCapacity, m_incrementSizeCBVSRVUAV);
}


void Sodo::InitRTV()
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandleRTV = m_cpuStartHandleRTV;
	for (int i = 0; i < m_backBufferCount; i++)
	{
		m_device->CreateRenderTargetView(m_backBuffers[i].Get(), nullptr, cpuHandleRTV.Offset(i, m_incrementSizeRTV));
	}
}

void Sodo::InitDSV()
{
	m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), nullptr, m_cpuStartHandleDSV);
}

void Sodo::InitCBVSRVUAV()
{

}

void Sodo::InitFenceEvent()
{
	CloseFenceEvent();

	m_fenceEvent = CreateEventExW(
		nullptr,
		nullptr,
		0,
		EVENT_MODIFY_STATE | SYNCHRONIZE
	);

	ThrowIfNull(m_fenceEvent);
}

void Sodo::InitImGui()
{
	if (m_imGuiInitialized)
	{
		CloseImGui();
		m_imGuiDescriptorHeapAllocator.Destroy();
	}

	m_imGuiDescriptorHeapAllocator.Create(m_device.Get(), m_descriptorHeapCBVSRVUAV.Get(), m_imGuiDescriptorHeapCapacity);

	ImGui_ImplDX12_InitInfo initInfo = {};
	initInfo.Device = m_device.Get();
	initInfo.CommandQueue = m_commandQueue.Get();
	initInfo.NumFramesInFlight = 1;
	initInfo.RTVFormat = m_optionHDR.IsActive() ? m_backBufferFormatHDR : m_backBufferFormatSDR;

	initInfo.SrvDescriptorHeap = m_descriptorHeapCBVSRVUAV.Get();
	initInfo.SrvDescriptorAllocFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_handle) { return m_imGuiDescriptorHeapAllocator.Alloc(out_cpu_handle, out_gpu_handle); };
	initInfo.SrvDescriptorFreeFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle) { return m_imGuiDescriptorHeapAllocator.Free(cpu_handle, gpu_handle); };
	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(m_hWnd);
	ImGui_ImplDX12_Init(&initInfo);

	ImGui::StyleColorsClassic();

	ImGuiStyle& style = ImGui::GetStyle();	
	style.ScaleAllSizes(m_imGuiScale);
	style.FontScaleDpi = m_imGuiScale;

	ImVec4* colors = style.Colors;
	colors[ImGuiCol_Text]				= ImVec4(0.78f, 0.74f, 0.63f, 1.0f);
	colors[ImGuiCol_TextDisabled]		= ImVec4(0.38f, 0.38f, 0.34f, 1.0f);
	colors[ImGuiCol_FrameBg]			= ImVec4(0.105f, 0.110f, 0.100f, 1.0f);
	colors[ImGuiCol_FrameBgHovered]		= ImVec4(0.185f, 0.175f, 0.130f, 1.0f);
	colors[ImGuiCol_FrameBgActive]		= ImVec4(0.260f, 0.230f, 0.140f, 1.0f);
	colors[ImGuiCol_CheckMark]			= ImVec4(0.82f, 0.68f, 0.32f, 1.0f);
	colors[ImGuiCol_SliderGrab]			= ImVec4(0.58f, 0.50f, 0.30f, 1.0f);
	colors[ImGuiCol_SliderGrabActive]	= ImVec4(0.82f, 0.66f, 0.28f, 1.0f);
	colors[ImGuiCol_WindowBg]			= ImVec4(0.075f, 0.078f, 0.075f, 1.0f);
	colors[ImGuiCol_TitleBg]			= ImVec4(0.115f, 0.115f, 0.105f, 1.0f);
	colors[ImGuiCol_TitleBgActive]		= ImVec4(0.180f, 0.170f, 0.145f, 1.0f);
	colors[ImGuiCol_TitleBgCollapsed]	= ImVec4(0.060f, 0.062f, 0.060f, 1.0f);
	colors[ImGuiCol_Button]				= ImVec4(0.210f, 0.215f, 0.200f, 1.0f);
	colors[ImGuiCol_ButtonHovered]		= ImVec4(0.340f, 0.320f, 0.250f, 1.0f);
	colors[ImGuiCol_ButtonActive]		= ImVec4(0.470f, 0.380f, 0.180f, 1.0f);
	colors[ImGuiCol_Border]				= ImVec4(0.30f, 0.28f, 0.23f, 0.55f);
	colors[ImGuiCol_Separator]			= ImVec4(0.34f, 0.31f, 0.25f, 0.65f);
	colors[ImGuiCol_SeparatorHovered]	= ImVec4(0.45f, 0.38f, 0.34f, 0.78f);
	colors[ImGuiCol_SeparatorActive]	= ImVec4(0.55f, 0.45f, 0.43f, 0.90f);

	m_imGuiInitialized = true;
}

void Sodo::InitTimer()
{
	ResetTimers();
}
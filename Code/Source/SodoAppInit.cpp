#include <windows.h>
#include <d3dx12_root_signature.h>
#include <d3d12sdklayers.h>
#include <d3dx12.h>
#include <d3d12.h>
#include <d3dcommon.h>
#include <dxgicommon.h>
#include <dxgi1_6.h>
#include <dxgi1_3.h>
#include <dxgi1_2.h>
#include <dxgi.h>
#include <dxgitype.h>
#include <wrl/client.h>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "SodoApp.h"
#include "Option.h"
#include "Throw.h"

using Microsoft::WRL::ComPtr;
using std::wstring;

void SodoApp::InitFactory()
{
	UINT factoryFlags = 0;

#ifdef _DEBUG
	factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	ThrowIfFailed(CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(m_factory.ReleaseAndGetAddressOf())));

	//TODO : m_optionTearing.featureSupported를 여기서 확인하자
}

void SodoApp::InitAdapter()
{
	//NOTE : 간단히 구현하기 위해, 가장 성능이 높은 GPU 하나로 adapter 인터페이스를 생성 시도함
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

void SodoApp::InitOutput()
{
	m_output.Reset();
	m_output6.Reset();

	//NOTE : 간단히 구현하기 위해, 현재 주 디스플레이로 output 인터페이스를 생성 시도함
	ThrowIfFailed(m_adapter->EnumOutputs(0, m_output.ReleaseAndGetAddressOf()));

	m_optionHDR.outputSupported = SUCCEEDED(m_output.As(&m_output6));

	if (m_optionHDR.outputSupported == true)
	{
		//TODO : m_outputDesc를 여기서 얻어내자
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

void SodoApp::InitDisplayMode()
{
	UINT modeCount = 0;
	ThrowIfFailed(m_output->GetDisplayModeList(m_backBufferFormatSDR, 0, &modeCount, nullptr));

	if (modeCount <= 0)
	{
		throw std::runtime_error("there is no supported display mode");
	}

	std::vector<DXGI_MODE_DESC> modeList(modeCount);
	ThrowIfFailed(m_output->GetDisplayModeList(m_backBufferFormatSDR, 0, &modeCount, &modeList[0]));

	//NOTE : 간단히 구현하기 위해, 가장 크고 주사율이 높은 디스플레이 모드를 멤버 변수에 저장함
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

void SodoApp::InitDevice()
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

	//TODO : m_backBufferFormatSDR를 지원하는지 여부를 여기서 확인하고, 지원하지 않을 시 ThrowIfFailed로 종료하자
	//TODO : m_optionHDR.formatSupported를 여기서 확인하자

	m_optionMeshShader.deviceSupported = SUCCEEDED(m_device.As(&m_device2));
	m_optionRayTracing.deviceSupported = SUCCEEDED(m_device.As(&m_device5));

	if (m_optionMeshShader.deviceSupported == true)
	{
		//TODO :m_optionMeshShader.featureSupported를 여기서 확인하자
	}
	if (m_optionRayTracing.deviceSupported == true)
	{
		//TODO : m_optionRayTracing.featureSupported를 여기서 확인하자
	}
}

void SodoApp::InitFence()
{
	ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.ReleaseAndGetAddressOf())));
}

void SodoApp::InitCommandQueue()
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

void SodoApp::InitCommandAllocator()
{
	ThrowIfFailed(
		m_device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT, 
			IID_PPV_ARGS(m_commandAllocator.ReleaseAndGetAddressOf())
		)
	);
}

void SodoApp::InitCommandList()
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

void SodoApp::InitSDRSwapChain()
{
	m_swapChain3.Reset();
	m_swapChain.Reset();

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = 0;
	swapChainDesc.Height = 0;
	swapChainDesc.Format = m_backBufferFormatSDR;
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = m_backBufferCount;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags	= DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT
						| (m_optionTearing.featureSupported ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0);

	ThrowIfFailed(
		m_factory->CreateSwapChainForHwnd(
			m_commandQueue.Get(),
			m_hWnd,
			&swapChainDesc,
			nullptr,
			nullptr,
			m_swapChain.ReleaseAndGetAddressOf()
		)
	);

	m_optionHDR.swapChainSupported = SUCCEEDED(m_swapChain.As(&m_swapChain3));

	if (m_optionHDR.swapChainSupported == true)
	{
		//TODO : m_optionHDR.colorSpaceSupported를 여기서 확인하자
	}
}

void SodoApp::InitHDRSwapChain()
{
	if (m_optionHDR.IsActive() == false)
	{
		return;
	}

	m_swapChain3.Reset();
	m_swapChain.Reset();

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
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

	ThrowIfFailed(
		m_factory->CreateSwapChainForHwnd(
			m_commandQueue.Get(),
			m_hWnd,
			&swapChainDesc,
			nullptr,
			nullptr,
			m_swapChain.ReleaseAndGetAddressOf()
		)
	);

	ThrowIfFailed(m_swapChain.As(&m_swapChain3));

	//여기서 스왑체인에 m_backBufferColorSpaceHDR을 설정하자
}

void SodoApp::InitBackBuffers()
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

	m_swapChain->GetDesc1(&swapChainDesc);
	m_backBufferWidth = swapChainDesc.Width;
	m_backBufferHeight = swapChainDesc.Height;

	for (int i = 0; i < m_backBufferCount; i++)
	{
		ThrowIfFailed(m_swapChain->GetBuffer(i, IID_PPV_ARGS(m_backBuffers[i].ReleaseAndGetAddressOf())));
	}
}

void SodoApp::InitDepthStencilBuffer()
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

void SodoApp::InitDescriptorHeapRTV()
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

void SodoApp::InitDescriptorHeapDSV()
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

void SodoApp::InitDescriptorHeapCBVSRVUAV()
{
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
	UINT totalCount = m_countCBV + m_countSRV + m_countUAV;
	descriptorHeapDesc.NumDescriptors = (totalCount ? totalCount : 1);
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descriptorHeapDesc.NodeMask = 0;

	ThrowIfFailed(
		m_device->CreateDescriptorHeap(
			&descriptorHeapDesc,
			IID_PPV_ARGS(m_descriptorHeapCBVSRVUAV.ReleaseAndGetAddressOf())
		)
	);

	m_incrementSizeCBVSRVUAV = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_cpuStartHandleCBVSRVUAV = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descriptorHeapCBVSRVUAV->GetCPUDescriptorHandleForHeapStart());
	m_gpuStartHandleCBVSRVUAV = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descriptorHeapCBVSRVUAV->GetGPUDescriptorHandleForHeapStart());
}


void SodoApp::InitRTV()
{
	for (int i = 0; i < m_backBufferCount; i++)
	{
		m_device->CreateRenderTargetView(m_backBuffers[i].Get(), nullptr, m_cpuStartHandleRTV.Offset(i, m_incrementSizeRTV));
	}
}

void SodoApp::InitDSV()
{
	m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), nullptr, m_cpuStartHandleDSV);
}

void SodoApp::InitCBVSRVUAV()
{

}
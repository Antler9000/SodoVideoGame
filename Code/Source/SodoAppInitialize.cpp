#include <windows.h>
#include <d3d12sdklayers.h>
#include <d3d12.h>
#include <d3dcommon.h>
#include <dxgi1_6.h>
#include <dxgi1_3.h>
#include <dxgi.h>
#include <dxgitype.h>
#include <wrl/client.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "SodoApp.h"
#include "Throw.h"

using Microsoft::WRL::ComPtr;
using std::wstring;

void SodoApp::InitializeFactory()
{
	UINT dxgiFactoryFlags = 0;

#ifdef _DEBUG
	dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));
}

void SodoApp::InitializeAdapter()
{
	//NOTE : 간단히 구현하기 위해, 가장 성능이 높은 GPU 하나로 adapter 인터페이스를 생성 시도함
	ThrowIfFailed(
		m_dxgiFactory->EnumAdapterByGpuPreference(
			0,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(m_dxgiAdapter.ReleaseAndGetAddressOf())
		)
	);

#ifdef _DEBUG
	DXGI_ADAPTER_DESC dxgiAdapterDesc;
	m_dxgiAdapter->GetDesc(&dxgiAdapterDesc);

	wstring adapterText = L"[SODO DEBUG] 어댑터 : ";
	adapterText += dxgiAdapterDesc.Description;
	adapterText += L"\n";

	OutputDebugStringW(adapterText.c_str());
#endif
}

void SodoApp::InitializeOutput()
{
	//NOTE : 간단히 구현하기 위해, 현재 주 디스플레이로 output 인터페이스를 생성 시도함
	ComPtr<IDXGIOutput> tempOutput = nullptr;
	ThrowIfFailed(m_dxgiAdapter->EnumOutputs(0, tempOutput.ReleaseAndGetAddressOf()));
	ThrowIfFailed(tempOutput.As(&m_dxgiOutput));

#ifdef _DEBUG
	DXGI_OUTPUT_DESC dxgiOutputDesc;
	m_dxgiOutput->GetDesc(&dxgiOutputDesc);

	wstring outputText = L"[SODO DEBUG] 아웃풋 : ";
	outputText += dxgiOutputDesc.DeviceName;
	outputText += L"\n";

	OutputDebugStringW(outputText.c_str());
#endif
}

void SodoApp::InitializeDisplayMode()
{
	UINT modeCount = 0;
	ThrowIfFailed(m_dxgiOutput->GetDisplayModeList(m_backBufferFormat, 0, &modeCount, nullptr));

	std::vector<DXGI_MODE_DESC> modeList(modeCount);
	ThrowIfFailed(m_dxgiOutput->GetDisplayModeList(m_backBufferFormat, 0, &modeCount, &modeList[0]));

	if (modeList.size() <= 0)
	{
		throw std::runtime_error("there is no supported display mode");
	}

	//NOTE : 간단히 구현하기 위해, 가장 크고 주사율이 높은 디스플레이 모드를 멤버 변수에 저장함
	m_displayModeDesc = *std::max_element(
		modeList.begin(),
		modeList.end(),
		[](const DXGI_MODE_DESC& lhs, const DXGI_MODE_DESC& rhs)
		{
			int lhsArea = lhs.Width * lhs.Height;
			int rhsArea = rhs.Width * rhs.Height;
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

void SodoApp::InitializeDevice()
{
#ifdef _DEBUG
	ComPtr<ID3D12Debug1> dxgiDebug;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(dxgiDebug.GetAddressOf())));
	dxgiDebug->EnableDebugLayer();
	dxgiDebug->SetEnableGPUBasedValidation(true);
#endif

	ThrowIfFailed(
		D3D12CreateDevice(m_dxgiAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(m_d3d12Device.ReleaseAndGetAddressOf()))
	);

	m_bDeviceSupportMeshShader = SUCCEEDED(m_d3d12Device.As(&m_d3d12Device2));
	m_bDeviceSupportRayTracing = SUCCEEDED(m_d3d12Device.As(&m_d3d12Device5));

#ifdef _DEBUG
	OutputDebugStringW(m_bDeviceSupportMeshShader ? L"[SODO DEBUG] 디바이스가 메시 셰이더 지원함\n" : L"[SODO DEBUG] 디바이스가 메시 셰이더 지원하지 않음\n");
	OutputDebugStringW(m_bDeviceSupportRayTracing ? L"[SODO DEBUG] 디바이스가 레이 트레이싱 지원함\n" : L"[SODO DEBUG] 디바이스가 레이 트레이싱 지원하지 않음\n");
#endif

	if (m_bDeviceSupportRayTracing == true)
	{
		//TODO : m_featureSupportRayTracing, m_bFeatureSupportMeshShader를 여기서 확인하자
	}
}

void SodoApp::InitializeFence()
{
	ThrowIfFailed(m_d3d12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_d3d12Fence.ReleaseAndGetAddressOf())));
}

void SodoApp::InitializeCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC d3d12QueueDesc = {};
	d3d12QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	d3d12QueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	d3d12QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	d3d12QueueDesc.NodeMask = 0;
	ThrowIfFailed(
		m_d3d12Device->CreateCommandQueue(&d3d12QueueDesc, IID_PPV_ARGS(m_d3d12CommandQueue.ReleaseAndGetAddressOf()))
	);
}

void SodoApp::InitializeCommandAllocator()
{
	ThrowIfFailed(
		m_d3d12Device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT, 
			IID_PPV_ARGS(m_d3d12CommandAllocator.ReleaseAndGetAddressOf())
		)
	);
}

void SodoApp::InitializeCommandList()
{
	ThrowIfFailed(
		m_d3d12Device->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_d3d12CommandAllocator.Get(),
			nullptr,
			IID_PPV_ARGS(m_d3d12CommandList.ReleaseAndGetAddressOf())
		)
	);

	m_bCommandListSupportRayTracing = SUCCEEDED(m_d3d12CommandList.As(&m_d3d12CommandList4));
	m_bCommandListSupportMeshShader = SUCCEEDED(m_d3d12CommandList.As(&m_d3d12CommandList6));

#ifdef _DEBUG
	OutputDebugStringW(m_bCommandListSupportRayTracing ? L"[SODO DEBUG] 커맨드 리스트가 레이 트레이싱 지원함\n" : L"[SODO DEBUG] 커맨드 리스트가 레이 트레이싱 지원하지 않음\n");
	OutputDebugStringW(m_bCommandListSupportMeshShader ? L"[SODO DEBUG] 커맨드 리스트가 메시 셰이더 지원함\n" : L"[SODO DEBUG] 커맨드 리스트가 메시 셰이더 지원하지 않음\n");
#endif
 
	m_d3d12CommandList->Close();
}
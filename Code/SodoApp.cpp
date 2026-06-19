#include <windows.h>
#include <windowsx.h>
#include <wchar.h>
#include <d3d12sdklayers.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <string>
#include <vector>
#include "SodoApp.h"

using Microsoft::WRL::ComPtr;
using std::wstring;

void SodoApp::CreateDevice()
{
	UINT dxgiFactoryFlags = 0;

#ifdef _DEBUG
	dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

	ComPtr<ID3D12Debug1> dxgiDebug;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(dxgiDebug.GetAddressOf())));
	dxgiDebug->EnableDebugLayer();
	dxgiDebug->SetEnableGPUBasedValidation(true);
#endif

	ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));

	ComPtr<IDXGIAdapter3> foundDxgiAdapter = nullptr;

	int i = 0;
	while (m_dxgiFactory->EnumAdapterByGpuPreference(
		i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
		IID_PPV_ARGS(foundDxgiAdapter.ReleaseAndGetAddressOf())) != DXGI_ERROR_NOT_FOUND)
	{
		HRESULT result = D3D12CreateDevice(
			foundDxgiAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(m_d3d12Device.ReleaseAndGetAddressOf())
		);
		
		if (SUCCEEDED(result))
		{
			m_dxgiAdapter = foundDxgiAdapter;

			m_deviceSupportRayTracing = SUCCEEDED(
				m_d3d12Device->QueryInterface(IID_PPV_ARGS(m_d3d12Device5.ReleaseAndGetAddressOf()))
			);

			if (m_deviceSupportRayTracing == true)
			{
				//TODO : m_featureSupportRayTracing를 여기서 확인하자
			}

			break;
		}

		i++;
	}
}

void SodoApp::PrintAdapterInfo() const
{
	DXGI_ADAPTER_DESC dxgiAdapterDesc;
	m_dxgiAdapter->GetDesc(&dxgiAdapterDesc);

	wstring adapterText = L"어댑터 : ";
	adapterText += dxgiAdapterDesc.Description;
	adapterText += L"\n";

	OutputDebugStringW(adapterText.c_str());

	PrintOutputInfo(m_dxgiAdapter.Get());
}

void SodoApp::PrintOutputInfo(IDXGIAdapter* dxgiAdapter) const
{
	ComPtr<IDXGIOutput> foundDxgiOutput = nullptr;

	int i = 0;
	while (dxgiAdapter->EnumOutputs(i, foundDxgiOutput.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC dxgiOutputDesc;
		foundDxgiOutput->GetDesc(&dxgiOutputDesc);

		wstring outputText = L"\t아웃풋 : ";
		outputText += dxgiOutputDesc.DeviceName;
		outputText += L"\n";

		OutputDebugStringW(outputText.c_str());

		PrintDisplayInfo(foundDxgiOutput.Get());

		i++;
	}
}

void SodoApp::PrintDisplayInfo(IDXGIOutput* dxgiOutput) const
{
	//NOTE : 마지막 매개변수를 nullptr로 줘서, modeCount에 모드 개수를 받아옴
	UINT modeCount = 0;
	dxgiOutput->GetDisplayModeList(m_backBufferFormat, 0, &modeCount, nullptr);

	std::vector<DXGI_MODE_DESC> foundModeList(modeCount);
	dxgiOutput->GetDisplayModeList(m_backBufferFormat, 0, &modeCount, &foundModeList[0]);

	for (const DXGI_MODE_DESC& mode : foundModeList)
	{
		UINT refreshRateNumerator = mode.RefreshRate.Numerator;
		UINT refreshRateDenominator = mode.RefreshRate.Denominator;

		wchar_t displayInfoBuffer[124] = { };
		swprintf(
			displayInfoBuffer,
			_countof(displayInfoBuffer),
			L"\t\t폭 : %u, 너비 : %u, 주사율 : %u/%u\n",
			mode.Width,
			mode.Height,
			refreshRateNumerator,
			refreshRateDenominator
		);

		OutputDebugStringW(displayInfoBuffer);
	}
}

//NOTE : SetWindowTextW를 너무 자주 호출하면 시스템 부하로 인해 윈도우 전체가 먹통이 되니 반복에 텀을 주자
void SodoApp::UpdateCaption()
{
	if (m_timer.GetNotCaptionedMilliSeconds() > 100.0f)
	{
		m_timer.MarkLastCaptionTime();

		wchar_t captionBuffer[124] = { };
		swprintf(
			captionBuffer,
			_countof(captionBuffer),
			L"%s (경과 시간 : %06.1f s / 프레임 시간 : %.4f ms / 마우스 위치 : (%04d p, %04d p) / 스크롤 각도 : %04d unit)",
			m_pAppName,
			m_timer.GetEleapsedMilliSeconds() / 1000,
			m_timer.GetFrameMilliSeconds(),
			m_mousePosClient.x,
			m_mousePosClient.y,
			m_scrollDelta
		);

		SetWindowTextW(m_hWnd, captionBuffer);
	}
}

//TODO :	각 메시지 처리를 함수로 만들어서 가독성을 높이자
//NOTE :	WindowProc이 수행 중에는 해당 스레드의 메시지 큐에 쌓인 다른 메시지들을 처리하지 못하므로,
//			되도록 이 안에서는 짧은 로직만 수행하도록 하고, 긴 대기가 필요한 로직은 별도 스레드로 처리하자
LRESULT SodoApp::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		//TODO :	이는 픽셀 단위의 마우스 커서 변화를 감지하므로, 정확도가 떨어지고 마우스 포인터 속도 설정에 영향을 받음
		//			따라서 인게임 마우스 조작의 경우엔 본 메시지가 아니라 마우스의 이동 자체를 받아오도록 WM_INPUT을 사용하자
		//TODO :	추가로, DirectXTK12에 인풋과 관련하여 사용 가능한 기능이 있는지 둘러보자
		case WM_MOUSEMOVE:
		{
			m_mousePosClient.x = GET_X_LPARAM(lParam);
			m_mousePosClient.y = GET_Y_LPARAM(lParam);

			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			if (m_isClicked == true)
			{
				return 0;
			}

			m_clickedPosClient.x = GET_X_LPARAM(lParam);
			m_clickedPosClient.y = GET_Y_LPARAM(lParam);
			m_isClicked = true;

			return 0;
		}

		case WM_LBUTTONUP:
		{
			wchar_t messageBuffer[64] = { };
			swprintf_s(
				messageBuffer,
				_countof(messageBuffer),
				L"%s%s 시작 (x, y) = (%d, %d) \n 끝 (x, y) = (%d, %d)",
				wParam & MK_CONTROL ? L"[CTRL]" : L"",
				wParam & MK_SHIFT ? L"[SHIFT]" : L"",
				m_clickedPosClient.x,
				m_clickedPosClient.y,
				GET_X_LPARAM(lParam),
				GET_Y_LPARAM(lParam)
			);

			int mouseManhattanDist = abs(GET_X_LPARAM(lParam) - m_clickedPosClient.x) + abs(GET_Y_LPARAM(lParam) - m_clickedPosClient.y);
			bool isDrag = (mouseManhattanDist > dragThresholdDist);
			MessageBoxW(m_hWnd, messageBuffer, isDrag ? L"좌측 마우스 드래그" : L"좌측 마우스 클릭", MB_OK);

			m_isClicked = false;

			return 0;
		}

		case WM_RBUTTONDOWN:
		{
			if (m_isClicked == true)
			{
				return 0;
			}

			m_clickedPosClient.x = GET_X_LPARAM(lParam);
			m_clickedPosClient.y = GET_Y_LPARAM(lParam);
			m_isClicked = true;

			return 0;
		}

		case WM_RBUTTONUP:
		{
			wchar_t messageBuffer[64] = { };
			swprintf_s(
				messageBuffer,
				_countof(messageBuffer),
				L"%s 시작 (x, y) = (%d, %d) \n 끝 (x, y) = (%d, %d)",
				wParam & MK_SHIFT ? L"[SHIFT]" : L"",
				m_clickedPosClient.x,
				m_clickedPosClient.y,
				GET_X_LPARAM(lParam),
				GET_Y_LPARAM(lParam)
			);

			int mouseManhattanDist = abs(GET_X_LPARAM(lParam) - m_clickedPosClient.x) + abs(GET_Y_LPARAM(lParam) - m_clickedPosClient.y);
			bool isDragging = (mouseManhattanDist > dragThresholdDist);
			MessageBoxW(m_hWnd, messageBuffer, isDragging ? L"우측 마우스 드래그" : L"우측 마우스 클릭", MB_OK);

			m_isClicked = false;

			return 0;
		}

		case WM_MBUTTONDOWN:
		{
			if (m_isClicked == true)
			{
				return 0;
			}

			m_clickedPosClient.x = GET_X_LPARAM(lParam);
			m_clickedPosClient.y = GET_Y_LPARAM(lParam);
			m_isClicked = true;

			return 0;
		}

		case WM_MBUTTONUP:
		{
			wchar_t messageBuffer[64] = { };
			swprintf_s(
				messageBuffer,
				_countof(messageBuffer),
				L"시작 (x, y) = (%d, %d) \n 끝 (x, y) = (%d, %d)",
				m_clickedPosClient.x,
				m_clickedPosClient.y,
				GET_X_LPARAM(lParam),
				GET_Y_LPARAM(lParam)
			);

			int mouseManhattanDist = abs(GET_X_LPARAM(lParam) - m_clickedPosClient.x) + abs(GET_Y_LPARAM(lParam) - m_clickedPosClient.y);
			bool isDrag = (mouseManhattanDist > dragThresholdDist);
			MessageBoxW(m_hWnd, messageBuffer, isDrag ? L"중간 마우스 드래그" : L"중간 마우스 클릭", MB_OK);

			m_isClicked = false;

			return 0;
		}

		case WM_MOUSEWHEEL:
		{
			m_scrollDelta += GET_WHEEL_DELTA_WPARAM(wParam);

			return 0;
		}

		//NOTE : ESC를 이용한 종료를 처리함
		case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE)
			{
				if (MessageBoxW(m_hWnd, L"ESC가 눌렸습니다. 어플리케이션을 종료합니까?", L"종료 문구", MB_OKCANCEL) == IDOK)
				{
					DestroyWindow(m_hWnd);
				}
			}

			return 0;
		}

		//NOTE : ALT+F4 혹은 우상단 창닫기 버튼을 이용한 종료를 처리함
		case WM_CLOSE:
		{
			if (MessageBoxW(m_hWnd, L"어플리케이션을 종료합니까?", L"종료 문구", MB_OKCANCEL) == IDOK)
			{
				DestroyWindow(m_hWnd);
			}

			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);

			return 0;
		}
	}

	return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}
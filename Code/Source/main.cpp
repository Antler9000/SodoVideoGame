#include <windows.h>
#include "SteelBalletApp.h"

//NOTE : NuGet 설치 확인용
#include "d3d12.h"
#include <wrl/client.h>

//NOTE :	링크 대상 라이브러리들을 명시함
//			이들은 기본 포함 라이브러리 디렉토리에 위치함
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

//NOTE : NuGet으로 설치한 DirectX12 Agility SDK 버전을 명시함
extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = 619;  }
extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = ".\\D3D12\\";  }

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	//NOTE : 컴파일 경고를 지우기 위해 작성함
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(pCmdLine);

	//NOTE : NuGet 설치 확인용
	using Microsoft::WRL::ComPtr;
	ComPtr<ID3D12Device> device;

	SteelBalletApp appInstance;

	if (appInstance.Create(hInstance, nCmdShow) == false)
	{
		return 0;
	}

	return appInstance.RunMessageLoop();
}
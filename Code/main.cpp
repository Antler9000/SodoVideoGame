#include <windows.h>
#include "SodoApp.h"

//NOTE : 수학 라이브러리 빌드 확인용 임시 코드
#include <DirectXMath.h>

//NOTE : 링크 대상 라이브러리들을 명시함
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxcompiler.lib")

//NOTE : DirectX12 Agility SDK의 런타임은 사용할 버전을 명시해야 한다
extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = 619;  }
extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = ".\\D3D12\\";  }

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	//NOTE : 컴파일 경고를 지우기 위해 작성함
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(pCmdLine);

	SodoApp appInstance;

	//NOTE : 수학 라이브러리 빌드 확인용 임시 코드	
	DirectX::XMVECTOR vec = { 1, 2, 3, 4 };
	DirectX::XMFLOAT4 vecSave = { 1, 2, 3, 4 };

	if (appInstance.Create(hInstance, nCmdShow) == false)
	{
		return 0;
	}

	return appInstance.RunMessageLoop();
}
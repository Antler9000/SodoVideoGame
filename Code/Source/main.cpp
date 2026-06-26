#include <windows.h>
#include <crtdbg.h>
#include "SodoApp.h"

//NOTE :	DXGI, D3D12, DXC 라이브러리가 링킹되도록 지시함
//			모든 번역 단위를 통틀어서 한 번만 지시하면 되므로 이곳 main.cpp에 작성했음
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxcompiler.lib")

//NOTE :	DirectX12 Agility SDK의 런타임은 사용할 버전을 정의해야 함
//			모든 번역 단위를 통틀어서 한 번만 정의해야 하므로 이곳 main.cpp에 작성했음
extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = 619;  }
extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = ".\\D3D12\\";  }

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	//NOTE : 컴파일 경고를 지우기 위해 작성함
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(pCmdLine);

	//NOTE : 종료시 메모리 누수가 있는지 확인함
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	SodoApp appInstance;

	appInstance.CreateAppWindow(hInstance, nCmdShow);

	appInstance.InitApp();

	return appInstance.RunMessageLoop();
}
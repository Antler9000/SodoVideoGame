#include <windows.h>
#include "TheseusApp.h"

extern "C" { __declspec(dllexport) extern const UINT D3D12SDKersion = 619;  }
extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = ".\\D3D12\\";  }

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(pCmdLine);

	TheseusApp appInstance;
	
	if (appInstance.Create(hInstance, nCmdShow) == false)
	{
		return 0;
	}

	return appInstance.RunMessageLoop();
}
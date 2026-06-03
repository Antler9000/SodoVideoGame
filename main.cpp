#include "TheseusApp.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(pCmdLine);

	TheseusApp appInstance;
	
	if (appInstance.Create(hInstance, nCmdShow) == false)
	{
		return 0;
	}

	appInstance.RunMessageLoop();

	return 0;
}
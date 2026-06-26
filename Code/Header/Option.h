#pragma once
#include <windows.h>

struct OptionSound
{
	bool userEnabled	= true;
	float mainVolume	= 100.0f;
	float uiVolume		= 100.0f;
	float unitVolume	= 100.0f;
	float effectVolume	= 100.0f;
	float musicVolume	= 100.0f;

	void DebugPrint() const
	{
#ifdef _DEBUG
		OutputDebugStringW(userEnabled ? L"[SODO DEBUG] Sound : 유저 활성화 On\n" : L"[SODO DEBUG]  : 유저 활성화 지원 off\n");
		
		wchar_t debugBuffer[128];

		swprintf_s(debugBuffer, L"[SODO DEBUG] Sound : 메인 볼륨 %.0f%%\n", mainVolume);
		OutputDebugStringW(debugBuffer);

		swprintf_s(debugBuffer, L"[SODO DEBUG] Sound : UI 볼륨 %.0f%%\n", uiVolume);
		OutputDebugStringW(debugBuffer);

		swprintf_s(debugBuffer, L"[SODO DEBUG] Sound : 효과 볼륨 %.0f%%\n", unitVolume);
		OutputDebugStringW(debugBuffer);

		swprintf_s(debugBuffer, L"[SODO DEBUG] Sound : 음악 볼륨 %.0f%%\n", effectVolume);
		OutputDebugStringW(debugBuffer);

		OutputDebugStringW(L"\n");
#endif
	}
};

struct OptionHDR
{
	bool outputSupported		= false;
	bool formatSupported		= false;
	bool colorSpaceSupported	= false;
	bool userEnabled			= false;

	bool IsActive() const
	{
		return CanBeEnabled() && userEnabled;
	}

	bool CanBeEnabled() const
	{
		return outputSupported && formatSupported && colorSpaceSupported;
	}

	void DebugPrint() const
	{
#ifdef _DEBUG
		OutputDebugStringW(outputSupported		? L"[SODO DEBUG] HDR : 아웃풋 지원 On\n"	: L"[SODO DEBUG] HDR : 아웃풋 지원 off\n");
		OutputDebugStringW(formatSupported		? L"[SODO DEBUG] HDR : 포맷 지원 On\n"		: L"[SODO DEBUG] HDR : 포맷 지원 off\n");
		OutputDebugStringW(colorSpaceSupported	? L"[SODO DEBUG] HDR : 색 공간 지원 On\n"	: L"[SODO DEBUG] HDR : 색 공간 지원 off\n");
		OutputDebugStringW(userEnabled			? L"[SODO DEBUG] HDR : 유저 활성화 On\n"	: L"[SODO DEBUG] HDR : 유저 활성화 지원 off\n");
		OutputDebugStringW(IsActive()			? L"[SODO DEBUG] => HDR On \n"				: L"[SODO DEBUG] => HDR Off \n");
		OutputDebugStringW(L"\n");
#endif
	}
};

struct OptionTearing
{
	bool featureSupported		= false;
	bool userEnabled			= false;

	bool IsActive() const
	{
		return CanBeEnabled() && userEnabled;
	}

	bool CanBeEnabled() const
	{
		return featureSupported;
	}

	void DebugPrint() const
	{
#ifdef _DEBUG
		OutputDebugStringW(featureSupported	? L"[SODO DEBUG] Tearing : 피처 지원 On\n"		: L"[SODO DEBUG] Tearing : 피처 지원 off\n");
		OutputDebugStringW(userEnabled		? L"[SODO DEBUG] Tearing : 유저 활성화 On\n"	: L"[SODO DEBUG] Tearing : 유저 활성화 지원 off\n");
		OutputDebugStringW(IsActive()		? L"[SODO DEBUG] => Tearing On \n"				: L"[SODO DEBUG] => Tearing Off \n");
		OutputDebugStringW(L"\n");
#endif
	}
};

struct OptionRayTracing
{
	bool deviceSupported		= false;
	bool featureSupported		= false;
	bool commandListSupported	= false;
	bool userEnabled			= false;

	bool IsActive() const
	{
		return CanBeEnabled() && userEnabled;
	}

	bool CanBeEnabled() const
	{
		return deviceSupported && featureSupported && commandListSupported;
	}

	void DebugPrint() const
	{
#ifdef _DEBUG
		OutputDebugStringW(deviceSupported		? L"[SODO DEBUG] RayTracing : 디바이스 지원 On\n"		: L"[SODO DEBUG] RayTracing : 디바이스 지원 off\n");
		OutputDebugStringW(featureSupported		? L"[SODO DEBUG] RayTracing : 피처 지원 On\n"			: L"[SODO DEBUG] RayTracing : 피처 지원 off\n");
		OutputDebugStringW(commandListSupported	? L"[SODO DEBUG] RayTracing : 커맨드 리스트 지원 On\n"	: L"[SODO DEBUG] RayTracing : 커맨드 리스트 지원 off\n");
		OutputDebugStringW(userEnabled			? L"[SODO DEBUG] RayTracing : 유저 활성화 On\n"			: L"[SODO DEBUG] RayTracing : 유저 활성화 off\n");
		OutputDebugStringW(IsActive()			? L"[SODO DEBUG] => RayTracing On \n"					: L"[SODO DEBUG] => RayTracing Off \n");
		OutputDebugStringW(L"\n");
#endif
	}
};

struct OptionMeshShader
{
	bool deviceSupported		= false;
	bool featureSupported		= false;
	bool commandListSupported	= false;
	bool userEnabled			= false;

	bool IsActive() const
	{
		return CanBeEnabled() && userEnabled;
	}

	bool CanBeEnabled() const
	{
		return deviceSupported && featureSupported && commandListSupported;
	}

	void DebugPrint() const
	{
#ifdef _DEBUG
		OutputDebugStringW(deviceSupported		? L"[SODO DEBUG] MeshShader : 디바이스 지원 On\n"		: L"[SODO DEBUG] MeshShader : 디바이스 지원 off\n");
		OutputDebugStringW(featureSupported		? L"[SODO DEBUG] MeshShader : 피처 지원 On\n"			: L"[SODO DEBUG] MeshShader : 피처 지원 off\n");
		OutputDebugStringW(commandListSupported	? L"[SODO DEBUG] MeshShader : 커맨드 리스트 지원 On\n"	: L"[SODO DEBUG] MeshShader : 커맨드 리스트 지원 off\n");
		OutputDebugStringW(userEnabled			? L"[SODO DEBUG] MeshShader : 유저 활성화 On\n"			: L"[SODO DEBUG] MeshShader : 유저 활성화 off\n");
		OutputDebugStringW(IsActive()			? L"[SODO DEBUG] => MeshShader On \n"					: L"[SODO DEBUG] => MeshShader Off \n");
		OutputDebugStringW(L"\n");
#endif
	}
};
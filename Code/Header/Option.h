#pragma once
#include <windows.h>

struct OptionHDR
{
	bool outputSupported		= false;
	bool formatSupported		= false;
	bool swapChainSupported		= false;
	bool colorSpaceSupported	= false;
	bool userEnabled			= false;

	bool IsActive() const
	{
		return CanBeEnabled() && userEnabled;
	}

	bool CanBeEnabled() const
	{
		return outputSupported && formatSupported && swapChainSupported && colorSpaceSupported;
	}

	void DebugPrint() const
	{
#ifdef _DEBUG
		OutputDebugStringW(outputSupported		? L"[SODO DEBUG] HDR : 아웃풋 지원 On\n"	: L"[SODO DEBUG] HDR : 아웃풋 지원 off\n");
		OutputDebugStringW(formatSupported		? L"[SODO DEBUG] HDR : 포맷 지원 On\n"		: L"[SODO DEBUG] HDR : 포맷 지원 off\n");
		OutputDebugStringW(swapChainSupported	? L"[SODO DEBUG] HDR : 스왑 체인 지원 On\n"	: L"[SODO DEBUG] HDR : 스왑 체인 지원 off\n");
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
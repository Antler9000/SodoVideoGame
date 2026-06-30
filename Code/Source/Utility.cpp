#include <windows.h>
#include <dxgi.h>
#include <string>
#include <cstdio>
#include <fstream>
#include <stdexcept>
#include "Sodo.h"
#include "Debug.h"

void Sodo::WaitAllCommandDone()
{
	if (m_fenceEvent == nullptr || m_fence == nullptr || m_commandQueue == nullptr)
	{
		return;
	}

	m_fenceCurrent++;

	ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_fenceCurrent));

	ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceCurrent, m_fenceEvent));
	auto waitResult = WaitForSingleObject(m_fenceEvent, INFINITE);

	if (waitResult != WAIT_OBJECT_0)
	{
		throw std::runtime_error("wait fence failed");
	}
}

void Sodo::ResetScreenSetting()
{
	if (m_needResetScreenMode == true)
	{
		if (m_optionFullScreen.userEnabled == true)
		{
			SetFullScreenMode();
		}
		else
		{
			SetWindowMode();
		}

		m_needResetScreenMode = false;
	}

	if (m_screenSwapChain != nullptr)
	{
		WaitAllCommandDone();

		for (int i = 0; i < m_screenBackBufferCount; i++)
		{
			m_screenBackBuffers[i].Reset();
		}

		ThrowIfFailed(
			m_screenSwapChain->ResizeBuffers(
				0,
				0,
				0,
				m_optionHDR.IsActive() ? m_screenBackBufferFormatHDR : m_screenBackBufferFormatSDR,
				DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT
				| (m_optionTearing.featureSupported ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0)
			)
		);

		if (m_optionHDR.IsActive() == true)
		{
			m_screenSwapChain->SetColorSpace1(m_screenBackBufferColorSpaceHDR);
		}
		else
		{
			m_screenSwapChain->SetColorSpace1(m_screenBackBufferColorSpaceSDR);
		}

		InitBackBuffers();
		InitViewPort();
		InitScissorRectangle();
		InitDepthStencilBuffer();
		InitRTV();
		InitDSV();
		InitImGui();
	}

	m_needResetHDR = false;
}

void Sodo::SetFullScreenMode()
{
	SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

	HMONITOR monitor = MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monitorInfo{};
	monitorInfo.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(monitor, &monitorInfo);

	LONG monitorXBase = monitorInfo.rcMonitor.left;
	LONG monitorYBase = monitorInfo.rcMonitor.top;
	LONG monitorWidth = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
	LONG monitorHeight = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

	SetWindowPos(
		m_hWnd,
		HWND_TOP,
		monitorXBase,
		monitorYBase,
		monitorWidth,
		monitorHeight,
		SWP_NOOWNERZORDER | SWP_FRAMECHANGED
	);
}

void Sodo::SetWindowMode()
{
	SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);

	HMONITOR monitor = MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monitorInfo{};
	monitorInfo.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(monitor, &monitorInfo);

	LONG monitorXBase = monitorInfo.rcMonitor.left;
	LONG monitorYBase = monitorInfo.rcMonitor.top;
	LONG monitorWidth = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
	LONG monitorHeight = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

	LONG windowWidth = monitorWidth * 2 / 3;
	LONG windowHeight = monitorHeight * 2 / 3;
	LONG windowXBase = monitorXBase + (monitorWidth / 2) - (windowWidth / 2);
	LONG windowYBase = monitorYBase + (monitorHeight / 2) - (windowHeight / 2);

	SetWindowPos(
		m_hWnd,
		HWND_TOP,
		windowXBase,
		windowYBase,
		windowWidth,
		windowHeight,
		SWP_NOOWNERZORDER | SWP_FRAMECHANGED
	);
}

void Sodo::SaveOptions()
{
	std::ofstream fout("SavedSettingsTemp.txt", std::ios::out | std::ios::trunc);

	if (!fout)
	{
		return;
	}

	fout << "FullScreen" << " " << (m_optionFullScreen.userEnabled ? "Yes" : "No") << '\n';
	fout << "HDR" << " " << (m_optionHDR.userEnabled ? "Yes" : "No") << '\n';
	fout << "Tearing" << " " << (m_optionTearing.userEnabled ? "Yes" : "No") << '\n';
	fout << "RayTracing" << " " << (m_optionRayTracing.userEnabled ? "Yes" : "No") << '\n';
	fout << "MeshShader" << " " << (m_optionMeshShader.userEnabled ? "Yes" : "No") << '\n';
	fout << "SoundMasterVolume" << " " << m_optionSound.masterVolume << '\n';
	fout << "SoundUIVolume" << " " << m_optionSound.uiVolume << '\n';
	fout << "SoundUnitVolume" << " " << m_optionSound.unitVolume << '\n';
	fout << "SoundEffectVolume" << " " << m_optionSound.effectVolume << '\n';
	fout << "SoundMusicVolume" << " " << m_optionSound.musicVolume << '\n';

	fout.close();

	int removeResult = std::remove("SavedSettings.txt");
	if (removeResult != 0)
	{
		return;
	}

	int renameResult = std::rename("SavedSettingsTemp.txt", "SavedSettings.txt");
	if (renameResult != 0)
	{
		return;
	}
}

void Sodo::RestoreOptions()
{
	std::ifstream fin("SavedSettings.txt", std::ios::in);

	if (!fin)
	{
		return;
	}

	bool result = true;

	bool fullScreenEnabledTemp = false;
	bool hdrEnabledTemp = false;
	bool tearingEnabledTemp = false;
	bool rayTracingEnabledTemp = false;
	bool meshShaderEnabledTemp = false;
	int soundMasterVolumeTemp = 0;
	int soundUIVolumeTemp = 0;
	int soundUnitVolumeTemp = 0;
	int soundEffectVolumeTemp = 0;
	int soundMusicVolumeTemp = 0;

	result &= ReadOptionBool(fin, "FullScreen", fullScreenEnabledTemp);
	result &= ReadOptionBool(fin, "HDR", hdrEnabledTemp);
	result &= ReadOptionBool(fin, "Tearing", tearingEnabledTemp);
	result &= ReadOptionBool(fin, "RayTracing", rayTracingEnabledTemp);
	result &= ReadOptionBool(fin, "MeshShader", meshShaderEnabledTemp);
	result &= ReadOptionInt(fin, "SoundMasterVolume", soundMasterVolumeTemp);
	result &= ReadOptionInt(fin, "SoundUIVolume", soundUIVolumeTemp);
	result &= ReadOptionInt(fin, "SoundUnitVolume", soundUnitVolumeTemp);
	result &= ReadOptionInt(fin, "SoundEffectVolume", soundEffectVolumeTemp);
	result &= ReadOptionInt(fin, "SoundMusicVolume", soundMusicVolumeTemp);

	if (result == false)
	{
		return;
	}

	m_optionFullScreen.userEnabled = fullScreenEnabledTemp;
	m_optionHDR.userEnabled = hdrEnabledTemp;
	m_optionTearing.userEnabled = tearingEnabledTemp;
	m_optionRayTracing.userEnabled = rayTracingEnabledTemp;
	m_optionMeshShader.userEnabled = meshShaderEnabledTemp;
	m_optionSound.masterVolume = soundMasterVolumeTemp;
	m_optionSound.uiVolume = soundUIVolumeTemp;
	m_optionSound.unitVolume = soundUnitVolumeTemp;
	m_optionSound.effectVolume = soundEffectVolumeTemp;
	m_optionSound.musicVolume = soundMusicVolumeTemp;
}

bool Sodo::ReadOptionBool(std::ifstream& fin, std::string optionName, bool& outOptionEnabled)
{
	std::string buffer;

	fin >> buffer;
	if (buffer != optionName)
	{
		return false;
	}

	fin >> buffer;
	if (buffer == "Yes")
	{
		outOptionEnabled = true;
	}
	else if (buffer == "No")
	{
		outOptionEnabled = false;
	}
	else
	{
		return false;
	}

	return true;
}

bool Sodo::ReadOptionInt(std::ifstream& fin, std::string optionName, int& outOptionEnabled)
{
	std::string buffer;

	fin >> buffer;
	if (buffer != optionName)
	{
		return false;
	}
	fin >> outOptionEnabled;

	return true;
}
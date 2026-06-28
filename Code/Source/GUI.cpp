#include <windows.h>
#include "imgui.h"
#include "Sodo.h"
#include "Game.h"

void Sodo::RenderGuiInGame(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos)
{
	ImVec2 pos = ImVec2(
		imGuiViewPort->Pos.x,
		imGuiViewPort->Pos.y + imGuiViewPort->Size.y * 0.75f
	);

	ImVec2 size = ImVec2(
		imGuiViewPort->Size.x,
		imGuiViewPort->Size.y * 0.25f
	);

	ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(size, ImGuiCond_Always);

	ImGui::Begin("In game", nullptr, m_imGuiBasicFlag);

	ImGui::Dummy(m_imGuiSpacingSize);

	if (ImGui::Button("Menu", m_imGuiSmallButtonSize))
	{
		m_gameMode = GAME_STATE_PAUSED;
	}

	ImGui::End();
}

void Sodo::RenderGuiLobbyMenu(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos)
{
	ImGui::SetNextWindowPos(imGuiCenterPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(250.0f, 330.0f), ImGuiCond_Always);

	ImGui::Begin("Lobby", nullptr, m_imGuiBasicFlag);

	ImGui::Dummy(m_imGuiSpacingSize);

	if (ImGui::Button("Play", m_imGuiSmallButtonSize))
	{
		m_gameMode = GAME_STATE_LOADING_TO_GAME;
	}

	ImGui::Dummy(m_imGuiSpacingSize);
	ImGui::Separator();
	ImGui::Dummy(m_imGuiSpacingSize);

	if (ImGui::Button("Option", m_imGuiSmallButtonSize))
	{
		m_gameMode = GAME_STATE_OPTION_FROM_LOBBY;
	}

	ImGui::Dummy(m_imGuiSpacingSize);
	ImGui::Separator();
	ImGui::Dummy(m_imGuiSpacingSize);

	if (ImGui::Button("Exit", m_imGuiSmallButtonSize))
	{
		m_gameMode = GAME_STATE_CHECK_EXIT_FROM_LOBBY_TO_WINDOWS;
	}

	ImGui::End();
}

void Sodo::RenderGuiPausedMenu(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos)
{
	ImGui::SetNextWindowPos(imGuiCenterPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(300.0f, 430.0f), ImGuiCond_Always);

	ImGui::Begin("Paused", nullptr, m_imGuiBasicFlag);

	ImGui::Dummy(m_imGuiSpacingSize);

	if (ImGui::Button("Resume", m_imGuiSmallButtonSize))
	{
		m_gameMode = GAME_STATE_IN_GAME;
	}

	ImGui::Dummy(m_imGuiSpacingSize);
	ImGui::Separator();
	ImGui::Dummy(m_imGuiSpacingSize);

	if (ImGui::Button("Option", m_imGuiSmallButtonSize))
	{
		m_gameMode = GAME_STATE_OPTION_FROM_PAUSED;
	}

	ImGui::Dummy(m_imGuiSpacingSize);
	ImGui::Separator();
	ImGui::Dummy(m_imGuiSpacingSize);

	if (ImGui::Button("Exit to lobby", m_imGuiMediumButtonSize))
	{
		m_gameMode = GAME_STATE_CHECK_EXIT_FROM_PAUSED_TO_LOBBY;
	}

	ImGui::Dummy(m_imGuiSpacingSize);
	ImGui::Separator();
	ImGui::Dummy(m_imGuiSpacingSize);

	if (ImGui::Button("Exit to window", m_imGuiMediumButtonSize))
	{
		m_gameMode = GAME_STATE_CHECK_EXIT_FROM_PAUSED_TO_WINDOWS;
	}

	ImGui::End();
}

void Sodo::RenderGuiLoadingToGame(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos)
{
	ImGuiWindowFlags loadingGuiFlag = m_imGuiBasicFlag | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground;

	ImVec2 pos = ImVec2(
		imGuiCenterPos.x,
		imGuiViewPort->Pos.y + imGuiViewPort->Size.y * 0.9f
	);

	ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(400.0f, 200.0f), ImGuiCond_Always);

	ImGui::Begin("Loading", nullptr, loadingGuiFlag);

	ImGui::Dummy(m_imGuiSpacingSize);

	//TODO : 버튼을 비활성화하고, 로딩이 다 되면 활성화하기
	if (ImGui::Button("Click here to start", m_imguiLargeButtonSize))
	{
		m_gameMode = GAME_STATE_IN_GAME;
	}

	ImGui::End();
}

void Sodo::RenderGuiLoadingToLobby(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos)
{
	ImGuiWindowFlags loadingGuiFlag = m_imGuiBasicFlag | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground;

	ImVec2 pos = ImVec2(
		imGuiCenterPos.x,
		imGuiViewPort->Pos.y + imGuiViewPort->Size.y * 0.9f
	);

	ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(400.0f, 200.0f), ImGuiCond_Always);

	ImGui::Begin("Loading", nullptr, loadingGuiFlag);

	ImGui::Dummy(m_imGuiSpacingSize);

	//TODO : 로비로의 로딩은 완료되면 버튼 없이 자동으로 수행되도록 하기
	if (ImGui::Button("Click here to end", m_imguiLargeButtonSize))
	{
		m_gameMode = GAME_STATE_LOBBY;
	}

	ImGui::End();
}

void Sodo::RenderGuiOptionFromLobby(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos)
{
	ImGui::SetNextWindowPos(imGuiCenterPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(700.0f, 770.0f), ImGuiCond_Always);

	ImGui::Begin("Option", nullptr, m_imGuiBasicFlag);

	if (ImGui::Button("Back", m_imGuiSmallButtonSize))
	{
		m_gameMode = GAME_STATE_LOBBY;
	}

	CommonRenderGuiOption();

	ImGui::End();
}

void Sodo::RenderGuiOptionFromPaused(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos)
{
	ImGui::SetNextWindowPos(imGuiCenterPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(700.0f, 750.0f), ImGuiCond_Always);

	ImGui::Begin("Option", nullptr, m_imGuiBasicFlag);

	if (ImGui::Button("Back", m_imGuiSmallButtonSize))
	{
		m_gameMode = GAME_STATE_PAUSED;
	}

	CommonRenderGuiOption();

	ImGui::End();
}

void Sodo::RenderGuiCheckExitFromLobbyToWindows(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos)
{
	ImGui::SetNextWindowPos(imGuiCenterPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(600.0f, 200.0f), ImGuiCond_Always);

	ImGui::Begin("Check", nullptr, m_imGuiBasicFlag);

	ImGui::Dummy(m_imGuiSpacingSize);

	ImGui::Text("Do you really want to exit to window?");

	CommonRenderGuiCheckExit(GAME_STATE_LOBBY, GAME_STATE_TERMINATE);

	ImGui::End();
}

void Sodo::RenderGuiCheckExitFromPausedToWindows(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos)
{
	ImGui::SetNextWindowPos(imGuiCenterPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(600.0f, 200.0f), ImGuiCond_Always);

	ImGui::Begin("Check", nullptr, m_imGuiBasicFlag);

	ImGui::Dummy(m_imGuiSpacingSize);

	ImGui::Text("Do you really want to exit to window?");

	CommonRenderGuiCheckExit(GAME_STATE_PAUSED, GAME_STATE_TERMINATE);

	ImGui::End();
}

void Sodo::RenderGuiCheckExitFromPausedToLobby(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos)
{
	ImGui::SetNextWindowPos(imGuiCenterPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(600.0f, 200.0f), ImGuiCond_Always);

	ImGui::Begin("Check", nullptr, m_imGuiBasicFlag);

	ImGui::Dummy(m_imGuiSpacingSize);

	ImGui::Text("Do you really want to exit to lobby?");

	CommonRenderGuiCheckExit(GAME_STATE_PAUSED, GAME_STATE_LOADING_TO_LOBBY);

	ImGui::End();
}

void Sodo::CommonRenderGuiOption()
{
	bool previousFullScreenState = m_optionFullScreen.IsActive();
	bool previousHDRState = m_optionHDR.IsActive();

	ImGui::Dummy(m_imGuiSpacingSize);

	ImGui::Text("Display");
	ImGui::Checkbox("Full Screen", &m_optionFullScreen.userEnabled);
	ImGui::BeginDisabled(!m_optionHDR.IsSupported());
	ImGui::Checkbox("HDR", &m_optionHDR.userEnabled);
	ImGui::EndDisabled();
	ImGui::SameLine();
	ImGui::BeginDisabled(!m_optionTearing.IsSupported());
	ImGui::Checkbox("VRR", &m_optionTearing.userEnabled);
	ImGui::EndDisabled();

	ImGui::Dummy(m_imGuiSpacingSize);
	ImGui::Separator();

	ImGui::Text("Graphics");
	ImGui::BeginDisabled(!m_optionRayTracing.IsSupported());
	ImGui::Checkbox("Ray Tracing", &m_optionRayTracing.userEnabled);
	ImGui::EndDisabled();
	ImGui::BeginDisabled(!m_optionMeshShader.IsSupported());
	ImGui::Checkbox("Mesh Shader", &m_optionMeshShader.userEnabled);
	ImGui::EndDisabled();

	ImGui::Dummy(m_imGuiSpacingSize);
	ImGui::Separator();

	ImGui::Text("Sound");
	ImGui::Checkbox("Master", &m_optionSound.userEnabled);
	ImGui::SliderFloat("Main Volume", &m_optionSound.mainVolume, 0.0f, 100.0f, "%.0f%%");
	ImGui::SliderFloat("UI Volume", &m_optionSound.uiVolume, 0.0f, 100.0f, "%.0f%%");
	ImGui::SliderFloat("Effect Volume", &m_optionSound.effectVolume, 0.0f, 100.0f, "%.0f%%");
	ImGui::SliderFloat("Music Volume", &m_optionSound.musicVolume, 0.0f, 100.0f, "%.0f%%");

	bool nowFullScreenState = m_optionFullScreen.IsActive();
	bool nowHDRState = m_optionHDR.IsActive();

	if (previousFullScreenState != nowFullScreenState)
	{
		m_needResetFullScreen = true;
	}
	if (previousHDRState != nowHDRState)
	{
		m_needResetHDR = true;
	}
}

void Sodo::CommonRenderGuiCheckExit(GameState from, GameState to)
{
	ImGui::Dummy(m_imGuiSpacingSize);

	if (ImGui::Button("No", m_imGuiSmallButtonSize))
	{
		m_gameMode = from;
	}

	ImGui::SameLine();

	if (ImGui::Button("Yes", m_imGuiSmallButtonSize))
	{
		if (to == GAME_STATE_TERMINATE)
		{
			DestroyWindow(m_hWnd);
		}
		else
		{
			m_gameMode = to;
		}
	}
}
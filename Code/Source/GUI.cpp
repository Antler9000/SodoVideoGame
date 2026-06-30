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

	bool menuButtonClicked = ImGui::Button("Menu", m_imGuiSmallButtonSize);
	if (menuButtonClicked == true)
	{
		m_gameState = GAME_STATE_PAUSED;
	}

	ImGui::End();
}

void Sodo::RenderGuiLobbyMenu(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos)
{
	ImGui::SetNextWindowPos(imGuiCenterPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(250.0f, 330.0f), ImGuiCond_Always);

	ImGui::Begin("Lobby", nullptr, m_imGuiBasicFlag);

	ImGui::Dummy(m_imGuiSpacingSize);

	bool playButtonClicked = ImGui::Button("Play", m_imGuiSmallButtonSize);
	if (playButtonClicked == true)
	{
		m_gameState = GAME_STATE_LOADING_TO_GAME;
	}

	ImGui::Dummy(m_imGuiSpacingSize);
	ImGui::Separator();
	ImGui::Dummy(m_imGuiSpacingSize);

	bool optionButtonClicked = ImGui::Button("Option", m_imGuiSmallButtonSize);
	if (optionButtonClicked == true)
	{
		m_gameState = GAME_STATE_OPTION_FROM_LOBBY;
	}

	ImGui::Dummy(m_imGuiSpacingSize);
	ImGui::Separator();
	ImGui::Dummy(m_imGuiSpacingSize);

	bool exitButtonClicked = ImGui::Button("Exit", m_imGuiSmallButtonSize);
	if (exitButtonClicked == true)
	{
		m_gameState = GAME_STATE_CHECK_EXIT_FROM_LOBBY_TO_WINDOWS;
	}

	ImGui::End();
}

void Sodo::RenderGuiPausedMenu(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos)
{
	ImGui::SetNextWindowPos(imGuiCenterPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(300.0f, 430.0f), ImGuiCond_Always);

	ImGui::Begin("Paused", nullptr, m_imGuiBasicFlag);

	ImGui::Dummy(m_imGuiSpacingSize);

	bool resumeButtonClicked = ImGui::Button("Resume", m_imGuiSmallButtonSize);
	if (resumeButtonClicked == true)
	{
		m_gameState = GAME_STATE_IN_GAME;
	}

	ImGui::Dummy(m_imGuiSpacingSize);
	ImGui::Separator();
	ImGui::Dummy(m_imGuiSpacingSize);

	bool optionButtonClicked = ImGui::Button("Option", m_imGuiSmallButtonSize);
	if (optionButtonClicked == true)
	{
		m_gameState = GAME_STATE_OPTION_FROM_PAUSED;
	}

	ImGui::Dummy(m_imGuiSpacingSize);
	ImGui::Separator();
	ImGui::Dummy(m_imGuiSpacingSize);

	bool exitToLobbyButtonClicked = ImGui::Button("Exit to lobby", m_imGuiMediumButtonSize);
	if (exitToLobbyButtonClicked == true)
	{
		m_gameState = GAME_STATE_CHECK_EXIT_FROM_PAUSED_TO_LOBBY;
	}

	ImGui::Dummy(m_imGuiSpacingSize);
	ImGui::Separator();
	ImGui::Dummy(m_imGuiSpacingSize);

	bool exitToWindowButtonClicked = ImGui::Button("Exit to window", m_imGuiMediumButtonSize);
	if (exitToWindowButtonClicked == true)
	{
		m_gameState = GAME_STATE_CHECK_EXIT_FROM_PAUSED_TO_WINDOWS;
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

	bool startButtonClicked = ImGui::Button("Click here to start", m_imguiLargeButtonSize);
	if (startButtonClicked == true)
	{
		m_gameState = GAME_STATE_IN_GAME;
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

	bool endButtonClicked = ImGui::Button("Click here to end", m_imguiLargeButtonSize);
	if (endButtonClicked == true)
	{
		m_gameState = GAME_STATE_LOBBY;
	}

	ImGui::End();
}

void Sodo::RenderGuiOptionFromLobby(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos)
{
	ImGui::SetNextWindowPos(imGuiCenterPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(700.0f, 770.0f), ImGuiCond_Always);

	ImGui::Begin("Option", nullptr, m_imGuiBasicFlag);

	bool backButtonClicked = ImGui::Button("Back", m_imGuiSmallButtonSize);
	if (backButtonClicked == true)
	{
		SaveOptions();

		m_gameState = GAME_STATE_LOBBY;
	}

	RenderGuiOptionCommon();

	ImGui::End();
}

void Sodo::RenderGuiOptionFromPaused(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos)
{
	ImGui::SetNextWindowPos(imGuiCenterPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(700.0f, 750.0f), ImGuiCond_Always);

	ImGui::Begin("Option", nullptr, m_imGuiBasicFlag);

	bool backButtonClicked = ImGui::Button("Back", m_imGuiSmallButtonSize);
	if (backButtonClicked == true)
	{
		SaveOptions();

		m_gameState = GAME_STATE_PAUSED;
	}

	RenderGuiOptionCommon();

	ImGui::End();
}

void Sodo::RenderGuiCheckExitFromLobbyToWindows(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos)
{
	ImGui::SetNextWindowPos(imGuiCenterPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(600.0f, 200.0f), ImGuiCond_Always);

	ImGui::Begin("Check", nullptr, m_imGuiBasicFlag);

	ImGui::Dummy(m_imGuiSpacingSize);

	ImGui::Text("Do you really want to exit to window?");

	RenderGuiCheckExitCommon(GAME_STATE_LOBBY, GAME_STATE_TERMINATE);

	ImGui::End();
}

void Sodo::RenderGuiCheckExitFromPausedToWindows(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos)
{
	ImGui::SetNextWindowPos(imGuiCenterPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(600.0f, 200.0f), ImGuiCond_Always);

	ImGui::Begin("Check", nullptr, m_imGuiBasicFlag);

	ImGui::Dummy(m_imGuiSpacingSize);

	ImGui::Text("Do you really want to exit to window?");

	RenderGuiCheckExitCommon(GAME_STATE_PAUSED, GAME_STATE_TERMINATE);

	ImGui::End();
}

void Sodo::RenderGuiCheckExitFromPausedToLobby(ImGuiViewport* imGuiViewPort, ImVec2 imGuiCenterPos)
{
	ImGui::SetNextWindowPos(imGuiCenterPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(600.0f, 200.0f), ImGuiCond_Always);

	ImGui::Begin("Check", nullptr, m_imGuiBasicFlag);

	ImGui::Dummy(m_imGuiSpacingSize);

	ImGui::Text("Do you really want to exit to lobby?");

	RenderGuiCheckExitCommon(GAME_STATE_PAUSED, GAME_STATE_LOADING_TO_LOBBY);

	ImGui::End();
}

void Sodo::RenderGuiOptionCommon()
{
	bool previousFullScreenState = m_optionFullScreen.IsActive();
	bool previousHDRState = m_optionHDR.IsActive();

	ImGui::Dummy(m_imGuiSpacingSize);

	ImGui::Text("Display");
	ImGui::Checkbox("Full Screen", &m_optionFullScreen.userEnabled);
	ImGui::BeginDisabled(m_optionHDR.IsSupported() == false);
	ImGui::Checkbox("HDR", &m_optionHDR.userEnabled);
	ImGui::EndDisabled();
	ImGui::SameLine();
	ImGui::BeginDisabled(m_optionTearing.IsSupported() == false);
	ImGui::Checkbox("VRR", &m_optionTearing.userEnabled);
	ImGui::EndDisabled();

	ImGui::Dummy(m_imGuiSpacingSize);
	ImGui::Separator();

	ImGui::Text("Graphics");
	ImGui::BeginDisabled(m_optionRayTracing.IsSupported() == false);
	ImGui::Checkbox("Ray Tracing", &m_optionRayTracing.userEnabled);
	ImGui::EndDisabled();
	ImGui::BeginDisabled(m_optionMeshShader.IsSupported() == false);
	ImGui::Checkbox("Mesh Shader", &m_optionMeshShader.userEnabled);
	ImGui::EndDisabled();

	ImGui::Dummy(m_imGuiSpacingSize);
	ImGui::Separator();

	ImGui::Text("Sound");
	ImGui::SliderInt("Master Volume", &m_optionSound.masterVolume, 0, 100, "%d%%");
	ImGui::SliderInt("UI Volume", &m_optionSound.uiVolume, 0, 100, "%d%%");
	ImGui::SliderInt("Unit Volume", &m_optionSound.unitVolume, 0, 100, "%d%%");
	ImGui::SliderInt("Effect Volume", &m_optionSound.effectVolume, 0, 100, "%d%%");
	ImGui::SliderInt("Music Volume", &m_optionSound.musicVolume, 0, 100, "%d%%");

	bool nowFullScreenState = m_optionFullScreen.IsActive();
	bool nowHDRState = m_optionHDR.IsActive();

	if (previousFullScreenState != nowFullScreenState)
	{
		m_needResetScreenMode = true;
	}
	if (previousHDRState != nowHDRState)
	{
		m_needResetHDR = true;
	}
}

void Sodo::RenderGuiCheckExitCommon(GameState from, GameState to)
{
	ImGui::Dummy(m_imGuiSpacingSize);

	bool noButtonClicked = ImGui::Button("No", m_imGuiSmallButtonSize);
	if (noButtonClicked == true)
	{
		m_gameState = from;
	}

	ImGui::SameLine();

	bool yesButtonClicked = ImGui::Button("Yes", m_imGuiSmallButtonSize);
	if (yesButtonClicked == true)
	{
		if (to == GAME_STATE_TERMINATE)
		{
			DestroyWindow(m_hWnd);
		}
		else
		{
			m_gameState = to;
		}
	}
}
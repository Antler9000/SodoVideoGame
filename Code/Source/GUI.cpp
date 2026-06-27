#include <windows.h>
#include "imgui.h"
#include "Sodo.h"
#include "Game.h"

//TODO : 버튼을 누를 때마다 효과 사운드를 주자
void Sodo::RenderGuiInGame(ImGuiViewport* viewPort, ImVec2 centerPos)
{
	ImVec2 pos = ImVec2(
		viewPort->Pos.x,
		viewPort->Pos.y + viewPort->Size.y * 0.75f
	);

	ImVec2 size = ImVec2(
		viewPort->Size.x,
		viewPort->Size.y * 0.25f
	);

	ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(size, ImGuiCond_Always);

	ImGui::Begin("In game", nullptr, m_basicGuiFlag);

	ImGui::Dummy(m_blankSize);

	if (ImGui::Button("Menu", m_smallButtonSize))
	{
		m_gameMode = GAME_STATE_PAUSED;
	}

	ImGui::End();
}

void Sodo::RenderGuiLobbyMenu(ImGuiViewport* viewPort, ImVec2 centerPos)
{
	ImGui::SetNextWindowPos(centerPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(250.0f, 330.0f), ImGuiCond_Always);

	ImGui::Begin("Lobby", nullptr, m_basicGuiFlag);

	ImGui::Dummy(m_blankSize);

	if (ImGui::Button("Play", m_smallButtonSize))
	{
		m_gameMode = GAME_STATE_LOADING_TO_GAME;
	}

	ImGui::Dummy(m_blankSize);
	ImGui::Separator();
	ImGui::Dummy(m_blankSize);

	if (ImGui::Button("Option", m_smallButtonSize))
	{
		m_gameMode = GAME_STATE_OPTION_FROM_LOBBY;
	}

	ImGui::Dummy(m_blankSize);
	ImGui::Separator();
	ImGui::Dummy(m_blankSize);

	if (ImGui::Button("Exit", m_smallButtonSize))
	{
		m_gameMode = GAME_STATE_CHECK_EXIT_FROM_LOBBY_TO_WINDOWS;
	}

	ImGui::End();
}

void Sodo::RenderGuiPausedMenu(ImGuiViewport* viewPort, ImVec2 centerPos)
{
	ImGui::SetNextWindowPos(centerPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(300.0f, 430.0f), ImGuiCond_Always);

	ImGui::Begin("Paused", nullptr, m_basicGuiFlag);

	ImGui::Dummy(m_blankSize);

	if (ImGui::Button("Resume", m_smallButtonSize))
	{
		m_gameMode = GAME_STATE_IN_GAME;
	}

	ImGui::Dummy(m_blankSize);
	ImGui::Separator();
	ImGui::Dummy(m_blankSize);

	if (ImGui::Button("Option", m_smallButtonSize))
	{
		m_gameMode = GAME_STATE_OPTION_FROM_PAUSED;
	}

	ImGui::Dummy(m_blankSize);
	ImGui::Separator();
	ImGui::Dummy(m_blankSize);

	if (ImGui::Button("Exit to lobby", m_mediumButtonSize))
	{
		m_gameMode = GAME_STATE_CHECK_EXIT_FROM_PAUSED_TO_LOBBY;
	}

	ImGui::Dummy(m_blankSize);
	ImGui::Separator();
	ImGui::Dummy(m_blankSize);

	if (ImGui::Button("Exit to window", m_mediumButtonSize))
	{
		m_gameMode = GAME_STATE_CHECK_EXIT_FROM_PAUSED_TO_WINDOWS;
	}

	ImGui::End();
}

void Sodo::RenderGuiLoadingToGame(ImGuiViewport* viewPort, ImVec2 centerPos)
{
	ImGuiWindowFlags loadingGuiFlag = m_basicGuiFlag | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground;

	ImVec2 pos = ImVec2(
		centerPos.x,
		viewPort->Pos.y + viewPort->Size.y * 0.9f
	);

	ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(400.0f, 200.0f), ImGuiCond_Always);

	ImGui::Begin("Loading", nullptr, loadingGuiFlag);

	ImGui::Dummy(m_blankSize);

	//TODO : 버튼을 비활성화하고, 로딩이 다 되면 활성화하기
	if (ImGui::Button("Press here to start", m_largeButtonSize))
	{
		m_gameMode = GAME_STATE_IN_GAME;
	}

	ImGui::End();
}

void Sodo::RenderGuiLoadingToLobby(ImGuiViewport* viewPort, ImVec2 centerPos)
{
	ImGuiWindowFlags loadingGuiFlag = m_basicGuiFlag | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground;

	ImVec2 pos = ImVec2(
		centerPos.x,
		viewPort->Pos.y + viewPort->Size.y * 0.9f
	);

	ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(400.0f, 200.0f), ImGuiCond_Always);

	ImGui::Begin("Loading", nullptr, loadingGuiFlag);

	ImGui::Dummy(m_blankSize);

	//TODO : 로비로의 로딩은 완료되면 버튼 없이 자동으로 수행되도록 하기
	if (ImGui::Button("Press here to end", m_largeButtonSize))
	{
		m_gameMode = GAME_STATE_LOBBY;
	}

	ImGui::End();
}

void Sodo::RenderGuiOptionFromLobby(ImGuiViewport* viewPort, ImVec2 centerPos)
{
	ImGui::SetNextWindowPos(centerPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(700.0f, 770.0f), ImGuiCond_Always);

	ImGui::Begin("Option", nullptr, m_basicGuiFlag);

	if (ImGui::Button("Back", m_smallButtonSize))
	{
		m_gameMode = GAME_STATE_LOBBY;
	}

	CommonRenderGuiOption();

	ImGui::End();
}

void Sodo::RenderGuiOptionFromPaused(ImGuiViewport* viewPort, ImVec2 centerPos)
{
	ImGui::SetNextWindowPos(centerPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(700.0f, 750.0f), ImGuiCond_Always);

	ImGui::Begin("Option", nullptr, m_basicGuiFlag);

	if (ImGui::Button("Back", m_smallButtonSize))
	{
		m_gameMode = GAME_STATE_PAUSED;
	}

	CommonRenderGuiOption();

	ImGui::End();
}

void Sodo::RenderGuiCheckExitFromLobbyToWindows(ImGuiViewport* viewPort, ImVec2 centerPos)
{
	ImGui::SetNextWindowPos(centerPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(600.0f, 200.0f), ImGuiCond_Always);

	ImGui::Begin("Check", nullptr, m_basicGuiFlag);

	ImGui::Dummy(m_blankSize);

	ImGui::Text("Do you really want to exit to window?");

	CommonRenderGuiCheckExit(GAME_STATE_LOBBY, GAME_STATE_TERMINATE);

	ImGui::End();
}

void Sodo::RenderGuiCheckExitFromPausedToWindows(ImGuiViewport* viewPort, ImVec2 centerPos)
{
	ImGui::SetNextWindowPos(centerPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(600.0f, 200.0f), ImGuiCond_Always);

	ImGui::Begin("Check", nullptr, m_basicGuiFlag);

	ImGui::Dummy(m_blankSize);

	ImGui::Text("Do you really want to exit to window?");

	CommonRenderGuiCheckExit(GAME_STATE_PAUSED, GAME_STATE_TERMINATE);

	ImGui::End();
}

void Sodo::RenderGuiCheckExitFromPausedToLobby(ImGuiViewport* viewPort, ImVec2 centerPos)
{
	ImGui::SetNextWindowPos(centerPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(600.0f, 200.0f), ImGuiCond_Always);

	ImGui::Begin("Check", nullptr, m_basicGuiFlag);

	ImGui::Dummy(m_blankSize);

	ImGui::Text("Do you really want to exit to lobby?");

	CommonRenderGuiCheckExit(GAME_STATE_PAUSED, GAME_STATE_LOADING_TO_LOBBY);

	ImGui::End();
}

void Sodo::CommonRenderGuiOption()
{
	ImGui::Dummy(m_blankSize);

	//TODO : 화면, 그래픽 옵션 값이 변화할 때마다 그래픽 요소를 초기화하기

	ImGui::Text("Display");
	ImGui::Checkbox("Full Screen", &m_optionFullScreen.userEnabled);
	ImGui::BeginDisabled(!m_optionHDR.IsSupported());
	ImGui::Checkbox("HDR", &m_optionHDR.userEnabled);
	ImGui::EndDisabled();
	ImGui::SameLine();
	ImGui::BeginDisabled(!m_optionTearing.IsSupported());
	ImGui::Checkbox("VRR", &m_optionTearing.userEnabled);
	ImGui::EndDisabled();

	ImGui::Dummy(m_blankSize);
	ImGui::Separator();

	ImGui::Text("Graphics");
	ImGui::BeginDisabled(!m_optionRayTracing.IsSupported());
	ImGui::Checkbox("Ray Tracing", &m_optionRayTracing.userEnabled);
	ImGui::EndDisabled();
	ImGui::BeginDisabled(!m_optionMeshShader.IsSupported());
	ImGui::Checkbox("Mesh Shader", &m_optionMeshShader.userEnabled);
	ImGui::EndDisabled();

	ImGui::Dummy(m_blankSize);
	ImGui::Separator();

	ImGui::Text("Sound");
	ImGui::Checkbox("Master", &m_optionSound.userEnabled);
	ImGui::SliderFloat("Main Volume", &m_optionSound.mainVolume, 0.0f, 100.0f, "%.0f%%");
	ImGui::SliderFloat("UI Volume", &m_optionSound.uiVolume, 0.0f, 100.0f, "%.0f%%");
	ImGui::SliderFloat("Effect Volume", &m_optionSound.effectVolume, 0.0f, 100.0f, "%.0f%%");
	ImGui::SliderFloat("Music Volume", &m_optionSound.musicVolume, 0.0f, 100.0f, "%.0f%%");
}

void Sodo::CommonRenderGuiCheckExit(GameState from, GameState to)
{
	ImGui::Dummy(m_blankSize);

	if (ImGui::Button("No", m_smallButtonSize))
	{
		m_gameMode = from;
	}

	ImGui::SameLine();

	if (ImGui::Button("Yes", m_smallButtonSize))
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
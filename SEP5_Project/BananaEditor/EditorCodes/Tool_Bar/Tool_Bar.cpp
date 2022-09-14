/******************************************************************************/
/*!
\file       Tool_Bar.cpp
\par        Project:
\author     Pham Thao Mien
\date       12 November 2021
\brief      This header file contains the handling of changing of scene state
			when the tool bar functions are being clicked.

\copyright	Copyright (C) 2021 DigiPen Institute of Technology.
			Reproduction or disclosure of this file or its contents without the
			prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Tool_Bar.h"
#include "../EditorSystem/EditorSystem.h"

namespace BananaEditor
{
	Tool_Bar* Tool_Bar::p_toolbar = nullptr;

	Tool_Bar::Tool_Bar() : m_scenestate{ SceneState::Pause } {}
	Tool_Bar::~Tool_Bar() {}

	void Tool_Bar::SetSceneState(SceneState state)
	{
		m_scenestate = state;
		if (m_scenestate == SceneState::Pause)
			button = play;
		else
			button = pause;
	}

	Tool_Bar* Tool_Bar::init()
	{
		p_toolbar = new Tool_Bar();
		return p_toolbar;
	}

	void Tool_Bar::update()
	{
		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration |
										   ImGuiWindowFlags_NoScrollbar |
										   ImGuiWindowFlags_NoScrollWithMouse);

		float size = 40.0f;
		//ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		auto MidPoint = ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x / 2.0f);
		auto StartCursorPosition = MidPoint - (2 * (50 + ImGui::GetStyle().ItemSpacing.x));
		ImGui::SetCursorScreenPos({ StartCursorPosition, ImGui::GetCursorScreenPos().y });

		if (ImGui::Button(ICON_FA_SAVE, ImVec2(size, size)))
		{
			//ImGui::SetTooltip
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("SAVE");

		ImGui::SameLine();

		if (ImGui::Button(ICON_FA_UNDO, ImVec2(size, size)))
		{
			EditorSystem::command->undo();
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("UNDO");

		ImGui::SameLine();

		if (ImGui::Button(ICON_FA_PLAY, ImVec2(size, size)))
		{
			//ImGui::SetTooltip("PLAY");
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("PLAY");

		ImGui::SameLine();

		if (ImGui::Button(ICON_FA_PAUSE, ImVec2(size, size)))
		{
			ImGui::SetTooltip("PAUSE");
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("PAUSE");

		ImGui::End();
	}

	void Tool_Bar::Shutdown()
	{
		delete p_toolbar;
	}
}

/******************************************************************************/
/*!
\file		Menu_Windows.cpp
\par        Project:
\author		Pham Thao Mien
\date   	October 7, 2022
\brief		Main Menu options

\copyright	Copyright (C) 2022 DigiPen Institute of Technology.
			Reproduction or disclosure of this file or its contents without the
			prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Menu_Window.h"

namespace BananaEditor
{
	bool save_allAssets = true;
	bool save = false;

	Menu_Window* Menu_Window::p_menu = nullptr;

	Menu_Window::Menu_Window() : showConsoleWindow{ true }, showFPSWindow{ true }, showResourcesWindow{ true },
		showEHierarWindow{ true }, showUHierarWindow{ true }, showEInspectWindow{ true }, showUInspectWindow{ true },
		showSceneWindow{ true }, showEditorGraphicsWindow{ true }, showPlayerGraphicsWindow{ true }, showAssetsWindow{ true },
		showEntityTagWindow{ true }
	{
	}

	Menu_Window::~Menu_Window()
	{
		
	}

	Menu_Window* Menu_Window::init()
	{
		p_menu = new Menu_Window();
		return p_menu;
	}

	void Menu_Window::update()
	{
		/*if ((SH::Input->IsKeyPressed(SH::Key::LEFT_CONTROL) || SH::Input->IsKeyPressed(SH::Key::RIGHT_CONTROL)) && SH::Input->IsKeyPressed(SH::Key::_S))
		{
			ImGui::OpenPopup("Save Current Scene?");
		}*/

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save Current Scene"))
				{
					save = true;
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Style"))
			{
				if (ImGui::MenuItem("Classic"))
				{
					ImGui::StyleColorsClassic();
				}
				if (ImGui::MenuItem("Dark"))
				{
					ImGui::StyleColorsDark();
				}
				if (ImGui::MenuItem("Light"))
				{
					ImGui::StyleColorsLight();
				}
				
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::MenuItem("Open Console"))
				{
					showConsoleWindow = true;
				}
				if (ImGui::MenuItem("Open FPS"))
				{
					showFPSWindow = true;
				}
				if (ImGui::MenuItem("Open Resources"))
				{
					showResourcesWindow = true;
				}
				if (ImGui::MenuItem("Open Entity Hierarchy"))
				{
					showEHierarWindow = true;
				}
				if (ImGui::MenuItem("Open UI Hierarchy"))
				{
					showUHierarWindow = true;
				}
				if (ImGui::MenuItem("Open Entity Inspector"))
				{
					showEInspectWindow = true;
				}
				if (ImGui::MenuItem("Open UI Inspector"))
				{
					showUInspectWindow = true;
				}
				if (ImGui::MenuItem("Open Scene List"))
				{
					showSceneWindow = true;
				}
				if (ImGui::MenuItem("Open Editor View"))
				{
					showEditorGraphicsWindow = true;
				}
				if (ImGui::MenuItem("Open Player View"))
				{
					showPlayerGraphicsWindow = true;
				}
				if (ImGui::MenuItem("Open Assets"))
				{
					showAssetsWindow = true;
				}
				if (ImGui::MenuItem("Open Entity Tag"))
				{
					showEntityTagWindow = true;
				}
				
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		/*if (save)
		{
			ImGui::OpenPopup("Save Current Scene?");
			save = false;
		}

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("Save Current Scene?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Are you sure you want to save current scene?");
			ImGui::Separator();

			ImGui::Checkbox("Save All Imported Assets?", &save_allAssets);
			ImGui::Text("");

			if (ImGui::Button("Save"))
			{
				SH::sceneID id = SH::ScM->GetCurrScene();
				std::string name = SH::ScM->GetSceneNameFromList(id);

				SH::ScM->SaveScene(name, save_allAssets);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}*/
	}

	void Menu_Window::Shutdown()
	{
		delete p_menu;
	}
}

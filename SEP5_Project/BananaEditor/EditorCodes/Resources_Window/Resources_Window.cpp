/******************************************************************************/
/*!
\file		Resources_Window.cpp
\par        Project:
\author		Pham Thao Mien
\date   	October 7, 2021
\brief		

\copyright	Copyright (C) 2021 DigiPen Institute of Technology.
			Reproduction or disclosure of this file or its contents without the
			prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Resources_Window.h"
#include "../Menu_Window/Menu_Window.h"

namespace BananaEditor
{
	Resources_Window* Resources_Window::p_resource = nullptr;
	std::string Resources_Window::filename{};
	std::string Resources_Window::dragdrop_path{};

	static std::filesystem::path s_AssetPath = "..";

	static std::string sss{};

	Resources_Window::Resources_Window() : m_currentDir{ s_AssetPath }
	{
	}

	Resources_Window::~Resources_Window()
	{
		
	}

	Resources_Window* Resources_Window::init()
	{
		p_resource = new Resources_Window();
		return p_resource;
	}

	void Resources_Window::update()
	{
		if (Menu_Window::Get()->showResourcesWindow)
		{
			if (!begin("Resources", &Menu_Window::Get()->showResourcesWindow))
			{
				end();
			}
			else
			{
				static ImGuiTextFilter filter;
				filter.Draw(ICON_FA_SEARCH);

				if (m_currentDir != std::filesystem::path(s_AssetPath))
				{
					if (ImGui::Button(ICON_FA_LONG_ARROW_ALT_LEFT))
					{
						m_currentDir = m_currentDir.parent_path();
					}
				}

				static float padding = 5.0f;
				static float thumbnailSize = 70;
				float cellSize = thumbnailSize + padding;
				float panelWidth = ImGui::GetContentRegionAvail().x;
				int columnCount = (int)(panelWidth / cellSize);
				if (columnCount < 1)
				{
					columnCount = 1;
				}

				ImGui::Columns(columnCount, 0, false);

				for (auto& dirEntry : std::filesystem::directory_iterator(m_currentDir))
				{
					const auto& path = dirEntry.path();
					auto relativePath = std::filesystem::relative(path, s_AssetPath);
					std::string fileNameString = relativePath.filename().string();

					if (filter.PassFilter(fileNameString.c_str()))
					{
						ImGui::PushID(fileNameString.c_str());

						std::string new_string = fileNameString.substr(fileNameString.find(".") + 1);

						const char* file = ICON_FA_FOLDER;

						if (new_string != "")
						{
							if (new_string == "lua")
							{
								file = ICON_FA_FILE_CODE;
							}
							if (new_string == "wav")
							{
								file = ICON_FA_FILE_AUDIO;
							}
							if (new_string == "png")
							{
								file = ICON_FA_FILE_IMAGE;
							}
						}

						ImGui::Button(file, { thumbnailSize, thumbnailSize });

						if (ImGui::BeginDragDropSource())
						{
							// Update clicked file's name
							filename = relativePath.string();

							ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", NULL, 0);

							ImGui::Text(fileNameString.c_str());

							sss = fileNameString;

							ImGui::EndDragDropSource();
						}

						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							if (dirEntry.is_directory())
							{
								m_currentDir /= path.filename();
							}
						}

						ImGui::TextWrapped(fileNameString.c_str());

						ImGui::PopID();

						ImGui::NextColumn();
					}
				}

				ImGui::Columns(1);
		
				dragdrop_path = m_currentDir.u8string() + "\\" + sss;

				ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 1, 100);
				ImGui::SliderFloat("Padding", &padding, 0, 32);

				end();
			}
		}
	}

	void Resources_Window::Shutdown()
	{
		delete p_resource;
	}
}

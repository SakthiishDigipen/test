/******************************************************************************/
/*!
\file		Assets_Window.cpp
\par        Project: Candy Crusade
\author		Pham Thao Mien
\date   	October 7, 2021
\brief		Get all game resources into the engine

\copyright	Copyright (C) 2021 DigiPen Institute of Technology.
			Reproduction or disclosure of this file or its contents without the
			prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Assets_Window.h"
#include "../Menu_Window/Menu_Window.h"
#include "../Resources_Window/Resources_Window.h"

namespace BananaEditor
{
	Assets_Window* Assets_Window::p_asset = nullptr;

	std::string Assets_Window::prefabs_filepath{};

	Assets_Window::Assets_Window()
	{
		//_mode = SH::AudioMode::audioLoopOff;
	}

	Assets_Window::~Assets_Window()
	{
		
	}

	Assets_Window* Assets_Window::init()
	{
		p_asset = new Assets_Window();
		return p_asset;
	}

	void Assets_Window::update()
	{
		if (Menu_Window::Get()->showAssetsWindow)
		{
			if (!begin("Assets", &Menu_Window::Get()->showAssetsWindow))
			{
				end();
			}
			else
			{
				//auto GetFileTypes = BE::AssetMgr->GetFileTypes();

				//for (auto const& x : GetFileTypes)
				//{
				//	//std::cout << x.first << ':' << x.second << std::endl;
				//}

				static float padding = 5.0f;
				static float thumbnailSize = 70;
				float cellSize = thumbnailSize + padding;
				float panelWidth = ImGui::GetContentRegionAvail().x;
				int columnCount = (int)(panelWidth / cellSize);
				if (columnCount < 1)
				{
					columnCount = 1;
				}

				static char file_path[100] = { 0 };
				ImGui::InputText("File Path", file_path, IM_ARRAYSIZE(file_path));

				//Drag and Drop
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload * payload{ ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM") })
					{
						strcpy_s(file_path, Resources_Window::getdragdrop_path().c_str());
					}
					ImGui::EndDragDropTarget();
				}

				if (ImGui::Button(ICON_FA_PLUS_SQUARE " Load Asset"))
				{
					//BE::AssetMgr->LoadAsset(file_path);
				}

				auto GetAllLoadedAssets = BE::AssetMgr->GetAllLoadedAssets();

				if (ImGui::BeginTabBar("##tab"))
				{
					if (ImGui::BeginTabItem("Audio"))
					{
						ImGui::Columns(columnCount, 0, false);

						for (auto const& x : GetAllLoadedAssets)
						{
							//std::cout << x.first << ':' << x.second << std::endl;
							//auto GetAssetTypeID = BE::AssetMgr->GetAssetTypeIDOf(x.first);

							//if(GetAssetTypeID == Audio ID)
							{
								ImGui::Button(ICON_FA_FILE_AUDIO, { thumbnailSize, thumbnailSize });

								if (ImGui::BeginPopupContextItem(x.second.c_str()))
								{
									if (ImGui::Button(ICON_FA_TRASH " Unload Asset"))
									{
										ImGui::OpenPopup("Remove Asset?");
									}

									ImVec2 center = ImGui::GetMainViewport()->GetCenter();
									ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

									if (ImGui::BeginPopupModal("Remove Asset?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
									{
										RemoveAsset_Popup(x.first);
									}

									ImGui::EndPopup();
								}

								ImGui::OpenPopupOnItemClick(x.second.c_str(), ImGuiPopupFlags_MouseButtonRight);

								std::string TruncateFileName = BE::AssetMgr->GetAssetTruncatedFileName(x.first);
								if (ImGui::IsItemHovered())
								{
									ImGui::BeginTooltip();
									ImGui::Text(TruncateFileName.c_str());
									ImGui::EndTooltip();
								}

								ImGui::TextWrapped(TruncateFileName.c_str());

								ImGui::NextColumn();
							}
						}

						/*ImGui::Separator();

						auto AllAudioFiles = SH::AssetMgr->GetAllAudioFiles();

						

						for (auto& it : AllAudioFiles)
						{
							std::string fileNameString = it.first;
							std::string new_string = fileNameString.substr(fileNameString.find(".") + 1);

							ImGui::Button(ICON_FA_FILE_AUDIO, { thumbnailSize, thumbnailSize });

							if (ImGui::BeginPopupContextItem(fileNameString.c_str()))
							{
								if (ImGui::Button(ICON_FA_TRASH " Remove Audio"))
								{
									ImGui::OpenPopup("Remove Asset?");
								}

								ImVec2 center = ImGui::GetMainViewport()->GetCenter();
								ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

								if (ImGui::BeginPopupModal("Remove Asset?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
								{
									RemoveAsset_Popup(it.first, "Audio");
								}

								ImGui::EndPopup();
							}

							ImGui::OpenPopupOnItemClick(fileNameString.c_str(), ImGuiPopupFlags_MouseButtonRight);

							if (ImGui::IsItemHovered())
							{
								ImGui::BeginTooltip();
								ImGui::Text(it.first.c_str());
								ImGui::EndTooltip();
							}

							//ImGui::TextWrapped(fileNameString.substr(fileNameString.find_last_of("/\\") + 1).c_str());

							ImGui::NextColumn();
						}*/

						ImGui::Columns(1);

						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Texture"))
					{
						ImGui::Columns(1);

						ImGui::EndTabItem();
					}

					ImGui::EndTabBar();
				}

				ImGui::Separator();
				ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 1, 100);
				ImGui::SliderFloat("Padding", &padding, 0, 32);

				end();
			}
		}
	}

	void Assets_Window::Shutdown()
	{
		delete p_asset;
	}

	void Assets_Window::RemoveAsset_Popup(BE::AssetID _assetID)
	{
		std::string file_name = BE::AssetMgr->GetAssetTruncatedFileName(_assetID);
		ImGui::Text("Are you sure you want to unload: %s?", file_name.c_str());
		ImGui::Separator();

		if (ImGui::Button("Remove"))
		{
			BE::AssetMgr->UnloadAsset(_assetID, true);

			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

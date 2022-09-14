/******************************************************************************/
/*!
\file		Inspector_Window.cpp
\par        Project:
\author		Pham Thao Mien
\date   	October 7, 2021
\brief		Inspector window for each game object

\copyright	Copyright (C) 2021 DigiPen Institute of Technology.
			Reproduction or disclosure of this file or its contents without the
			prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Inspector_Window.h"
#include "../Hierarchy_Window/Hierarchy_Window.h"
#include "../Menu_Window/Menu_Window.h"
//#include "../Resources_Window/Resources_Window.h"
//#include "../Graphics_Window/Graphics_Window.h"
#include "../EntityCommand/EntityCommand.h"
#include "../EditorSystem/EditorSystem.h"

namespace BananaEditor
{
	Inspector_Window* Inspector_Window::p_insp = nullptr;

	Inspector_Window::Inspector_Window()
	{
	}

	Inspector_Window::~Inspector_Window()
	{
	}

	Inspector_Window* Inspector_Window::init()
	{
		p_insp = new Inspector_Window();
		return p_insp;
	}

	void Inspector_Window::update()
	{
		if (Menu_Window::Get()->showEInspectWindow)
		{
			if (!begin("Entity Inspector", &Menu_Window::Get()->showEInspectWindow))
			{
				end();
			}
			else
			{
				bool isusing = Hierarchy_Window::Get()->isusing;

				if (isusing)
				{
					if (BE::ECS->GetAllEntities().size() != 0)
					{
						BE::EntityID x = Hierarchy_Window::Get()->id;

						const std::string& get_name = BE::ECS->GetComponent<BE::EntityInfo>(x).GetName();
						const char* cstr = get_name.data();
						char* name = (char*)(cstr);

						std::string s_name = ICON_FA_CUBE " " + get_name;
						ImGui::Text(s_name.c_str());

						if (ImGui::InputText("Entity Name", name, 50))
						{
							BE::ECS->GetComponent<BE::EntityInfo>(x).SetName(name);
						}

						ImGui::Separator();

						if (BE::ECS->HasComponent<BE::EntityInfo>(x))
						{
							if (ImGui::TreeNode("Entity Info Component"))
							{
								//auto getTags = BE::ECS->GetComponent<BE::EntityInfo>(x).GetAllTags();
								ImGui::Text("List of Tags");
								const auto& getAllTags = BE::ECS->GetComponent<BE::EntityInfo>(x).GetAllTags();

								if (ImGui::BeginListBox("Tags"))
								{
									static int selected;
									int i = 0;

									for (const auto& e : getAllTags)
									{
										std::string tagName = BE::ECS->GetTag(e);
										if (ImGui::Selectable(tagName.c_str(), selected == i))
										{
											selected = i;
										}

										if (ImGui::BeginPopupContextItem(tagName.c_str()))
										{
											if (ImGui::Button("Remove Tag"))
											{
												BE::ECS->GetComponent<BE::EntityInfo>(x).RemoveTag(tagName);
											}

											ImGui::EndPopup();
										}
										++i;
									}

									ImGui::EndListBox();
								}

								ImGui::Text("");
								if (ImGui::TreeNode("Add Tag to the Entity"))
								{
									const auto& AllTags = BE::ECS->GetAllTags();
									static std::string add_tag;

									if (ImGui::BeginListBox("##Tags"))
									{
										static int add_selected;
										int b = 0;

										for (const auto& a : AllTags)
										{
											if (ImGui::Selectable(a.first.c_str(), add_selected == b))
											{
												add_selected = b;
												add_tag = a.first;
											}
											++b;
										}

										ImGui::EndListBox();
									}

									if (ImGui::Button("Add"))
									{
										BE::ECS->GetComponent<BE::EntityInfo>(x).AddTag(add_tag);
									}

									ImGui::TreePop();
								}

								ImGui::Text("");

								bool flagActive = BE::ECS->GetComponent<BE::EntityInfo>(x).HasFlag(BE::EntityFlag::active);
								if (ImGui::Checkbox("Entity Active?", &flagActive))
								{
									BE::ECS->GetComponent<BE::EntityInfo>(x).SetFlag(BE::EntityFlag::active, flagActive);
								}

								bool flagUpdateWhenPaused = BE::ECS->GetComponent<BE::EntityInfo>(x).HasFlag(BE::EntityFlag::updateWhenPaused);
								if (ImGui::Checkbox("Update When Paused?", &flagUpdateWhenPaused))
								{
									BE::ECS->GetComponent<BE::EntityInfo>(x).SetFlag(BE::EntityFlag::updateWhenPaused, flagUpdateWhenPaused);
								}

								bool flagUpdateWhenNotVisible = BE::ECS->GetComponent<BE::EntityInfo>(x).HasFlag(BE::EntityFlag::updateWhenNotVisible);
								if (ImGui::Checkbox("Update When Not Visible?", &flagUpdateWhenNotVisible))
								{
									BE::ECS->GetComponent<BE::EntityInfo>(x).SetFlag(BE::EntityFlag::updateWhenNotVisible, flagUpdateWhenNotVisible);
								}

								bool dontDestroyOnSceneChange = BE::ECS->GetComponent<BE::EntityInfo>(x).HasFlag(BE::EntityFlag::dontDestroyOnSceneChange);
								if (ImGui::Checkbox("Dont Destroy On Scene Change?", &dontDestroyOnSceneChange))
								{
									BE::ECS->GetComponent<BE::EntityInfo>(x).SetFlag(BE::EntityFlag::dontDestroyOnSceneChange, dontDestroyOnSceneChange);
								}

								ImGui::TreePop();
							}
						}

						ImGui::Separator();

						if (BE::ECS->HasComponent<BE::Transform>(x))
						{
							if (ImGui::TreeNode("Transform Component"))
							{
								float pos[] = { BE::ECS->GetComponent<BE::Transform>(x).GetPositon().x,
												BE::ECS->GetComponent<BE::Transform>(x).GetPositon().y,
												BE::ECS->GetComponent<BE::Transform>(x).GetPositon().z };
								if (ImGui::DragFloat3("Position", pos))
								{
									BE::ECS->GetComponent<BE::Transform>(x).SetPosition(BE::Vec3f{ pos[0], pos[1], pos[2] });
								}
								
								float scale[] = { BE::ECS->GetComponent<BE::Transform>(x).GetScale().x,
												BE::ECS->GetComponent<BE::Transform>(x).GetScale().y,
												BE::ECS->GetComponent<BE::Transform>(x).GetScale().z };
								if (ImGui::DragFloat3("Scale", scale))
								{
									BE::ECS->GetComponent<BE::Transform>(x).SetScale(BE::Vec3f{ scale[0], scale[1], scale[2] });
								}

								float rot[] = { BE::ECS->GetComponent<BE::Transform>(x).GetRotation().GetEulerAngles().x, //pitch
												BE::ECS->GetComponent<BE::Transform>(x).GetRotation().GetEulerAngles().y, //yaw
												BE::ECS->GetComponent<BE::Transform>(x).GetRotation().GetEulerAngles().z }; //roll
								if (ImGui::DragFloat3("Rotation", rot))
								{
									BE::ECS->GetComponent<BE::Transform>(x).SetRotation(BE::Quaternion(rot[0], rot[1], rot[2]));
								}

								ImGui::TreePop();
							}

							ImGui::Separator();
						}

						ImGui::Text("");
						ImGui::Text("Add/ Remove Component");

						const char* items[] = { "AudioSource", "Rigidbody", "MeshRenderer", "Collider", "GameLogic", "TextRenderer", "Camera", "ParticleSystem" };
						static bool selected[8];

						static std::string preview_combo;

						if (ImGui::BeginCombo("Components", preview_combo.c_str()))
						{
							preview_combo = "";

							std::vector<std::string> vec;

							for (int i = 0; i < IM_ARRAYSIZE(items); ++i)
							{
								ImGui::Selectable(items[i], &selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);

								if (selected[i])
								{
									vec.push_back(items[i]);
								}
							}

							for (int i = 0; i < vec.size(); ++i)
							{
								if (vec.size() == 1)
								{
									preview_combo += vec.at(i);
								}
								else if (!(i == vec.size() - 1))
								{
									preview_combo += vec.at(i) + ",";
								}
								else
								{
									preview_combo += vec.at(i);
								}
							}

							ImGui::EndCombo();
						}

						if (ImGui::Button(ICON_FA_PLUS_SQUARE " Add"))
						{
							//if (selected[0])
							//{
							//	if (!BE::ECS->HasComponent<BE::Transform>(x))
							//	{
							//		//BE::ECS->AddComponent(x, BE::Transform{});
							//		std::shared_ptr<AddComponentCommand<BE::Transform>> p = std::make_shared<AddComponentCommand<BE::Transform>>();
							//		p->setID(x);
							//		EditorSystem::command->executeCmd(p);
							//	}
							//}
							if (selected[0])
							{
								/*if (!BE::ECS->HasComponent<BE::AudioSource>(x) && BE::ECS->HasComponent<BE::Transform>(x))
								{
									BE::ECS->AddComponent(x, BE::AudioSource{});
								}	*/
							}
							if (selected[1])
							{
								/*if (!BE::ECS->HasComponent<BE::Rigidbody>(x) && BE::ECS->HasComponent<BE::Transform>(x))
								{
									BE::ECS->AddComponent(x, BE::Rigidbody{});
								}*/
							}
							if (selected[2])
							{
								/*if (!BE::ECS->HasComponent<BE::MeshRenderer>(x) && BE::ECS->HasComponent<BE::Transform>(x))
								{
									BE::ECS->AddComponent(x, BE::MeshRenderer{});
								}*/
							}
							if (selected[3])
							{
								/*if (!BE::ECS->HasComponent<BE::Collider>(x) && BE::ECS->HasComponent<BE::Transform>(x))
								{
									BE::ECS->AddComponent(x, BE::Collider{});
								}*/
							}
							if (selected[4])
							{
								/*if (!BE::ECS->HasComponent<BE::GameLogicLua>(x) && BE::ECS->HasComponent<BE::Transform>(x))
								{
									BE::ECS->AddComponent(x, BE::GameLogicLua{});
								}*/
							}
							if (selected[5])
							{
								/*if (!BE::ECS->HasComponent<BE::TextRenderer>(x) && BE::ECS->HasComponent<BE::Transform>(x))
								{
									BE::ECS->AddComponent(x, BE::TextRenderer{});
								}*/
							}
							if (selected[6])
							{
								/*if (!BE::ECS->HasComponent<BE::Camera>(x) && BE::ECS->HasComponent<BE::Transform>(x))
								{
									BE::ECS->AddComponent(x, BE::Camera{});
								}*/
							}
							if (selected[7])
							{
								/*if (!BE::ECS->HasComponent<BE::ParticleSystem>(x) && BE::ECS->HasComponent<BE::Transform>(x))
								{
									BE::ECS->AddComponent(x, BE::ParticleSystem{});
								}*/
							}

							preview_combo = "";

							for (auto j = 0; j < IM_ARRAYSIZE(selected); ++j)
							{
								selected[j] = false;
							}
						}

						ImGui::SameLine();

						static std::string remove_comp = "";
						if (ImGui::Button(ICON_FA_TRASH " Remove"))
						{
							/*if (selected[0])
							{
								remove_comp.append("Transform\n");
								
							}*/
							if (selected[0])
							{
								remove_comp.append("AudioSource\n");
							}
							if (selected[1])
							{
								remove_comp.append("Rigidbody\n");
							}
							if (selected[2])
							{
								remove_comp.append("MeshRenderer\n");
							}
							if (selected[3])
							{
								remove_comp.append("Collider\n");
							}
							if (selected[4])
							{
								remove_comp.append("GameLogic\n");
							}
							if (selected[5])
							{
								remove_comp.append("TextRenderer\n");
							}
							if (selected[6])
							{
								remove_comp.append("Camera\n");
							}
							if (selected[7])
							{
								remove_comp.append("ParticleSystem\n");
							}

							ImGui::OpenPopup("Remove Component?");
						}

						ImVec2 center = ImGui::GetMainViewport()->GetCenter();
						ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

						if (ImGui::BeginPopupModal("Remove Component?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
						{
							//Graphics_Window::SetPicking(false);

							ImGui::Text("Are you sure you want to remove the following Components?\n");
							ImGui::Text(remove_comp.c_str());
							ImGui::Separator();

							if (ImGui::Button("Remove"))
							{
								if (selected[0])
								{
									/*if (BE::ECS->HasComponent<BE::AudioSource>(x))
									{
										BE::ECS->RemoveComponent<BE::AudioSource>(x);
									}*/
								}
								if (selected[1])
								{
									/*if (BE::ECS->HasComponent<BE::Rigidbody>(x))
									{
										BE::ECS->RemoveComponent<BE::Rigidbody>(x);
									}*/
								}
								if (selected[2])
								{
									/*if (BE::ECS->HasComponent<BE::MeshRenderer>(x))
									{
										BE::ECS->RemoveComponent<BE::MeshRenderer>(x);
									}*/
								}
								if (selected[3])
								{
									/*if (BE::ECS->HasComponent<BE::Collider>(x))
									{
										BE::ECS->RemoveComponent<BE::Collider>(x);
									}*/
								}
								if (selected[4])
								{
									/*if (BE::ECS->HasComponent<BE::GameLogicLua>(x))
									{
										BE::ECS->RemoveComponent<BE::GameLogicLua>(x);
									}*/
								}
								if (selected[5])
								{
									/*if (BE::ECS->HasComponent<BE::TextRenderer>(x))
									{
										BE::ECS->RemoveComponent<BE::TextRenderer>(x);
									}*/
								}
								if (selected[6])
								{
									/*if (BE::ECS->HasComponent<BE::Camera>(x))
									{
										BE::ECS->RemoveComponent<BE::Camera>(x);
									}*/
								}
								if (selected[7])
								{
									/*if (BE::ECS->HasComponent<BE::ParticleSystem>(x))
									{
										BE::ECS->RemoveComponent<BE::ParticleSystem>(x);
									}*/
								}

								preview_combo = "";
								remove_comp.clear();
								for (auto j = 0; j < IM_ARRAYSIZE(selected); ++j)
								{
									selected[j] = false;
								}

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

				}
				end();
			}
		}
	}

	void Inspector_Window::Shutdown()
	{
		delete p_insp;
	}
}

/******************************************************************************/
/*!
\file		Hierachy_Window.cpp
\par        Project:
\author		Pham Thao Mien
\date   	October 7, 2022
\brief		Show all game objects in the scene

\copyright	Copyright (C) 2022 DigiPen Institute of Technology.
			Reproduction or disclosure of this file or its contents without the
			prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Hierarchy_Window.h"
#include "../Menu_Window/Menu_Window.h"
//#include "../Graphics_Window/Graphics_Window.h"
//#include "../Resources_Window/Resources_Window.h"
//#include <algorithm>
#include "../EntityCommand/EntityCommand.h"
#include "../EditorSystem/EditorSystem.h"

namespace BananaEditor
{
	Hierarchy_Window* Hierarchy_Window::p_hier = nullptr;
	//BE::EntityID Hierarchy_Window::selected_id;
	//std::vector<int> Hierarchy_Window::selected_flag;
	std::vector<BE::EntityID> Hierarchy_Window::selected_id;
	BE::EntityID Hierarchy_Window::id = BE::BEID(0);
	bool Hierarchy_Window::isusing = false;
	bool Hierarchy_Window::isSet = false;

	int SelectedIndex;
	bool clear_selected_entities;

	Hierarchy_Window::Hierarchy_Window() : duplicate_entity{ true }//, isusing{ false }, isSet{false}
	{
	}

	Hierarchy_Window::~Hierarchy_Window()
	{
		
	}

	Hierarchy_Window* Hierarchy_Window::init()
	{
		p_hier = new Hierarchy_Window();
		return p_hier;
	}

	void Hierarchy_Window::update()
	{
		if (Menu_Window::Get()->showEHierarWindow)
		{
			if (!begin("Entity Hierarchy", &Menu_Window::Get()->showEHierarWindow))
			{
				end();
			}
			else
			{
				if (ImGui::Button(ICON_FA_PLUS_SQUARE " Add Entity"))
				{
					//BE::ECS->CreateEntity();

					std::shared_ptr<CreateEntityCommand> p = std::make_shared<CreateEntityCommand>();

					EditorSystem::command->executeCmd(p);
				}

				ImGui::Text("");
				static ImGuiTextFilter filter;
				filter.Draw(ICON_FA_SEARCH);

				const std::vector<BE::EntityID>& LivingEntityArray = BE::ECS->GetAllEntities();

				//static BE::EntityID temp;

				for (BE::EntityID x : LivingEntityArray)
				{
					const std::string& get_name = BE::ECS->GetComponent<BE::EntityInfo>((BE::BEID)x).GetName();

					if (filter.PassFilter(get_name.c_str()))
					{
						std::string name = ICON_FA_CUBE " " + get_name;
						isSet = false;

						if (!BE::ECS->GetComponent<BE::Transform>(x).HasParent())
						{
							ParentChild_Hierarchy(name, x);
						}

						/*if (ImGui::Selectable(name.c_str(), temp == x))
						{
							id = x;
							isusing = true;
							temp = x;
						}*/

						//if (ImGui::BeginPopupContextItem(name.c_str()))
						//{
						//	//if (ImGui::Button(ICON_FA_SAVE " Save Entity Prefab"))
						//	//{
						//	//	Factory::SaveEntityPrefab(x);
						//	//}

						//	if (ImGui::Button(ICON_FA_TRASH " Delete Entity"))
						//	{
						//		ImGui::OpenPopup("Delete Entity?");
						//	}

						//	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
						//	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

						//	if (ImGui::BeginPopupModal("Delete Entity?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
						//	{
						//		Delete_Popup(id);
						//	}

						//	if (ImGui::Button(ICON_FA_COPY " Clone Entity"))
						//	{
						//		ImGui::OpenPopup("Clone Entity?");
						//	}

						//	if (ImGui::BeginPopupModal("Clone Entity?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
						//	{
						//		Hierarchy_Window::Clone_Popup(x, get_name);
						//	}

						//	ImGui::EndPopup();
						//}

						//ImGui::OpenPopupOnItemClick(name.c_str(), ImGuiPopupFlags_MouseButtonRight);
					}
				}

				if((ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL) || ImGui::IsKeyDown(GLFW_KEY_RIGHT_CONTROL)) && ImGui::IsKeyPressed(GLFW_KEY_BACKSPACE))
				//if ((SH::Input->IsKeyPressed(SH::Key::LEFT_CONTROL) || SH::Input->IsKeyPressed(SH::Key::RIGHT_CONTROL)) && SH::Input->IsKeyPressed(SH::Key::BACKSPACE))
				{
					ImGui::OpenPopup("Delete Entity?");
				}
				
				ImVec2 GetCenter = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(GetCenter, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Delete Entity?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					Delete_Popup();
				}

				if ((ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL) || ImGui::IsKeyDown(GLFW_KEY_RIGHT_CONTROL)) && ImGui::IsKeyPressed(GLFW_KEY_D))
				//if ((SH::Input->IsKeyPressed(SH::Key::LEFT_CONTROL) || SH::Input->IsKeyPressed(SH::Key::RIGHT_CONTROL)) && SH::Input->IsKeyPressed(SH::Key::_D))
				{
					//if (duplicate_entity)
					//{
						ImGui::OpenPopup("Clone Entity?");
					//}
				}

				if (ImGui::BeginPopupModal("Clone Entity?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					Clone_Popup();
				}
				
				end();
			}
		}
	}

	void Hierarchy_Window::Shutdown()
	{
		delete p_hier;
	}

	void Hierarchy_Window::Delete_Popup()
	{
		//(void)vec;
		//Graphics_Window::SetPicking(false);

		ImGui::Text("Are you sure you want to delete?");

		for (size_t selectedID = 0; selectedID < selected_id.size(); ++selectedID)
		{
			ImGui::Text(BE::ECS->GetComponent<BE::EntityInfo>(BE::BEID(selected_id[selectedID])).GetName().c_str());
		}
	
		ImGui::Separator();

		if (ImGui::Button("Delete"))
		{
			isusing = false;

			for (size_t selectedID = 0; selectedID < selected_id.size(); ++selectedID)
			{
				//std::cout << selected_id[selectedID] << std::endl;
				BE::ECS->DestroyEntity(selected_id[selectedID]);
			}
			
			if (BE::ECS->GetAllEntities().size() != 0)
			{
				id = BE::ECS->GetAllEntities().front();
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

	void Hierarchy_Window::Clone_Popup()
	{
		//(void)x;
		//(void)get_name;
		//Graphics_Window::SetPicking(false);

		ImGui::Text("Are you sure you want to clone?");

		for (size_t selectedID = 0; selectedID < selected_id.size(); ++selectedID)
		{
			ImGui::Text(BE::ECS->GetComponent<BE::EntityInfo>(BE::BEID(selected_id[selectedID])).GetName().c_str());
		}

		if (ImGui::Button("Clone"))
		{
			for (size_t selectedID = 0; selectedID < selected_id.size(); ++selectedID)
			{
				//BE::ECS->CloneEntity(selected_id[selectedID]);
				std::shared_ptr<CloneEntityCommand> p = std::make_shared<CloneEntityCommand>();
				p->setCloneID(selected_id[selectedID]);
				EditorSystem::command->executeCmd(p);			
			}

			if (BE::ECS->GetAllEntities().size() != 0)
			{
				id = BE::ECS->GetAllEntities().front();
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

	void Hierarchy_Window::ParentChild_Hierarchy(std::string name, BE::EntityID& _x)
	{
		//id = _x;
		////std::cout << id << std::endl;
		//isusing = true;
		bool selected = false;
		int flag = ImGuiTreeNodeFlags_DefaultOpen;
		std::vector<BE::EntityID>::iterator it;

		it = std::find(selected_id.begin(), selected_id.end(), _x);

		if (it != selected_id.end())
		{
			flag |= ImGuiTreeNodeFlags_Selected;
			selected = true;
		}

		const auto& children = BE::ECS->GetComponent<BE::Transform>(_x).GetChildren();

		if (children.size() == 0)
		{
			flag |= ImGuiTreeNodeFlags_Bullet;
		}

		ImGui::PushID((int)((uint64_t)_x));
		
		bool open = ImGui::TreeNodeEx(name.c_str(), flag);
		
		////For each children in children list
		//for (BE::EntityID child : children)
		//{
		//	const std::string& child_get_name = BE::ECS->GetComponent<BE::EntityInfo>(child).GetName();
		//	std::string child_name = ICON_FA_CUBE " " + child_get_name;

		//	//id = child;
		//	//isusing = true;

		//	ParentChild_Hierarchy(child_name, child);
		//}

		//ImGui::TreePop();


		if (ImGui::BeginPopupContextItem(name.c_str()))
		{
			//if (ImGui::Button(ICON_FA_SAVE " Save Entity Prefab"))
			//{
			//	Factory::SaveEntityPrefab(x);
			//}

			if (ImGui::Button(ICON_FA_TRASH " Delete Entity"))
			{
				ImGui::OpenPopup("Delete Entity?");
			}

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("Delete Entity?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				Delete_Popup();
			}

			if (ImGui::Button(ICON_FA_COPY " Clone Entity"))
			{
				ImGui::OpenPopup("Clone Entity?");
			}

			if (ImGui::BeginPopupModal("Clone Entity?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				Hierarchy_Window::Clone_Popup();
			}

			if (BE::ECS->GetComponent<BE::Transform>(_x).HasParent())
			{
				if (ImGui::Button(ICON_FA_CHILD " Detach from Parent"))
				{
					BE::EntityID temp_parent = BE::ECS->GetComponent<BE::Transform>(_x).GetParent();
					BE::Transform& temp = dynamic_cast<BE::Transform&>(BE::ECS->GetComponent("Transform", temp_parent));
					temp.DetachChild(_x);
				}
			}

			ImGui::EndPopup();
		}

		ImGui::OpenPopupOnItemClick(name.c_str(), ImGuiPopupFlags_MouseButtonRight);

		ImGui::PopID();

		if (ImGui::IsItemClicked() && !isSet)
		{
			id = _x;
			//std::cout << id << std::endl;
			isSet = true;
			isusing = true;

			if (!(ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL) || ImGui::IsKeyDown(GLFW_KEY_RIGHT_CONTROL)))
			{
				selected_id.clear();
			}


			if (selected)
			{
				auto a = std::remove(selected_id.begin(), selected_id.end(), _x);
			}
			else
			{
				selected_id.push_back(_x);
			}
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload * payload{ ImGui::AcceptDragDropPayload("PARENT_CHILD_HIERARCHY") })
			{
				BE::EntityID* child_entity = static_cast<BE::EntityID*>(payload->Data);

				BE::EntityID parent_entity = _x;

				BE::Transform& temp = dynamic_cast<BE::Transform&>(BE::ECS->GetComponent("Transform", parent_entity));
				temp.AttachChild(parent_entity, *child_entity);

				//strcpy_s(LoadSaveSceneList, Resources_Window::getdragdrop_path().c_str());
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("PARENT_CHILD_HIERARCHY", &_x, sizeof(_x));
			ImGui::Text(name.c_str());

			ImGui::EndDragDropSource();
		}

		if (open)
		{
			//For each children in children list
			for (BE::EntityID child : children)
			{
				const std::string& child_get_name = BE::ECS->GetComponent<BE::EntityInfo>(child).GetName();
				std::string child_name = ICON_FA_CUBE " " + child_get_name;

				//id = child;
				//isusing = true;

				ParentChild_Hierarchy(child_name, child);
			}

			ImGui::TreePop();
		}
	}
}

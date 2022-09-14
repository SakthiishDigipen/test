/******************************************************************************/
/*!
\file		EditorSystem.h
\par        Project: 
\author		Pham Thao Mien
\date   	October 7, 2022
\brief		Editor Manager, calling all ImGui windows

\copyright	Copyright (C) 2022 DigiPen Institute of Technology.
			Reproduction or disclosure of this file or its contents without the
			prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#ifndef EDITOR_SYSTEM_H
#define EDITOR_SYSTEM_H

#include <BananaEngine.h>
#include "../ImGuiWindow/ImGuiWindow.h"
#include "../Menu_Window/Menu_Window.h"
#include "../Console_Window/Console_Window.h"
#include "../Hierarchy_Window/Hierarchy_Window.h"
#include "../Inspector_Window/Inspector_Window.h"
#include "../Tool_Bar/Tool_Bar.h"
#include "../UndoSystem/UndoSystem.h"
#include "../EntityTag_Window/EntityTag_Window.h"
#include "../Resources_Window/Resources_Window.h"
#include "../Assets_Window/Assets_Window.h"

namespace BananaEditor
{
	class EditorSystem : public BE::Singleton<EditorSystem>
	{
		friend class BE::Singleton<EditorSystem>;

	private:
		Menu_Window* menu;
		Console_Window* console;
		Hierarchy_Window* hierarchy;
		Inspector_Window* inspector;
		Tool_Bar* tool_bar;
		EntityTag_Window* entityTag;
		Resources_Window* resources;
		Assets_Window* assets;

		std::vector<ImGuiWindow*> windows;

		static EditorSystem* pInstance;

		void ShowDockSpace(bool* p_open);
		void updateWindows();

		EditorSystem();
		~EditorSystem();

	public:
		static CommandManager* command;

		//Functions
		//void Init();
		void Init(GLFWwindow* window, ImGui_ImplVulkanH_Window* wd, VkInstance g_Instance, VkPhysicalDevice g_PhysicalDevice,
			VkDevice g_Device, uint32_t g_QueueFamily, VkQueue g_Queue, VkPipelineCache g_PipelineCache, VkDescriptorPool g_DescriptorPool,
			int g_MinImageCount, VkAllocationCallbacks* g_Allocator);
		void Update();
		//void Shutdown();
		void Shutdown(VkInstance g_Instance, VkDevice g_Device, ImGui_ImplVulkanH_Window g_MainWindowData, VkAllocationCallbacks* g_Allocator);
		//void HandleMessage(PE::IMessage* msg);
		void initWindows();
		void addWindow(ImGuiWindow* newWin);

		//Getters
		//inline static EditorSystem* GetInstance() { return pInstance; }
		//inline FPS_Window* Get_fps() { return fps; }

		//Creates an instance
		//static EditorSystem* CreateInstance();

		//Disabling copy constructor/assignment
		//EditorSystem& operator=(const EditorSystem&) = delete;
		//EditorSystem(const EditorSystem&) = delete;
	};
};

#endif
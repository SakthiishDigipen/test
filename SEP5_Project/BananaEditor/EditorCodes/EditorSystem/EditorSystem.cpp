/******************************************************************************/
/*!
\file		EditorSystem.cpp
\par        Project:
\author		Pham Thao Mien
\date   	October 7, 2022
\brief		Editor Manager, calling all ImGui windows

\copyright	Copyright (C) 2022 DigiPen Institute of Technology.
			Reproduction or disclosure of this file or its contents without the
			prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "EditorSystem.h"
#include"../UndoSystem/UndoSystem.h"

namespace BananaEditor
{
	EditorSystem* EditorSystem::pInstance =  nullptr;

	CommandManager* EditorSystem::command = nullptr;

	EditorSystem::EditorSystem() : menu{ nullptr }, console{ nullptr }, hierarchy{ nullptr }, 
		inspector{ nullptr }, tool_bar{ nullptr }, entityTag{ nullptr }, resources{ nullptr }, assets{ nullptr }
	{
		LogEditor("Created Editor System");
	}

	EditorSystem::~EditorSystem()
	{
		LogEditor("Destroyed Editor System");
	}

	void EditorSystem::Init(GLFWwindow* window, ImGui_ImplVulkanH_Window* wd, VkInstance g_Instance, VkPhysicalDevice g_PhysicalDevice,
		VkDevice g_Device, uint32_t g_QueueFamily, VkQueue g_Queue, VkPipelineCache g_PipelineCache, VkDescriptorPool g_DescriptorPool,
		int g_MinImageCount, VkAllocationCallbacks* g_Allocator)
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.IniFilename = "../BananaEditor/GameImGuiData.ini";
		//////////////////////////////////////////////////////////////////////////////////////
		//io.Fonts->AddFontDefault();
		io.Fonts->AddFontFromFileTTF("../BananaEditor/ImguiStuff/IconFont/ReadexPro-VariableFont_wght.ttf", 14);

		// merge in icons from Font Awesome
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		io.Fonts->AddFontFromFileTTF("../BananaEditor/ImguiStuff/IconFont/fa-solid-900.ttf", 16.0f, &icons_config, icons_ranges);
		//io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, 16.0f, &icons_config, icons_ranges);
		//////////////////////////////////////////////////////////////////////////////////////

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForVulkan(window, true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = g_Instance;
		init_info.PhysicalDevice = g_PhysicalDevice;
		init_info.Device = g_Device;
		init_info.QueueFamily = g_QueueFamily;
		init_info.Queue = g_Queue;
		init_info.PipelineCache = g_PipelineCache;
		init_info.DescriptorPool = g_DescriptorPool;
		init_info.Subpass = 0;
		init_info.MinImageCount = g_MinImageCount;
		init_info.ImageCount = wd->ImageCount;
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		init_info.Allocator = g_Allocator;
		//init_info.CheckVkResultFn = check_vk_result;
		ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);

		initWindows();

		//// Setup Dear ImGui context
		//IMGUI_CHECKVERSION();
		//ImGui::CreateContext();
		////ImPlot::CreateContext();
		//ImGuiIO& io = ImGui::GetIO(); (void)io;

		//io.IniFilename = "../SHEditor/GameImGuiData.ini";
		////////////////////////////////////////////////////////////////////////////////////////
		////io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("../SHEditor/ImguiStuff/IconFont/ReadexPro-VariableFont_wght.ttf", 14);

		//// merge in icons from Font Awesome
		//static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		//ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		//io.Fonts->AddFontFromFileTTF("../SHEditor/ImguiStuff/IconFont/fa-solid-900.ttf", 16.0f, &icons_config, icons_ranges);
		////io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, 16.0f, &icons_config, icons_ranges);
		////////////////////////////////////////////////////////////////////////////////////////

		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		////io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		////io.ConfigViewportsNoAutoMerge = true;
		////io.ConfigViewportsNoTaskBarIcon = true;

		//// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		////ImGui::StyleColorsClassic();

		//// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		//ImGuiStyle& style = ImGui::GetStyle();
		//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		//{
		//	style.WindowRounding = 0.0f;
		//	style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		//}

		//initWindows();

		//// Setup Platform/Renderer backends
		//ImGui_ImplGlfw_InitForVulkan(window, true);
		////ImGui_ImplOpenGL3_Init("#version 330 core");
	}

	void EditorSystem::Update()
	{
		bool show = true;

		// Start the Dear ImGui frame
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ShowDockSpace(&show);
		updateWindows();

		// Rendering
		ImGui::Render();

		//bool show = true;

		//// Start the Dear ImGui frame
		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		//ImGui::NewFrame();

		//ImGuizmo::BeginFrame();

		//ImGuiIO& io = ImGui::GetIO();

		//ShowDockSpace(&show);
		//updateWindows();

		//// Rendering
		//ImGui::Render();

		//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		//{
		//	GLFWwindow* backup_current_context = glfwGetCurrentContext();
		//	ImGui::UpdatePlatformWindows();
		//	ImGui::RenderPlatformWindowsDefault();
		//	glfwMakeContextCurrent(backup_current_context);
		//}

		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		////glfwSwapBuffers(SH::GLFW->GetWindowHandler());
		////glfwPollEvents();
		////ImGui::UpdatePlatformWindows();
		////ImGui::RenderPlatformWindowsDefault();

		////SH::ToggleIMGUIPausing togglePause(true);
		////SH::Engine->DispatchMessage(&togglePause);
	}

	void EditorSystem::Shutdown(VkInstance g_Instance, VkDevice g_Device, ImGui_ImplVulkanH_Window g_MainWindowData, VkAllocationCallbacks* g_Allocator)
	{
		for (auto i = 0; i < windows.size(); i++)
		{
			windows[i]->Shutdown();
		}

		// Cleanup
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();

		ImGui::DestroyContext();

		ImGui_ImplVulkanH_DestroyWindow(g_Instance, g_Device, &g_MainWindowData, g_Allocator);

		delete command;
		delete pInstance;
	}

	/*void EditorSystem::HandleMessage(SH::IMessage* msg)
	{
		(void)msg;
	}*/

	//EditorSystem* EditorSystem::CreateInstance()
	//{
	//	//If game engine already exists, then don't create a new one
	//	if (pInstance)
	//		return pInstance;

	//	pInstance = new EditorSystem();

	//	command = new CommandManager();

	//	return pInstance;
	//}

	void EditorSystem::ShowDockSpace(bool* p_open)
	{
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("###DockSpace", p_open, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		else
		{
			//ShowDockingDisabledMessage();
		}

		ImGui::End();
	}

	void EditorSystem::initWindows()
	{
		menu = Menu_Window::Get()->init();
		addWindow(Menu_Window::Get());

		console = Console_Window::Get()->init();
		addWindow(Console_Window::Get());

		hierarchy = Hierarchy_Window::Get()->init();
		addWindow(Hierarchy_Window::Get());

		inspector = Inspector_Window::Get()->init();
		addWindow(Inspector_Window::Get());

		tool_bar = Tool_Bar::Get()->init();
		addWindow(Tool_Bar::Get());

		entityTag = EntityTag_Window::Get()->init();
		addWindow(EntityTag_Window::Get());

		resources = Resources_Window::Get()->init();
		addWindow(Resources_Window::Get());

		assets = Assets_Window::Get()->init();
		addWindow(Assets_Window::Get());
	}

	void EditorSystem::updateWindows()
	{
		for (auto i = 0; i < windows.size(); i++)
		{
			windows[i]->update();
		}
	}

	void EditorSystem::addWindow(ImGuiWindow* newWin)
	{
		if (newWin)
		{
			windows.push_back(newWin);
		}
	}
}

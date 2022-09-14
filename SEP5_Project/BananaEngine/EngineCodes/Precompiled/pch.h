#pragma once
#ifndef PCH_H
#define PCH_H

//External Libraries
#if defined( _WIN32 )
#define VK_USE_PLATFORM_WIN32_KHR 1
#include <Windows.h>
#endif

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
//#include <glm/ext/matrix_transform.inl>
//#include <glm/ext/matrix_clip_space.inl>
//#include <glm/detail/func_trigonometric.inl>

//vulkan
#include "vk_mem_alloc.h"

#include "VkBootstrap.h"

//Fmod
#include <fmod.hpp>

//Imgui
#include "../../../BananaEditor/ImguiStuff/imgui-master/imgui.h"
#include "../../../BananaEditor/ImguiStuff/imgui-master/imgui_impl_glfw.h"
#include "../../../BananaEditor/ImguiStuff/imgui-master/imgui_impl_vulkan.h"

#include "../../../BananaEditor/ImguiStuff/IconFont/IconsFontAwesome5.h"

#include "../../../BananaEditor/ImguiStuff/ImGuizmo-master/ImGuizmo.h"

//Input output
#include <iostream>
#include <iomanip>
#include <cstdarg>
#include <fstream>
#include <filesystem>

//STL stuff
#include <random>
#include <utility>
#include <cmath>
#include <algorithm>
#include <memory>
#include <type_traits>
#include <exception>
#include <optional>
#include <functional>

//STL Data structure
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <bitset>
#include <array>
#include <deque>
#include <stack>

//RTTR
#include <rttr/type>
#include <rttr/registration>
#include <rttr/registration_friend>

//JSON stuff
#include <document.h>
#include <writer.h>
#include <prettywriter.h>
#include <stringbuffer.h>
#include <ostreamwrapper.h>


//Class forward declaration

namespace BE
{
	//Maths
	template <typename T> class Vec3;
	template <typename T> class Mat3;
	template <typename T> class Quaternion;

	//Misc
	class BEID;

	//Engine systems
	class GameEngine;
	class ISystem;

	//Engine Events
	class IEvent;
	class EventDispatcher;
	class QuitEvent;
	class RestartEvent;
	class SceneUnloadedEvent;
	class SceneLoadedEvent;
	class SceneRestartEvent;
	class AssetsLoadedEvent;

	//ECS
	class ECSEntityMgr;
	class ICompArray;
	template <typename ComponentType> class ECSCompArray;
	class ECSComponentMgr;
	class IECSTracker;
	class ECSTrackerMgr;
	class ECSCoordinator;

	//ECS Components
	class IECSComponent;
	class Transform;
	class EntityInfo;
	class AudioSource;
	class AudioListener;

	//Asset Manager
	class AssetManager;

	//Scene Manager
	class Scene;
	class SceneManager;
	
	//Asset Files
	class AudioClip;

	//Graphics
	class GLFWSystem;
	class VulkanSystem;
}

#endif
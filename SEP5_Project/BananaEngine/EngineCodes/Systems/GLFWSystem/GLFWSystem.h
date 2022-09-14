#pragma once
#ifndef	GLFWSYSTEM_H
#define GLFWSYSTEM_H
#include <Utilities/Singleton/Singleton.h>
#include "../ISystem.h"

namespace BE 
{
	class GLFWSystem : public Singleton<GLFWSystem>, public ISystem
	{
	private:
		static GLFWSystem* m_Instance;
		//Window information
		GLFWwindow* m_wHandler;				//The window handler of the application
		std::string m_windowName;			//The title of the window
		int m_windowWidth;					//Width of the window
		int m_windowHeight;					//Height of the window
		bool m_isFullScreen;				//Whether is the game in full screen
		int m_nonFullScreenWindowWidth;
		int m_nonFullScreenWindowHeight;

		//To disable input when user input in textbox
		bool m_disableInput;				//Disable input
		bool m_isInterrupted;

		
		//Constructor/Destructor
		GLFWSystem();
		~GLFWSystem();

		void SetIsFullScreen(bool newVal);

		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	

	public:

		// regular instance and device layers and extensions
		std::vector<const char*> instance_layers;
		//	std::vector<const char*> device_layers;					// depricated
		std::vector<const char*> instance_extensions;
		std::vector<const char*> device_extensions;
		VkInstance instance = VK_NULL_HANDLE;
		VkSurfaceKHR surface = VK_NULL_HANDLE;
		VkDevice device = VK_NULL_HANDLE;
		GLFWwindow* window;
		//Serialise
		int width = 800;
		int height = 600;
		//getters
		inline GLFWwindow* GetWindowHandler() const { return m_wHandler; }
		inline int GetWindowWidth() const { return m_windowWidth; }
		inline int GetWindowHeight() const { return m_windowHeight; }
		inline bool GetIsFullScreen() const { return m_isFullScreen; }
		inline const std::string& GetWindowTitle() const { return m_windowName; }
		inline const bool GetDisabledInput() const { return m_disableInput; }
		inline int GetNonFullScreenWindowWidth() const { return m_nonFullScreenWindowWidth; }
		inline int GetNonFullScreenWindowHeight() const { return m_nonFullScreenWindowHeight; }

		//Functions to overide
		ISYSTEM_FUNCTIONS;

		friend class Singleton<GLFWSystem>;
	};

	//GLFW callback for inputs
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void mousebutton_callback(GLFWwindow* window, int button, int action, int mods);
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	//void iconification_callback(GLFWwindow* window, int isIconified);
	void cursor_enter_callback(GLFWwindow* window, int entered);
	void window_focus_callback(GLFWwindow* window, int isFocused);

	/*
	GLFW callback to ensure the new window dimension matches the viewport in terms of pixel
	*/
	void framebuffer_size_callback(GLFWwindow* pWindow, GLuint width, GLuint height);
	void window_size_callback(GLFWwindow* window, int width, int height);


#define GLFW GLFWSystem::GetInstance()
}

#endif
#pragma once
#ifndef Window_h
#define Window_h

#include <Precompiled/pch.h>

#include <string>
#include <vector>
#include <assert.h>
#include <codecvt>
#include <array>
//#include <vulkan/vulkan.h>
#include <algorithm>



class Window
{
	public:
		Window( uint32_t size_x, uint32_t size_y, std::string name);
		~Window();

		void Close();
		bool Update();
		void Resize(UINT x, UINT y);

		void	beginRender();
		void	endRender(std::vector<VkSemaphore> wait_semaphores);

		VkRenderPass	getVulkanRenderPass();
		VkFramebuffer		getVulkanActiveFramebuffer();
		VkExtent2D	getVulkanSurfaceSize();
		bool& getResizingStatus();
		bool& getFirstRun();
		VkFormat getSwapChainImageFormat();
	private:
		void	initOSWindow();
		void	destroyOSWindow();
		void	updateOSWindow();
		void	initOSSurface();

		void	initSurface();
		void	destroySurface();

		void	initSwapChain();
		void	destroySwapChain();

		void	initSwapChainImages();
		void	destroySwapChainImages();

		void	initDepthStencilImage();
		void	destroyDepthStencilImage();

		void	initRenderPass();
		void	destroyRenderPass();

		void	initFrameBuffers();
		void	destroyFrameBuffers();

		void	initSynchronizations();
		void	destroySynchronizations();

		uint32_t	surfaceWidth{ 1024 };
		uint32_t	surfaceHeight{ 768 };
		LPCWSTR	windowName;

		bool firstRun{ true };
		bool	windowEnabled{ true };
		bool windowResizing{ false };

		HINSTANCE	win32Instance{ NULL };
		HWND	win32Window{ NULL };
		LPCWSTR	win32ClassName;
		static uint64_t	win32ClassIdCounter;

		uint32_t	swapchainImageCount{ 2 };
		VkSurfaceKHR	surfaceKHR{ VK_NULL_HANDLE };
		VkSwapchainKHR	swapchainKHR{ VK_NULL_HANDLE };

		VkSurfaceFormatKHR		surfaceFormat{};
		VkSurfaceCapabilitiesKHR	surfaceCapabilities{};

		std::vector<VkImage>	swapchainImageVector;
		std::vector<VkImageView>	swapchainImageViewVector;
		uint32_t	currentSwapChainImageID{ UINT32_MAX };
		VkFence	swapchainImageAvailable{ VK_NULL_HANDLE };
		VkFormat swapchainImageFormat{};


		VkImage	depthStencilImage{ VK_NULL_HANDLE };
		VkDeviceMemory	depthStencilImageMemory{ VK_NULL_HANDLE };
		VkImageView	depthStencilImageView{ VK_NULL_HANDLE };
		VkFormat		depthStencilFormat{ VK_FORMAT_UNDEFINED };
		bool	stencilStatus{ false };

		VkRenderPass	renderPass{ VK_NULL_HANDLE };
		std::vector<VkFramebuffer>	frameBufferVector;

		//helper
		uint32_t  FindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties* properties, const VkMemoryRequirements* requirements, const VkMemoryPropertyFlags propertyFlags);

		VkResult result;

	public:
		static bool dragging;
		static bool scrollUp, scrollDown;
};
#endif
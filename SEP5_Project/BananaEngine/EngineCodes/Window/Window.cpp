#include "pch.h"
#include "Window.h"

uint64_t	Window::win32ClassIdCounter = 0;
bool Window::dragging{};
bool Window::scrollUp{}, Window::scrollDown{};
inline std::wstring StringToWString(const std::string s)
{
	std::wstring temp(s.length(), L' ');
	std::copy(s.begin(), s.end(), temp.begin());
	return temp;
}


inline std::string WStringToString(const std::wstring s)
{
	std::string temp(s.length(), ' ');
	std::transform(s.begin(), s.end(), std::back_inserter(temp), [](wchar_t c) {
		return (char)c;
		});;

	return temp;
}
//Detects if user is dragging mouse while holding right click or if user is scrolling
LRESULT CALLBACK LowLevelMouseProc(_In_ int    nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	if (nCode >= 0)
	{
		MSLLHOOKSTRUCT* info = (MSLLHOOKSTRUCT*)(lParam);

		if (wParam == WM_RBUTTONDOWN)
		{
			Window::dragging = true;
		}
		else if (wParam == WM_RBUTTONUP)
		{
			Window::dragging = false;
		}

		if (wParam == WM_MOUSEWHEEL)
		{
			short zDelta = HIWORD(info->mouseData);
			zDelta *= -1;

			if (zDelta > 0)
			{
				Window::scrollUp = true;
			}
			else if (zDelta < 0)
			{
				Window::scrollDown = true;
			}
		}
		else
		{
			Window::scrollUp = false;
			Window::scrollDown = false;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

//Detecting if user presses a button
LRESULT CALLBACK LowLevelKBProc(_In_ int    nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	if (nCode >= 0)
	{
		KBDLLHOOKSTRUCT* info = (KBDLLHOOKSTRUCT*)(lParam);

		if (wParam == WM_KEYDOWN)
		{
			//Example
			if (info->vkCode & VK_SPACE)
			{
			}
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

//Get vkdevice from renderer to here
Window::Window( uint32_t size_x, uint32_t size_y, std::string name)
{
	surfaceWidth = size_x;
	surfaceHeight = size_y;

	windowName = StringToWString(name).c_str();

	//initOSWindow();
	//initSurface();
	//initSwapChain();
	//initSwapChainImages();
	//initDepthStencilImage();
	//initRenderPass();
	//initFrameBuffers();
	//initSynchronizations();
}

Window::~Window()
{

}

void Window::Close()
{
	windowEnabled = false;
	
	//Should wait for queue to be cleared before closing the window
	//result = vkQueueWaitIdle(ptrToRenderer->getVulkanQueue());
	//assert(result == VK_SUCCESS && "Window close wait for queue idle failed\n");

	//destroySynchronizations();
	//destroyFrameBuffers();
	//destroyRenderPass();
	//destroyDepthStencilImage();
	//destroySwapChainImages();
	//destroySwapChain();
	//destroySurface();
	//destroyOSWindow();
}

bool Window::Update()
{
	updateOSWindow();
	return windowEnabled;
}

void Window::Resize(UINT x, UINT y)
{
	windowResizing = true;
	while (x == 0 && y == 0)
	{

	}
	if (x != UINT32_MAX && y != UINT32_MAX)
	{
		surfaceWidth = x;
		surfaceHeight = y;
	}

	//vkDeviceWaitIdle(ptrToRenderer->getVulkanDevice());

	//vkDestroyPipelineLayout(ptrToRenderer->getVulkanDevice(), ptrToRenderer->getPipeLineLayout(), nullptr);
	//vkDestroyPipeline(ptrToRenderer->getVulkanDevice(), ptrToRenderer->getPipeLine(), nullptr);

	//for (size_t i = 0; i < frameBufferVector.size(); i++) {
	//	vkDestroyFramebuffer(ptrToRenderer->getVulkanDevice(), frameBufferVector[i], nullptr);
	//}

	//vkDestroyImageView(ptrToRenderer->getVulkanDevice(), depthStencilImageView, nullptr);
	//vkFreeMemory(ptrToRenderer->getVulkanDevice(), depthStencilImageMemory, nullptr);
	//vkDestroyImage(ptrToRenderer->getVulkanDevice(), depthStencilImage, nullptr);

	//for (size_t i = 0; i < swapchainImageViewVector.size(); i++) {
	//	vkDestroyImageView(ptrToRenderer->getVulkanDevice(), swapchainImageViewVector[i], nullptr);
	//}
	//vkDestroySwapchainKHR(ptrToRenderer->getVulkanDevice(), swapchainKHR, nullptr);

	//initSwapChain();
	//initSwapChainImages();
	//initDepthStencilImage();
	//initFrameBuffers();
	//ptrToRenderer->createPipeline();
}

void Window::beginRender()
{
	//result = vkAcquireNextImageKHR(ptrToRenderer->getVulkanDevice(), swapchainKHR, UINT64_MAX, VK_NULL_HANDLE, swapchainImageAvailable, &currentSwapChainImageID);
	//ptrToRenderer->updateUniformBuffer(0);

	//if (result == VK_ERROR_OUT_OF_DATE_KHR) {
	//	Resize(UINT32_MAX, UINT32_MAX);
	//	return;
	//}
	//else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
	//	assert("Failed to acquire swap chain images in beginRender()\n");
	//}

	//result = vkWaitForFences(ptrToRenderer->getVulkanDevice(), 1, &swapchainImageAvailable, VK_TRUE, UINT64_MAX);
	//if (result == VK_ERROR_OUT_OF_DATE_KHR) {
	//	Resize(UINT32_MAX, UINT32_MAX);
	//	return;
	//}
	//else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
	//	assert("Failed to wait for fences in beginRender()\n");
	//}

	//result = vkResetFences(ptrToRenderer->getVulkanDevice(), 1, &swapchainImageAvailable);
	//assert(result == VK_SUCCESS && "Failed to reset fences in beginRender()\n");

	//result = vkQueueWaitIdle(ptrToRenderer->getVulkanQueue());
	//if (result == VK_ERROR_OUT_OF_DATE_KHR) {
	//	Resize(UINT32_MAX, UINT32_MAX);
	//	return;
	//}
	//else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
	//	assert("Failed to queue wait idle in beginRender()\n");
	//}

	//// Record command buffer
	//VkCommandBufferBeginInfo command_buffer_begin_info{};
	//command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	//vkBeginCommandBuffer(ptrToRenderer->getCommandBuffer(), &command_buffer_begin_info);

	//VkRect2D render_area{};
	//render_area.offset.x = 0;
	//render_area.offset.y = 0;
	//render_area.extent = ptrToRenderer->getWindowPtr()->getVulkanSurfaceSize();

	//std::array<VkClearValue, 2> clear_values{};
	//clear_values[0].depthStencil.depth = 1.0f;
	//clear_values[0].depthStencil.stencil = 0;
	//clear_values[1].color.float32[0] = 0.0f;
	//clear_values[1].color.float32[1] = 0.0f;
	//clear_values[1].color.float32[2] = 0.0f;
	//clear_values[1].color.float32[3] = 0.0f;

	//VkRenderPassBeginInfo render_pass_begin_info{};
	//render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	//render_pass_begin_info.renderPass = ptrToRenderer->getWindowPtr()->getVulkanRenderPass();
	//render_pass_begin_info.framebuffer = ptrToRenderer->getWindowPtr()->getVulkanActiveFramebuffer();
	//render_pass_begin_info.renderArea = render_area;
	//render_pass_begin_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
	//render_pass_begin_info.pClearValues = clear_values.data();

	//vkCmdBeginRenderPass(ptrToRenderer->getCommandBuffer(), &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
	//vkCmdBindPipeline(ptrToRenderer->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, ptrToRenderer->getPipeLine());
}

void Window::endRender(std::vector<VkSemaphore> wait_semaphores)
{
	//vkCmdEndRenderPass(ptrToRenderer->getCommandBuffer());

	//vkEndCommandBuffer(ptrToRenderer->getCommandBuffer());

	//// Submit command buffer
	//VkSubmitInfo submit_info{};
	//submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	//submit_info.waitSemaphoreCount = 0;
	//submit_info.pWaitSemaphores = nullptr;
	//submit_info.pWaitDstStageMask = nullptr;
	//submit_info.commandBufferCount = 1;
	//submit_info.pCommandBuffers = &ptrToRenderer->getCommandBuffer();
	//submit_info.signalSemaphoreCount = 1;
	//submit_info.pSignalSemaphores = &ptrToRenderer->getRenderStatusSemaphore();

	//vkQueueSubmit(ptrToRenderer->getVulkanQueue(), 1, &submit_info, VK_NULL_HANDLE);

	//result = VkResult::VK_RESULT_MAX_ENUM;

	//VkPresentInfoKHR info{};
	//info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	//info.waitSemaphoreCount = static_cast<uint32_t>(wait_semaphores.size());
	//info.pWaitSemaphores = wait_semaphores.data();
	//info.swapchainCount = 1;
	//info.pSwapchains = &swapchainKHR;
	//info.pImageIndices = &currentSwapChainImageID;
	//info.pResults = &result;

	//result = vkQueuePresentKHR(ptrToRenderer->getVulkanQueue(), &info);
	//if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
	//	Resize(UINT32_MAX, UINT32_MAX);
	//}
	//else if (result != VK_SUCCESS) {
	//	assert("Failed to present swap chain image!\n");
	//}
	//assert(result == VK_SUCCESS && "Failed queue present khr in endRender()\n");
}

VkRenderPass Window::getVulkanRenderPass()
{
	return renderPass;
}

VkFramebuffer Window::getVulkanActiveFramebuffer()
{
	return frameBufferVector[currentSwapChainImageID];
}

VkExtent2D Window::getVulkanSurfaceSize()
{
	return { surfaceWidth, surfaceHeight };
}

bool& Window::getResizingStatus()
{
	return windowResizing;
}

bool& Window::getFirstRun()
{
	return firstRun;
}

VkFormat Window::getSwapChainImageFormat()
{
	return swapchainImageFormat;
}

// Microsoft Windows specific versions of window functions
LRESULT CALLBACK WindowsEventHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window* window = reinterpret_cast<Window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));

	switch (uMsg) {
	case WM_CLOSE:
		window->Close();
		return 0;
	case WM_SIZE:
		if (window->getFirstRun() == false)
		{
			window->Resize(LOWORD(lParam), HIWORD(lParam));
		}
		window->getFirstRun() = false;
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Window::initOSWindow()
{
	WNDCLASSEX windowClass{};
	assert(surfaceWidth > 0 && "surface width no more than 0\n");
	assert(surfaceHeight > 0 && "surface height not more than 0\n");

	win32Instance = GetModuleHandle(nullptr);
	win32ClassName = windowName;
	++win32ClassIdCounter;

	// Initialize the window class structure:
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowsEventHandler;
	windowClass.hInstance = win32Instance; // hInstance
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = WStringToString(win32ClassName).c_str();
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	// Register window class:
	if (!RegisterClassEx(&windowClass)) {
		// It didn't work, so try to give a useful error:
		assert(0 && "Cannot create a window in which to draw!\n");
		fflush(stdout);
		std::exit(-1);
	}

	DWORD style = WS_SIZEBOX | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	DWORD exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	// Create window with the registered class:
	RECT wr = { 0, 0, LONG(surfaceWidth), LONG(surfaceHeight) };
	AdjustWindowRectEx(&wr, style, FALSE, exStyle);
	win32Window = CreateWindowEx(0,
		WStringToString(win32ClassName).c_str(),		// class name
		WStringToString(windowName).c_str(),			// app name
		style,							// window style
		CW_USEDEFAULT, CW_USEDEFAULT,	// x/y coords
		wr.right - wr.left,				// width
		wr.bottom - wr.top,				// height
		NULL,							// handle to parent
		NULL,							// handle to menu
		win32Instance,				// hInstance
		NULL);							// no extra parameters

	if (!win32Window) {
		// It didn't work, so try to give a useful error:
		assert(1 && "Cannot create a window in which to draw!\n");
		fflush(stdout);
		std::exit(-1);
	}
	SetWindowLongPtr(win32Window, GWLP_USERDATA, (LONG_PTR)this);
	SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
	SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKBProc, NULL, 0);

	ShowWindow(win32Window, SW_SHOW);
	SetForegroundWindow(win32Window);
	SetFocus(win32Window);
}

void Window::destroyOSWindow()
{
	DestroyWindow(win32Window);
	UnregisterClass(WStringToString(win32ClassName).c_str(), win32Instance);
}

void Window::updateOSWindow()
{
	MSG msg;
	if (PeekMessage(&msg, win32Window, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	windowResizing = false;
}

void Window::initOSSurface()
{
	//VkWin32SurfaceCreateInfoKHR createInfo{};
	//createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	//createInfo.hinstance = win32Instance;
	//createInfo.hwnd = win32Window;
	//result = vkCreateWin32SurfaceKHR(ptrToRenderer->getVulkanInstance(), &createInfo, nullptr, &surfaceKHR);
	//assert(result == VK_SUCCESS && "Failed to create win32 surface khr in initOSSurface()\n");
}

void Window::initSurface()
{
	initOSSurface();

	//auto gpu = ptrToRenderer->getVulkanPhysicalDevice();

	//VkBool32 wsiSupport = false;
	//result = vkGetPhysicalDeviceSurfaceSupportKHR(gpu, ptrToRenderer->getVulkanGraphicsQueueFamilyIndex(), surfaceKHR, &wsiSupport);
	//assert(result == VK_SUCCESS && "Failed to get physical device surface support khr in initSurface()\n");

	//if (!wsiSupport) {
	//	assert(0 && "WSI not supported");
	//	std::exit(-1);
	//}

	//result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surfaceKHR, &surfaceCapabilities);
	//assert(result == VK_SUCCESS && "Failed to create win32 surface capabilities in initOSSurface()\n");

	//if (surfaceCapabilities.currentExtent.width < UINT32_MAX)
	//{
	//	surfaceWidth = surfaceCapabilities.currentExtent.width;
	//	surfaceHeight = surfaceCapabilities.currentExtent.height;
	//}

	//{
	//	uint32_t formatCount = 0;
	//	result = vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surfaceKHR, &formatCount, nullptr);
	//	assert(result == VK_SUCCESS && "Failed to get physical device surface formats khr in initOSSurface()\n");

	//	if (formatCount == 0) {
	//		assert(0 && "Surface formats missing.");
	//		std::exit(-1);
	//	}
	//	std::vector<VkSurfaceFormatKHR> formats(formatCount);
	//	result = vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surfaceKHR, &formatCount, formats.data());
	//	assert(result == VK_SUCCESS && "Failed to get physical device surface formats in initOSSurface()\n");

	//	if (formats[0].format == VK_FORMAT_UNDEFINED)
	//	{
	//		//code shouldnt enter here
	//		surfaceFormat.format = VK_FORMAT_R8G8B8A8_SRGB;
	//		surfaceFormat.colorSpace = VK_COLOR_SPACE_PASS_THROUGH_EXT;
	//		//VK_COLOR_SPACE_SRGB_NONLINEAR_KHR | VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT  | VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT 
	//		//VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT  | VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT  | VK_COLOR_SPACE_PASS_THROUGH_EXT 
	//	}
	//	else
	//	{
	//		surfaceFormat = formats[0];
	//	}

	//	swapchainImageFormat = surfaceFormat.format;
	//}
}

void Window::destroySurface()
{
	//vkDestroySurfaceKHR(ptrToRenderer->getVulkanInstance(), surfaceKHR, nullptr);
}

void Window::initSwapChain()
{
	// The code above will work just fine in our tutorials and likely on every possible implementation of vulkan as well
	// so this change isn't that important. Just to be absolutely sure we don't go over or below the given limits we should check this a
	// little bit different though. maxImageCount can actually be zero in which case the amount of swapchain images do not have an
	// upper limit other than available memory. It's also possible that the swapchain image amount is locked to a certain
	// value on certain systems. The code below takes into consideration both of these possibilities.
	if (swapchainImageCount < surfaceCapabilities.minImageCount + 1)
		swapchainImageCount = surfaceCapabilities.minImageCount + 1;

	if (surfaceCapabilities.maxImageCount > 0)
	{
		if (swapchainImageCount > surfaceCapabilities.maxImageCount)
			swapchainImageCount = surfaceCapabilities.maxImageCount;
	}

	//VkPresentModeKHR present_mode{ VK_PRESENT_MODE_FIFO_KHR };
	//uint32_t present_mode_count = 0;

	//result = vkGetPhysicalDeviceSurfacePresentModesKHR(ptrToRenderer->getVulkanPhysicalDevice(), surfaceKHR, &present_mode_count, nullptr);
	//assert(result == VK_SUCCESS && "Failed to get physical device surface present modes khr count in initSwapChain()\n");

	//std::vector<VkPresentModeKHR> present_mode_list(present_mode_count);

	//result = vkGetPhysicalDeviceSurfacePresentModesKHR(ptrToRenderer->getVulkanPhysicalDevice(), surfaceKHR, &present_mode_count, present_mode_list.data());
	//assert(result == VK_SUCCESS && "Failed to get physical device surface present modes khr details in initSwapChain()\n");

	//for (auto m : present_mode_list) {
	//	if (m == VK_PRESENT_MODE_MAILBOX_KHR) present_mode = m;
	//}

	//VkSwapchainCreateInfoKHR swapchain_create_info{};
	//swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	//swapchain_create_info.surface = surfaceKHR;
	//swapchain_create_info.minImageCount = swapchainImageCount;
	//swapchain_create_info.imageFormat = surfaceFormat.format;
	//swapchain_create_info.imageColorSpace = surfaceFormat.colorSpace;
	//swapchain_create_info.imageExtent.width = surfaceWidth;
	//swapchain_create_info.imageExtent.height = surfaceHeight;
	//swapchain_create_info.imageArrayLayers = 1;
	//swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	//swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	//swapchain_create_info.pQueueFamilyIndices = nullptr;
	//swapchain_create_info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	//swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	//swapchain_create_info.presentMode = present_mode;
	//swapchain_create_info.clipped = VK_TRUE;
	//swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;

	////assert
	//result = vkCreateSwapchainKHR(ptrToRenderer->getVulkanDevice(), &swapchain_create_info, nullptr, &swapchainKHR);
	//assert(result == VK_SUCCESS && "Failed to create swapchainkhr in initSwapChain()\n");

	//result = vkGetSwapchainImagesKHR(ptrToRenderer->getVulkanDevice(), swapchainKHR, &swapchainImageCount, nullptr);
	//assert(result == VK_SUCCESS && "Failed to get swapchain images khr in initSwapChain()\n");
}

void Window::destroySwapChain()
{
	//vkDestroySwapchainKHR(ptrToRenderer->getVulkanDevice(), swapchainKHR, nullptr);
}

void Window::initSwapChainImages()
{
	swapchainImageVector.resize(swapchainImageCount);
	swapchainImageViewVector.resize(swapchainImageCount);

	/*result = vkGetSwapchainImagesKHR(ptrToRenderer->getVulkanDevice(), swapchainKHR, &swapchainImageCount, swapchainImageVector.data());
	assert(result == VK_SUCCESS && "Failed to get swapchain images khr in initSwapChainImages\n");

	for (uint32_t i = 0; i < swapchainImageCount; ++i)
	{
		VkImageViewCreateInfo image_view_create_info{};
		image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		image_view_create_info.image = swapchainImageVector[i];
		image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_create_info.format = surfaceFormat.format;
		image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_create_info.subresourceRange.levelCount = 1;
		image_view_create_info.subresourceRange.layerCount = 1;

		result = vkCreateImageView(ptrToRenderer->getVulkanDevice(), &image_view_create_info, nullptr, &swapchainImageViewVector[i]);
		assert(result == VK_SUCCESS && "Failed to create image view in initSwapChainImages()\n");
	}*/
}

void Window::destroySwapChainImages()
{
	//for (auto view : swapchainImageViewVector)
	//{
		//vkDestroyImageView(ptrToRenderer->getVulkanDevice(), view, nullptr);
	//}
}

void Window::initDepthStencilImage()
{
	/*{
		std::vector<VkFormat> try_formats
		{
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM_S8_UINT,
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D16_UNORM
		};

		for (auto f : try_formats) {
			VkFormatProperties format_properties{};
			vkGetPhysicalDeviceFormatProperties(ptrToRenderer->getVulkanPhysicalDevice(), f, &format_properties);
			if (format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
				depthStencilFormat = f;
				break;
			}
		}

		if (depthStencilFormat == VK_FORMAT_UNDEFINED) {
			assert(0 && "Depth stencil format not selected.\n");
			std::exit(-1);
		}

		if ((depthStencilFormat == VK_FORMAT_D32_SFLOAT_S8_UINT) ||
			(depthStencilFormat == VK_FORMAT_D24_UNORM_S8_UINT) ||
			(depthStencilFormat == VK_FORMAT_D16_UNORM_S8_UINT) ||
			(depthStencilFormat == VK_FORMAT_S8_UINT)) {
			stencilStatus = true;
		}
	}

	VkImageCreateInfo image_create_info{};
	image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_create_info.imageType = VK_IMAGE_TYPE_2D;
	image_create_info.format = depthStencilFormat;
	image_create_info.extent.width = surfaceWidth;
	image_create_info.extent.height = surfaceHeight;
	image_create_info.extent.depth = 1;
	image_create_info.mipLevels = 1;
	image_create_info.arrayLayers = 1;
	image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
	image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
	image_create_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	image_create_info.queueFamilyIndexCount = VK_QUEUE_FAMILY_IGNORED;
	image_create_info.pQueueFamilyIndices = nullptr;
	image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	result = vkCreateImage(ptrToRenderer->getVulkanDevice(), &image_create_info, nullptr, &depthStencilImage);
	assert(result == VK_SUCCESS && "Failed to create image in initDepthStencilImage()\n");

	VkMemoryRequirements image_memory_requirements{};
	vkGetImageMemoryRequirements(ptrToRenderer->getVulkanDevice(), depthStencilImage, &image_memory_requirements);

	uint32_t memory_index = FindMemoryTypeIndex(&ptrToRenderer->getVulkanPhysicalDeviceMemoryProperties(), &image_memory_requirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	VkMemoryAllocateInfo memory_allocate_info{};
	memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memory_allocate_info.allocationSize = image_memory_requirements.size;
	memory_allocate_info.memoryTypeIndex = memory_index;

	result = vkAllocateMemory(ptrToRenderer->getVulkanDevice(), &memory_allocate_info, nullptr, &depthStencilImageMemory);
	assert(result == VK_SUCCESS && "Failed to allocate memory in initDepthStencilImage()\n");

	result = vkBindImageMemory(ptrToRenderer->getVulkanDevice(), depthStencilImage, depthStencilImageMemory, 0);
	assert(result == VK_SUCCESS && "Failed to bind image memory in initDepthStencilImage()\n");

	VkImageViewCreateInfo image_view_create_info{};
	image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	image_view_create_info.image = depthStencilImage;
	image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	image_view_create_info.format = depthStencilFormat;
	image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | (stencilStatus ? VK_IMAGE_ASPECT_STENCIL_BIT : 0);
	image_view_create_info.subresourceRange.levelCount = 1;
	image_view_create_info.subresourceRange.layerCount = 1;

	result = vkCreateImageView(ptrToRenderer->getVulkanDevice(), &image_view_create_info, nullptr, &depthStencilImageView);
	assert(result == VK_SUCCESS && "Failed to create image view in initDepthStencilImage()\n");*/
}

void Window::destroyDepthStencilImage()
{
	//vkDestroyImageView(ptrToRenderer->getVulkanDevice(), depthStencilImageView, nullptr);
	//vkFreeMemory(ptrToRenderer->getVulkanDevice(), depthStencilImageMemory, nullptr);
	//vkDestroyImage(ptrToRenderer->getVulkanDevice(), depthStencilImage, nullptr);
}

void Window::initRenderPass()
{
	//std::array<VkAttachmentDescription, 2> attachments{};
	//attachments[0].format = depthStencilFormat;
	//attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
	//attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	//attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	//attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	//attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
	//attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	//attachments[0].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//attachments[1].format = surfaceFormat.format;
	//attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
	//attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	//attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	//attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	//attachments[1].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


	//VkAttachmentReference sub_pass_0_depth_stencil_attachment{};
	//sub_pass_0_depth_stencil_attachment.attachment = 0;
	//sub_pass_0_depth_stencil_attachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//std::array<VkAttachmentReference, 1> sub_pass_0_color_attachments{};
	//sub_pass_0_color_attachments[0].attachment = 1;
	//sub_pass_0_color_attachments[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	//std::array<VkSubpassDescription, 1> sub_passes{};
	//sub_passes[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	//sub_passes[0].colorAttachmentCount = static_cast<uint32_t>(sub_pass_0_color_attachments.size());
	//sub_passes[0].pColorAttachments = sub_pass_0_color_attachments.data();		// layout(location=0) out vec4 FinalColor;
	//sub_passes[0].pDepthStencilAttachment = &sub_pass_0_depth_stencil_attachment;


	//VkRenderPassCreateInfo render_pass_create_info{};
	//render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	//render_pass_create_info.attachmentCount = static_cast<uint32_t>(attachments.size());
	//render_pass_create_info.pAttachments = attachments.data();
	//render_pass_create_info.subpassCount = static_cast<uint32_t>(sub_passes.size());
	//render_pass_create_info.pSubpasses = sub_passes.data();

	//result = vkCreateRenderPass(ptrToRenderer->getVulkanDevice(), &render_pass_create_info, nullptr, &renderPass);
	//assert(result == VK_SUCCESS && "Failed to create render pass in initRenderPass()\n");
}

void Window::destroyRenderPass()
{
	//vkDestroyRenderPass(ptrToRenderer->getVulkanDevice(), renderPass, nullptr);
}

void Window::initFrameBuffers()
{
	//frameBufferVector.resize(swapchainImageCount);

	//for (uint32_t i = 0; i < swapchainImageCount; ++i) {
	//	std::array<VkImageView, 2> attachments{};
	//	attachments[0] = depthStencilImageView;
	//	attachments[1] = swapchainImageViewVector[i];

	//	VkFramebufferCreateInfo framebuffer_create_info{};
	//	framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	//	framebuffer_create_info.renderPass = renderPass;
	//	framebuffer_create_info.attachmentCount = static_cast<uint32_t>(attachments.size());
	//	framebuffer_create_info.pAttachments = attachments.data();
	//	framebuffer_create_info.width = surfaceWidth;
	//	framebuffer_create_info.height = surfaceHeight;
	//	framebuffer_create_info.layers = 1;

	//	//result = vkCreateFramebuffer(ptrToRenderer->getVulkanDevice(), &framebuffer_create_info, nullptr, &frameBufferVector[i]);
	//	//assert(result == VK_SUCCESS && "Failed to create frame buffer in initFrameBuffers()\n");
	//}
}

void Window::destroyFrameBuffers()
{
	//for (auto f : frameBufferVector) {
		//vkDestroyFramebuffer(ptrToRenderer->getVulkanDevice(), f, nullptr);
	//}
}

void Window::initSynchronizations()
{
	VkFenceCreateInfo fence_create_info{};
	fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	//result = vkCreateFence(ptrToRenderer->getVulkanDevice(), &fence_create_info, nullptr, &swapchainImageAvailable);
	//assert(result == VK_SUCCESS && "Failed to create fence in initSynchronizations()\n");

}

void Window::destroySynchronizations()
{
	//vkDestroyFence(ptrToRenderer->getVulkanDevice(), swapchainImageAvailable, nullptr);
}

uint32_t Window::FindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties* properties, const VkMemoryRequirements* requirements, const VkMemoryPropertyFlags propertyFlags)
{
	for (uint32_t i = 0; i < properties->memoryTypeCount; ++i)
	{
		if (requirements->memoryTypeBits & (1 << i))
		{
			if ((properties->memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags)
			{
				return i;
			}
		}
	}
	assert(0 && "Couldn't find proper memory type.\n");
	return UINT32_MAX;
}
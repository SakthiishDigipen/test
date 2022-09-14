#pragma once
#ifndef	VULKANSYSTEM_H
#define VULKANSYSTEM_H
#include <Utilities/Singleton/Singleton.h>
#include "../ISystem.h"
#include <Components/RenderObject.h>
#include <Precompiled/pch.h>
//idk why got errror if not included //TO-DO
#include <Maths/Matrix3D.h>

constexpr bool bUseValidationLayers = true;

namespace BE
{

	class PipelineBuilder {
	public:

		std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
		VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
		VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
		VkViewport _viewport;
		VkRect2D _scissor;
		VkPipelineRasterizationStateCreateInfo _rasterizer;
		VkPipelineColorBlendAttachmentState _colorBlendAttachment;
		VkPipelineMultisampleStateCreateInfo _multisampling;
		VkPipelineLayout _pipelineLayout;
		VkPipelineDepthStencilStateCreateInfo _depthStencil;
		VkPipeline build_pipeline(VkDevice device, VkRenderPass pass);
	};

	struct DeletionQueue
	{
		std::deque<std::function<void()>> deletors;

		void push_function(std::function<void()>&& function) {
			deletors.push_back(function);
		}

		void flush() {
			// reverse iterate the deletion queue to execute all the functions
			for (auto it = deletors.rbegin(); it != deletors.rend(); it++) {
				(*it)(); //call functors
			}

			deletors.clear();
		}
	};

	struct FrameData {
		VkSemaphore _presentSemaphore, _renderSemaphore;
		VkFence _renderFence;

		DeletionQueue _frameDeletionQueue;

		VkCommandPool _commandPool;
		VkCommandBuffer _mainCommandBuffer;

		AllocatedBuffer cameraBuffer;
		VkDescriptorSet globalDescriptor;

		AllocatedBuffer objectBuffer;
		VkDescriptorSet objectDescriptor;
	};

	struct UploadContext {
		VkFence _uploadFence;
		VkCommandPool _commandPool;
		VkCommandBuffer _commandBuffer;
	};

	struct GPUCameraData {
		glm::mat4 view;
		glm::mat4 proj;
		glm::mat4 viewproj;
	};

	struct GPUSceneData {
		glm::vec4 fogColor; // w is for exponent
		glm::vec4 fogDistances; //x for min, y for max, zw unused.
		glm::vec4 ambientColor;
		glm::vec4 sunlightDirection; //w for sun power
		glm::vec4 sunlightColor;
	};

	struct GPUObjectData {
		glm::mat4 modelMatrix;
	};

	constexpr unsigned int FRAME_OVERLAP = 2;

	class VulkanSystem : public Singleton<VulkanSystem>, public ISystem
	{
	public:
		bool _isInitialized{ false };
		int _frameNumber{ 0 };
		int _selectedShader{ 0 };

		//should be in glfw
		VkExtent2D _windowExtent{ 1700 , 900 };

		VkInstance _instance;
		VkDebugUtilsMessengerEXT _debug_messenger;
		VkPhysicalDevice _chosenGPU;
		VkDevice _device;

		VkPhysicalDeviceProperties _gpuProperties;

		FrameData _frames[FRAME_OVERLAP];

		VkQueue _graphicsQueue;
		uint32_t _graphicsQueueFamily;

		VkRenderPass _renderPass;

		VkSurfaceKHR _surface;
		VkSwapchainKHR _swapchain;
		VkFormat _swachainImageFormat;

		std::vector<VkFramebuffer> _framebuffers;
		std::vector<VkImage> _swapchainImages;
		std::vector<VkImageView> _swapchainImageViews;

		DeletionQueue _mainDeletionQueue;
	
		VmaAllocator _allocator; //vma lib allocator

		//depth resources
		VkImageView _depthImageView;
		AllocatedImage _depthImage;

		//the format for the depth image
		VkFormat _depthFormat;

		VkDescriptorPool _descriptorPool;

		VkDescriptorSetLayout _globalSetLayout;
		VkDescriptorSetLayout _objectSetLayout;
		VkDescriptorSetLayout _singleTextureSetLayout;

		GPUSceneData _sceneParameters;
		AllocatedBuffer _sceneParameterBuffer;

		UploadContext _uploadContext;

		
		//Constructors
		VulkanSystem();
		~VulkanSystem();

		void init_vulkan();

		void init_swapchain();

		void init_default_renderpass();

		void init_framebuffers();

		void init_commands();

		void init_sync_structures();

		void init_pipelines();

		void init_scene();

		void init_descriptors();

		//loads a shader module from a spir-v file. Returns false if it errors
		bool load_shader_module(const char* filePath, VkShaderModule* outShaderModule);

		void load_meshes();

		void load_images();

		void upload_mesh(Mesh& mesh);

		void draw();

		//System Function
		//void Init() override;		//Initialise all the systems and the engine
		//void Awake() override;		//Awake all the systems and the engine
		//void Update(double dt) override;		//Updates all the systems, to be called every frame
		//void Restart() override;		//For restarting all the systems
		//void Shutdown() override;	//Uninit the systems and free the memory
		

		FrameData& get_current_frame();

		FrameData& get_last_frame();
		//create material and add it to the map
		Material* create_material(VkPipeline pipeline, VkPipelineLayout layout, const std::string& name);
		//returns nullptr if it cant be found
		Material* get_material(const std::string& name);
		//returns nullptr if it cant be found
		Mesh* get_mesh(const std::string& name);
		//our draw function
		void draw_objects(VkCommandBuffer cmd, RenderObject* first, int count);

		AllocatedBuffer create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

		void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);

		size_t pad_uniform_buffer_size(size_t originalSize);

		ISYSTEM_FUNCTIONS

		friend class Singleton<VulkanSystem>;

	};

#define Vulkan VulkanSystem::GetInstance()
}
#endif
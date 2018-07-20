#pragma once
#include "Common.h"
#include "Instance.h"
#include "Surface.h"
#include "QueueFamilyIndices.h"
#include "Queue.h"
#include "LogicalDevice.h"
#include "ImageView.h"
#include "RenderPass.h"
#include "FrameBuffer.h"
#include "CommandPool.h"
#include "Pipeline.h"
#include "Shader.h"
#include "GraphicsPipeline.h"
#include "ComputePipeline.h"

#include "../Input/InputManager.h"

const int WIDTH = 800;
const int HEIGHT = 600;

glm::vec2 mousePosition = glm::vec2(0,0);

float frameLength = 0;
bool firstFrame = true;
bool shouldExit = false;

struct VkCamera {
	float	yaw;
	float	pitch;
	glm::vec3 position;
};

VkCamera camera = { { 0.0f },{ 0.0f },{ 0.0f,0.0f,10.0f } };

void Exit() {
	shouldExit = true;
}

void MouseMovement(double x, double y) {

	//float cfl = frameLength;

	//if (!firstFrame) {
		glm::vec2 mouseDelta = (glm::vec2(x, y) - mousePosition);
		mousePosition = glm::vec2(x, y);

		//deltaRoll = 0;

		camera.yaw += 0.0002f * mouseDelta.x;
		camera.pitch += 0.0002f * mouseDelta.y;
	//}
}

const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

float msec;
std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
		//auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkEnumerateInstanceVersion");
		if (func != nullptr) {
			func(instance, callback, pAllocator);
		}
	};
	const std::vector<uint16_t> indexes = {
		0, 1, 2, 2, 3, 0
	};

	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
		float time;
	};

	glm::vec3 multiply(glm::mat4 values, glm::vec3 v) {
		glm::vec3 vec;

		float temp;

		vec.x = v.x*values[0][0] + v.y*values[1][0] + v.z*values[2][0] + values[3][0];
		vec.y = v.x*values[0][1] + v.y*values[1][1] + v.z*values[2][1] + values[3][1];
		vec.z = v.x*values[0][2] + v.y*values[1][2] + v.z*values[2][2] + values[3][2];

		temp = v.x*values[0][3] + v.y*values[1][3] + v.z*values[2][3] + values[3][3];

		vec.x = vec.x / temp;
		vec.y = vec.y / temp;
		vec.z = vec.z / temp;

		return vec;
	};

	void MoveForward(){
		camera.position += multiply(glm::rotate(glm::mat4(), -camera.yaw, glm::vec3(0, 1, 0)), glm::vec3(0,0,-1)) * (frameLength * 20);
	};
	void MoveBack() {
		camera.position -= multiply(glm::rotate(glm::mat4(), -camera.yaw, glm::vec3(0, 1, 0)), glm::vec3(0, 0, -1)) * (frameLength * 20);
	};
	void MoveLeft() {
		camera.position += multiply(glm::rotate(glm::mat4(), -camera.yaw, glm::vec3(0, 1, 0)), glm::vec3(-1, 0, 0)) * (frameLength * 20);
	};
	void MoveRight() {
		camera.position -= multiply(glm::rotate(glm::mat4(), -camera.yaw, glm::vec3(0, 1, 0)), glm::vec3(-1, 0, 0)) * (frameLength * 20);
	};

	class Scene {
	public:
		void run() {
			UI();
			initWindow();
			initVulkan();
			mainLoop();
			cleanup();
		}
	private:
		void initWindow() {

			srand(time(NULL));

			glfwInit();

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //tells GLFW we're not using OpenGL
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //Handling Resize Window

			window = glfwCreateWindow(WIDTH, HEIGHT, "vulkan", nullptr, nullptr); //fourth input specifies moniter, last window is only relavent to OpenGL
			InputManager::Instance()->AttachToGLFW(window);
			InputManager::Instance()->LockCursor();
			InputManager::Instance()->OnCursorMovement(MouseMovement);
			InputManager::Instance()->OnKeyHold(FORWARD,MoveForward);
			InputManager::Instance()->OnKeyHold(BACK, MoveBack);
			InputManager::Instance()->OnKeyHold(LEFT, MoveLeft);
			InputManager::Instance()->OnKeyHold(RIGHT, MoveRight);
			InputManager::Instance()->OnKeyPress(QUIT, MoveRight);
		}
		void initVulkan() {
			createInstance();
			setupDebugCallback();
			createSurface();
			PickPhysicalDevice();
			CreateLogicalDevice();
			createSwapChain();
			createImageViews();
			createRenderPass();
			createDescriptorSetLayout();
			createGraphicsPipeline();
			createFramebuffers();
			createCommandPool();
			createVertexBuffer();
			createIndexBuffer();
			createUniformBuffer();
			createComputeBuffer();
			createDescriptorPool();
			createDescriptorSet();
			createCommandBuffers();
			createSemaphores();
		}

		void createSwapChain() {
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);


			VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
			VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
			VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

			uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
			if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
				imageCount = swapChainSupport.capabilities.maxImageCount;
			}

			VkSwapchainCreateInfoKHR createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			createInfo.surface = surface.getSurface();

			createInfo.minImageCount = imageCount;
			createInfo.imageFormat = surfaceFormat.format;
			createInfo.imageColorSpace = surfaceFormat.colorSpace;
			createInfo.imageExtent = extent;
			createInfo.imageArrayLayers = 1;
			createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //render directly to image

			uint32_t queueFamilyIndices[] = { (uint32_t)indices.getLocation("graphicsFamily"), (uint32_t)indices.getLocation("presentFamily"), (uint32_t)indices.getLocation("computeFamily") };

			if (indices.getLocation("graphicsFamily") != indices.getLocation("presentFamily")) {
				createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				createInfo.queueFamilyIndexCount = 2;
				createInfo.pQueueFamilyIndices = queueFamilyIndices;
			}
			else {
				createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				createInfo.queueFamilyIndexCount = 0; // Optional
				createInfo.pQueueFamilyIndices = nullptr; // Optional
			}

			createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

			createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

			createInfo.presentMode = presentMode;
			createInfo.clipped = VK_TRUE;

			createInfo.oldSwapchain = VK_NULL_HANDLE;

			if (vkCreateSwapchainKHR(device.getDevice(), &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
				throw std::runtime_error("failed to create swap chain!");
			}

			vkGetSwapchainImagesKHR(device.getDevice(), swapChain, &imageCount, nullptr);
			swapChainImages.resize(imageCount);
			vkGetSwapchainImagesKHR(device.getDevice(), swapChain, &imageCount, swapChainImages.data());

			swapChainImageFormat = surfaceFormat.format;
			swapChainExtent = extent;
		}

		void mainLoop() {
			while (!glfwWindowShouldClose(window) && !shouldExit) {
				InputManager::Instance()->PollEvents();
				updateUniformBuffer();
				drawFrame();
				firstFrame = false;
				frameCount++;
			}

			vkDeviceWaitIdle(device.getDevice());
		}
		void cleanup() {
			std::fstream fs;
			fs.open("results.txt", std::fstream::in | std::fstream::out | std::fstream::app);

			fs << maxParticles <<" particles: " << average << "ms\n";

			fs.close();

			vkDestroyFence(device.getDevice(), computeFence, nullptr);
			vkDestroyDescriptorPool(device.getDevice(), descriptorPool, nullptr);
			vkDestroyDescriptorSetLayout(device.getDevice(), descriptorSetLayout, nullptr);
			vkDestroyDescriptorSetLayout(device.getDevice(), computeSetLayout, nullptr);
			vkDestroyBuffer(device.getDevice(), uniformBuffer, nullptr);
			vkFreeMemory(device.getDevice(), uniformBufferMemory, nullptr);
			//vkDestroyBuffer(device.getDevice(), vertexBuffer, nullptr);
			//vkFreeMemory(device.getDevice(), vertexBufferMemory, nullptr);
			vkDestroyBuffer(device.getDevice(), computeBuffer, nullptr);
			vkFreeMemory(device.getDevice(), computeBufferMemory, nullptr);
			vkDestroySemaphore(device.getDevice(), renderFinishedSemaphore, nullptr);
			vkDestroySemaphore(device.getDevice(), imageAvailableSemaphore, nullptr);
			vkDestroyCommandPool(device.getDevice(), commandPool.getCommandPool(), nullptr);
			vkDestroyCommandPool(device.getDevice(), computeCommandPool.getCommandPool(), nullptr);
			for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
				vkDestroyFramebuffer(device.getDevice(), swapChainFramebuffers[i].getFramebuffer(), nullptr);
			}
			computePipeline.destroy();
			graphicsPipeline.destroy();

			vkDestroyRenderPass(device.getDevice(), renderPass.getRenderPass(), nullptr);
			for (size_t i = 0; i < swapChainImageViews.size(); i++) {
				vkDestroyImageView(device.getDevice(), swapChainImageViews[i].getSwapChainImageView(), nullptr);
			}

			vkDestroySwapchainKHR(device.getDevice(), swapChain, nullptr);

			vkDestroyDevice(device.getDevice(), nullptr);

			if (enableValidationLayers) {
				DestroyDebugReportCallbackEXT(instance.getInstance(), ValidationManager::Instance()->getCallback(), nullptr);
			}

			vkDestroySurfaceKHR(instance.getInstance(), surface.getSurface(), nullptr);
			vkDestroyInstance(instance.getInstance(), nullptr);

			glfwDestroyWindow(window);

			glfwTerminate();
		}

		void createInstance() {
			instance.SetName("ParticleSystem");
			instance.Construct();
		}

		bool checkValidationLayerSupport() { //check if requested layers are available
			uint32_t layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

			std::vector<VkLayerProperties> availableLayers;
			availableLayers.resize(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			for (const char* layerName : validationLayers) {
				bool layerFound = false;

				for (const auto& layerProperties : availableLayers) {
					if (strcmp(layerName, layerProperties.layerName) == 0) {
						layerFound = true;
						break;
					}
				}

				if (!layerFound) {
					return false;
				}
			}

			return true;
		}

		std::vector<const char*> getRequiredExtensions() {

			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

			/*for (unsigned int i = 0; i < glfwExtensionCount; i++) {
				extensions.push_back(glfwExtensions[i]);
			}*/

			if (enableValidationLayers) {
				extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
			}

			return extensions;
		}

		void setupDebugCallback() {
			ValidationManager::Instance()->setupDebugCallback(instance.getInstance());
		}

		void PickPhysicalDevice() {
			uint32_t deviceCount = 0;
			vkEnumeratePhysicalDevices(instance.getInstance(), &deviceCount, nullptr);

			if (deviceCount == 0) {
				throw std::runtime_error("failed to find GPUs with Vulkan support!");
			}

			std::vector<VkPhysicalDevice> devices(deviceCount);
			vkEnumeratePhysicalDevices(instance.getInstance(), &deviceCount, devices.data());

			for (const auto& device : devices) {
				if (isDeviceSuitable(device)) {
					physicalDevice = device;
					break;
				}
			}

			if (physicalDevice == VK_NULL_HANDLE) {
				throw std::runtime_error("failed to find a suitable GPU!");
			}

		}

		bool isDeviceSuitable(VkPhysicalDevice device) {
			/*VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties); //get name, type and supported vulkan versions

			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures); //get information on optional feature support*/


			std::string required[] = { "graphicsFamily","presentFamily", "computeFamily" };
			indices.setRequired(required, 3);
			indices.findQueueFamilies(device, surface);

			bool extensionsSupported = checkDeviceExtensionSupport(device);
			bool swapChainAdequate = false;

			if (extensionsSupported) {
				SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
				swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
			}

			return indices.isComplete() && extensionsSupported && swapChainAdequate;
		}

		int rateDeviceSuitability(VkPhysicalDevice device) {
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties); //get name, type and supported vulkan versions

			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures); //get information on optional feature support

			int score = 0;
			// Discrete GPUs have a significant performance advantage
			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				score += 1000;
			}
			// Maximum possible size of textures affects graphics quality
			score += deviceProperties.limits.maxImageDimension2D;
			// Application can't function without geometry shaders
			if (!deviceFeatures.geometryShader) {
				return 0;
			}
			return score;
		}

		void CreateLogicalDevice() {

			//specifying Queue to be created

			device.setQueueFamilies(indices.getIndicies());
			device.setDeviceExtensions(deviceExtensions);
			device.setPhysicalDevices(physicalDevice);

			device.Construct();

			//Retrieving Queues

			graphicsQueue.setDevice(device.getDevice());
			graphicsQueue.setIndex(indices.getLocation("graphicsFamily"));
			graphicsQueue.Construct();

			presentQueue.setDevice(device.getDevice());
			presentQueue.setIndex(indices.getLocation("presentFamily"));
			presentQueue.Construct();

			computeQueue.setDevice(device.getDevice());
			computeQueue.setIndex(indices.getLocation("computeFamily"));
			computeQueue.Construct();

		}

		void createSurface() {
			surface.setInstance(instance.getInstance());
			surface.setWindow(window);
			surface.Construct();
		}

		bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
			uint32_t extensionCount;
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

			std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

			for (const auto& extension : availableExtensions) {
				requiredExtensions.erase(extension.extensionName);
			}

			return requiredExtensions.empty();
		}

		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) {
			SwapChainSupportDetails details;

			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface.getSurface(), &details.capabilities);

			//quering surface fromats

			uint32_t formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface.getSurface(), &formatCount, nullptr);

			if (formatCount != 0) {
				details.formats.resize(formatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface.getSurface(), &formatCount, details.formats.data());
			}

			uint32_t presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface.getSurface(), &presentModeCount, nullptr);

			if (presentModeCount != 0) {
				details.presentModes.resize(presentModeCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface.getSurface(), &presentModeCount, details.presentModes.data());
			}

			return details;
		}

		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {

			if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
				return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
			}

			for (const auto& availableFormat : availableFormats) {
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
					return availableFormat;
				}
			}

			return availableFormats[0];
		}

		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
			VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

			for (const auto& availablePresentMode : availablePresentModes) {
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
					return availablePresentMode;
				}
				else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
					bestMode = availablePresentMode;
				}
			}

			return bestMode;
		}

		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
			if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
				return capabilities.currentExtent;
			}
			else {
				VkExtent2D actualExtent = { WIDTH, HEIGHT };

				actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
				actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

				return actualExtent;
			}
		}

		void createImageViews() {
			swapChainImageViews.resize(swapChainImages.size());
			for (size_t i = 0; i < swapChainImages.size(); i++) {
				ImageView imageView;
				imageView.setSwapChainImages(swapChainImages[i]);
				imageView.setSwapChainImageFormat(swapChainImageFormat);
				imageView.setDevice(device.getDevice());
				imageView.Construct();
				swapChainImageViews[i] = imageView;
			}
		}

		void createGraphicsPipeline() {
			/*auto vertShaderCode = readFile("../VulkanShader/vert.spv");
			auto fragShaderCode = readFile("../VulkanShader/frag.spv");

			vertShaderModule = createShaderModule(vertShaderCode);
			fragShaderModule = createShaderModule(fragShaderCode);

			VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
			vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;

			vertShaderStageInfo.module = vertShaderModule;
			vertShaderStageInfo.pName = "main";

			VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
			fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			fragShaderStageInfo.module = fragShaderModule;
			fragShaderStageInfo.pName = "main";*/

			VkShader vertexShader;
			vertexShader.setDevice(device.getDevice());
			vertexShader.setType(VK_SHADER_STAGE_VERTEX_BIT);
			vertexShader.readFile("../VulkanShader/vert.spv");
			vertexShader.Construct();

			VkShader fragmentShader;
			fragmentShader.setDevice(device.getDevice());
			fragmentShader.setType(VK_SHADER_STAGE_FRAGMENT_BIT);
			fragmentShader.readFile("../VulkanShader/frag.spv");
			fragmentShader.Construct();

			graphicsPipeline.setRenderPass(renderPass.getRenderPass());
			graphicsPipeline.setSwapChainExtent(swapChainExtent);
			graphicsPipeline.setDescriptorSetLayout(&descriptorSetLayout,1);

			graphicsPipeline.setDevice(device.getDevice());

			graphicsPipeline.setShader(vertexShader.getShaderModule(), VK_SHADER_STAGE_VERTEX_BIT);
			graphicsPipeline.setShader(fragmentShader.getShaderModule(), VK_SHADER_STAGE_FRAGMENT_BIT);

			graphicsPipeline.Construct();


			VkShader computeShader;
			computeShader.setDevice(device.getDevice());
			computeShader.setType(VK_SHADER_STAGE_COMPUTE_BIT);
			computeShader.readFile("../VulkanShader/comp.spv");
			computeShader.Construct();

			computePipeline.setDescriptorSetLayout(&computeSetLayout, 1);

			computePipeline.setDevice(device.getDevice());
			computePipeline.setShader(computeShader.getShaderModule(), VK_SHADER_STAGE_COMPUTE_BIT);
			computePipeline.Construct();

		}

		static std::vector<char> readFile(const std::string& filename) {
			std::ifstream file(filename, std::ios::ate | std::ios::binary);
			if (!file.is_open()) {
				throw std::runtime_error("failed to open file!");
			}

			size_t fileSize = (size_t)file.tellg();
			std::vector<char> buffer(fileSize);

			file.seekg(0);
			file.read(buffer.data(), fileSize);

			file.close();

			return buffer;
		}

		VkShaderModule createShaderModule(const std::vector<char>& code) {
			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = code.size();
			createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

			VkShaderModule shaderModule;
			if (vkCreateShaderModule(device.getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
				throw std::runtime_error("failed to create shader module!");
			}

			return shaderModule;
		}

		void createRenderPass() {
			renderPass.setSwapChainImageFormat(swapChainImageFormat);
			renderPass.setDevice(device.getDevice());
			renderPass.Construct();
		}

		void createFramebuffers() {
			swapChainFramebuffers.resize(swapChainImageViews.size());
			for (size_t i = 0; i < swapChainImageViews.size(); i++) {
				swapChainFramebuffers[i].setSwapChainImageView(swapChainImageViews[i].getSwapChainImageView());
				swapChainFramebuffers[i].setRenderPass(renderPass.getRenderPass());
				swapChainFramebuffers[i].setSwapChainExtent(swapChainExtent);
				swapChainFramebuffers[i].setDevice(device.getDevice());
				swapChainFramebuffers[i].Construct();
			}
		}

		void createCommandPool() {

			commandPool.setIndex(indices.getLocation("graphicsFamily"));
			commandPool.setDevice(device.getDevice());
			commandPool.Construct();

			computeCommandPool.setIndex(indices.getLocation("computeFamily"));
			computeCommandPool.setDevice(device.getDevice());
			computeCommandPool.Construct();

		}
		void createCommandBuffers() {
			commandBuffers.resize(swapChainFramebuffers.size());

			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = commandPool.getCommandPool();
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();
			if (vkAllocateCommandBuffers(device.getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
				throw std::runtime_error("failed to allocate command buffers!");
			}

			for (size_t i = 0; i < commandBuffers.size(); i++) {
				VkCommandBufferBeginInfo beginInfo = {};
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
				beginInfo.pInheritanceInfo = nullptr; // Optional
				vkBeginCommandBuffer(commandBuffers[i], &beginInfo);

				VkRenderPassBeginInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = renderPass.getRenderPass();
				renderPassInfo.framebuffer = swapChainFramebuffers[i].getFramebuffer();

				renderPassInfo.renderArea.offset = { 0, 0 };
				renderPassInfo.renderArea.extent = swapChainExtent;

				VkClearValue clearColor = { 0.2f, 0.2f, 0.2f, 1.0f };
				renderPassInfo.clearValueCount = 1;
				renderPassInfo.pClearValues = &clearColor;

				vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

				vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.getPipeline());

				VkBuffer vertexBuffers[] = { computeBuffer };
				VkDeviceSize offsets[] = { 0 };
				vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
				vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);

				vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.getPipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);

				//vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indexes.size()), 1, 0, 0, 0);
				vkCmdDraw(commandBuffers[i], static_cast<uint32_t>(maxParticles), 1, 0, 0);

				vkCmdEndRenderPass(commandBuffers[i]);

				if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
					throw std::runtime_error("failed to record command buffer!");
				}
			}


			computeCommandBuffers;

			VkCommandBufferAllocateInfo computeAllocInfo = {};
			computeAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			computeAllocInfo.commandPool = computeCommandPool.getCommandPool();
			computeAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			computeAllocInfo.commandBufferCount = (uint32_t)1;
			if (vkAllocateCommandBuffers(device.getDevice(), &computeAllocInfo, &computeCommandBuffers) != VK_SUCCESS) {
				throw std::runtime_error("failed to allocate command buffers!");
			}
				VkCommandBufferBeginInfo beginInfo = {};
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
				beginInfo.pInheritanceInfo = nullptr; // Optional
				vkBeginCommandBuffer(computeCommandBuffers, &beginInfo);

				/*VkBufferMemoryBarrier bufferMemoryBarrier{};
				bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
				bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

				bufferMemoryBarrier.buffer = computeBuffer;
				bufferMemoryBarrier.size = maxParticles * sizeof(Particle);
				bufferMemoryBarrier.srcAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;						// Vertex shader invocations have finished reading from the buffer
				bufferMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT;								// Compute shader wants to write to the buffer
																										// Transfer ownership if compute and graphics queue familiy indices differ
				bufferMemoryBarrier.srcQueueFamilyIndex = graphicsQueue.getIndex();
				bufferMemoryBarrier.dstQueueFamilyIndex = computeQueue.getIndex();

				vkCmdPipelineBarrier(
					computeCommandBuffers,
					VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
					VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
					0,
					0, nullptr,
					1, &bufferMemoryBarrier,
					0, nullptr);*/

				vkCmdBindPipeline(computeCommandBuffers, VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline.getPipeline());
				vkCmdBindDescriptorSets(computeCommandBuffers, VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline.getPipelineLayout(), 0, 1, &cDescriptorSet, 0, nullptr);

				vkCmdDispatch(computeCommandBuffers, maxParticles / 1024 , 1, 1);

				/*bufferMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT;								// Compute shader has finished writes to the buffer
				bufferMemoryBarrier.dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
				bufferMemoryBarrier.buffer = computeBuffer;
				bufferMemoryBarrier.size = maxParticles * sizeof(Particle);
				// No ownership transfer necessary
				bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;*/


				if (vkEndCommandBuffer(computeCommandBuffers) != VK_SUCCESS) {
					throw std::runtime_error("failed to record command buffer!");
				}

		}

		void drawFrame() {
			vkResetFences(device.getDevice(), 1, &computeFence);
			uint32_t imageIndex;
			vkAcquireNextImageKHR(device.getDevice(), swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

			//******submitting Compute Command Buffer******

			VkSubmitInfo cSubmitInfo = {};
			cSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			cSubmitInfo.commandBufferCount = 1;
			cSubmitInfo.pCommandBuffers = &computeCommandBuffers;

			if (vkQueueSubmit(computeQueue.getQueue(), 1, &cSubmitInfo, computeFence) != VK_SUCCESS) {
				throw std::runtime_error("failed to submit compute command buffer!");
			}

			vkWaitForFences(device.getDevice(), 1, &computeFence, VK_TRUE, UINT64_MAX);
			//******submitting Command Buffer******

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			VkSemaphore waitSemaphores[] = { imageAvailableSemaphore/*, computeFinishedSemaphore*/ };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStages;

			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

			VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;

			if (vkQueueSubmit(graphicsQueue.getQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
				throw std::runtime_error("failed to submit draw command buffer!");
			}

			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;

			VkSwapchainKHR swapChains[] = { swapChain };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;
			presentInfo.pImageIndices = &imageIndex;

			presentInfo.pResults = nullptr; // Optional

			vkQueuePresentKHR(presentQueue.getQueue(), &presentInfo);

			if (enableValidationLayers) {
				vkQueueWaitIdle(presentQueue.getQueue());
			}

		}

		void createSemaphores() {
			VkSemaphoreCreateInfo semaphoreInfo = {};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			if (vkCreateSemaphore(device.getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS||
				vkCreateSemaphore(device.getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS) {
				throw std::runtime_error("failed to create semaphores!");
			}

			VkFenceCreateInfo fenceCreateInfo{};
			fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;


			if (vkCreateFence(device.getDevice(), &fenceCreateInfo, nullptr, &computeFence) != VK_SUCCESS) {
				throw std::runtime_error("failed to create a fence!");
			}
		}

		void createVertexBuffer() {
			/*VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

			void* data;
			vkMapMemory(device.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
			memcpy(data, vertices.data(), (size_t)bufferSize);
			vkUnmapMemory(device.getDevice(), stagingBufferMemory);

			createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

			copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
			
			vkDestroyBuffer(device.getDevice(), stagingBuffer, nullptr);
			vkFreeMemory(device.getDevice(), stagingBufferMemory, nullptr);*/
		}

		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
				if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					return i;
				}
			}

			throw std::runtime_error("failed to find suitable memory type!");
		}

		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
			VkBufferCreateInfo bufferInfo = {};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = size;
			bufferInfo.usage = usage;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			/*if (queueFamilyIndex > -1) {
				uint32_t indices[] = { (uint32_t)queueFamilyIndex };
				bufferInfo.pQueueFamilyIndices = indices;
			}*/

			if (vkCreateBuffer(device.getDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
				throw std::runtime_error("failed to create vertex buffer!");
			}
			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(device.getDevice(), buffer, &memRequirements);

			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

			if (vkAllocateMemory(device.getDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
				throw std::runtime_error("failed to allocate vertex buffer memory!");
			}

			vkBindBufferMemory(device.getDevice(), buffer, bufferMemory, 0);
		}

		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandPool = commandPool.getCommandPool();
			allocInfo.commandBufferCount = 1;

			VkCommandBuffer commandBuffer;
			vkAllocateCommandBuffers(device.getDevice(), &allocInfo, &commandBuffer);

			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			vkBeginCommandBuffer(commandBuffer, &beginInfo);

			VkBufferCopy copyRegion = {};
			copyRegion.size = size;
			vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

			vkEndCommandBuffer(commandBuffer);

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;

			vkQueueSubmit(graphicsQueue.getQueue(), 1, &submitInfo, VK_NULL_HANDLE);
			vkQueueWaitIdle(graphicsQueue.getQueue());

			vkFreeCommandBuffers(device.getDevice(), commandPool.getCommandPool(), 1, &commandBuffer);

		}
		
		void createIndexBuffer() {
			VkDeviceSize bufferSize = sizeof(indexes[0]) * indexes.size();

			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

			void* data;
			vkMapMemory(device.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
			memcpy(data, indexes.data(), (size_t)bufferSize);
			vkUnmapMemory(device.getDevice(), stagingBufferMemory);

			createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

			copyBuffer(stagingBuffer, indexBuffer, bufferSize);

			vkDestroyBuffer(device.getDevice(), stagingBuffer, nullptr);
			vkFreeMemory(device.getDevice(), stagingBufferMemory, nullptr);
		}

		void createDescriptorSetLayout() {
			VkDescriptorSetLayoutBinding uboLayoutBinding[1] = {
				{0,VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,1,VK_SHADER_STAGE_VERTEX_BIT,0 }
			};

			VkDescriptorSetLayoutCreateInfo layoutInfo = {};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.bindingCount = 1;
			layoutInfo.pBindings = uboLayoutBinding;

			if (vkCreateDescriptorSetLayout(device.getDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
				throw std::runtime_error("failed to create descriptor set layout!");
			}

			VkDescriptorSetLayoutBinding descriptorSetLayoutBindings[2] = {
				{ 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT, 0 },
			{ 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT, 0 }
			};

			VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {
				VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
				0, 0, 2, descriptorSetLayoutBindings
			};

			if (vkCreateDescriptorSetLayout(device.getDevice(), &descriptorSetLayoutCreateInfo, nullptr, &computeSetLayout) != VK_SUCCESS) {
				throw std::runtime_error("failed to create descriptor set layout!");
			}
		}


		void createUniformBuffer() {
			VkDeviceSize bufferSize = sizeof(UniformBufferObject);
			createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffer, uniformBufferMemory);
		}

		void updateUniformBuffer() {
			static auto startTime = std::chrono::high_resolution_clock::now();

			auto currentTime = std::chrono::high_resolution_clock::now();
			if (firstFrame) {
				lastTime = currentTime;
			}
			frameLength = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
			lastTime = currentTime;

			UniformBufferObject ubo = {}; //__CAMERA
			//ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			/*ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));*/
			ubo.view = glm::rotate(glm::mat4(),camera.pitch, glm::vec3(1, 0, 0)) *
				glm::rotate(glm::mat4(), camera.yaw, glm::vec3(0, 1, 0)) *
				glm::translate(glm::mat4(), -camera.position);

			ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
			ubo.proj[1][1] *= -1;
			ubo.time = frameLength;

			void* data;
			vkMapMemory(device.getDevice(), uniformBufferMemory, 0, sizeof(ubo), 0, &data);
			memcpy(data, &ubo, sizeof(ubo));
			vkUnmapMemory(device.getDevice(), uniformBufferMemory);

			if (frameCount == 0) {

			}
			else if (frameCount == 1) {
				average = frameLength * 1000;
			}
			else {
				average = (average * (frameCount - 1) + frameLength * 1000)/frameCount;
			}
		}

		void createDescriptorPool() {
			VkDescriptorPoolSize poolSize[2] = {
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1 }};

			VkDescriptorPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.poolSizeCount = 2;
			poolInfo.pPoolSizes = poolSize;
			poolInfo.maxSets = 3;

			if (vkCreateDescriptorPool(device.getDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
				throw std::runtime_error("failed to create descriptor pool!");
			}
		}

		void createDescriptorSet() {
			VkDescriptorSetLayout layouts[] = { descriptorSetLayout };
			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = descriptorPool;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = layouts;
			if (vkAllocateDescriptorSets(device.getDevice(), &allocInfo, &descriptorSet) != VK_SUCCESS) {
				throw std::runtime_error("failed to allocate descriptor set!");
			}

			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = uniformBuffer;
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkDescriptorBufferInfo cBufferInfo = {};
			cBufferInfo.buffer = computeBuffer;
			cBufferInfo.offset = 0;
			cBufferInfo.range = VK_WHOLE_SIZE;

			VkWriteDescriptorSet descriptorWrite[1] ={
				{ VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,nullptr,descriptorSet,
				0,0,1,VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nullptr,&bufferInfo }
			};

			vkUpdateDescriptorSets(device.getDevice(), 1, descriptorWrite, 0, nullptr);

			/******compute Descriptor******/

			cDescriptorSet;

			VkDescriptorSetLayout cLayouts[] = { computeSetLayout};
			VkDescriptorSetAllocateInfo cAllocInfo = {};
			cAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			cAllocInfo.descriptorPool = descriptorPool;
			cAllocInfo.descriptorSetCount = 1;
			cAllocInfo.pSetLayouts = cLayouts;
			if (vkAllocateDescriptorSets(device.getDevice(), &cAllocInfo, &cDescriptorSet) != VK_SUCCESS) {
				throw std::runtime_error("failed to allocate descriptor set!");
			}

			VkDescriptorBufferInfo cBufferInfos1[] = { cBufferInfo};

			VkWriteDescriptorSet cDescriptorWrite[2] = {
				{ VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,nullptr,cDescriptorSet,
				0,0,1,VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, nullptr,cBufferInfos1 },
			{ VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,nullptr,cDescriptorSet,
				1,0,1,VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nullptr,&bufferInfo }
			};
			vkUpdateDescriptorSets(device.getDevice(), 2, cDescriptorWrite, 0, nullptr);
		}

		void initParticles(int maxParticles) {
			for (int i = 0; i < maxParticles; i++) {
				VkParticle p = { {-10000.0f,-10000.0f,-10000.0f, 0.0f }, {0.0f, 0.0f,0.0f,0.0f},{(float)(rand() % (xLMax - xLMin) + xLMin) / 100, (float)(rand() % (yLMax - yLMin) + yLMin) / 100, (float)(rand() % (zLMax - zLMin) + zLMin) / 100 ,0.0f},
				{ (float)(rand() % (xVMax - xVMin) + xVMin)/100, (float)(rand() % (yVMax - yVMin) + yVMin)/100, (float)(rand() % (zVMax - zVMin) + zVMin)/100, 0.0f } , {(float)(rand() % maxBirthTime)/100}, { rand() % (MaxLifeTime - MinLifeTime) + MinLifeTime } };
				particles[i] = p;
			}
		}

		void createComputeBuffer() {
			VkDeviceSize bufferSize = sizeof(VkParticle) * maxParticles;

			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

			void* data;
			vkMapMemory(device.getDevice(), stagingBufferMemory, 0, sizeof(VkParticle) * 1024, 0, &data);

			for (int i = 0; i < maxParticles / 1024; i++) {
				initParticles(1024);
				memcpy((char*)data + sizeof(VkParticle) * 1024 * i, particles, sizeof(VkParticle) * 1024);
			}
			if(maxParticles % 1024 !=0){
				initParticles(sizeof(VkParticle) % 1024);
				memcpy((char*)data + sizeof(VkParticle) * 1024 * (maxParticles / 1024), particles, maxParticles % 1024 * sizeof(VkParticle));
			}
			vkUnmapMemory(device.getDevice(), stagingBufferMemory);

			createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, computeBuffer, computeBufferMemory);

			copyBuffer(stagingBuffer, computeBuffer, bufferSize);

			vkDestroyBuffer(device.getDevice(), stagingBuffer, nullptr);
			vkFreeMemory(device.getDevice(), stagingBufferMemory, nullptr);
		}

		void UI() {
			std::cout << "how many sets of 1024 particles do you want to generate?\n";
			int input;
			std::cin >> input;
			maxParticles = 1024 * input;
		}

	GLFWwindow* window;
	//VkInstance instance;
	
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;


	LogicalDevice device;
	Queue graphicsQueue;
	Queue presentQueue;
	Queue computeQueue;
	
	Instance instance;
	Surface surface;
	QueueFamilyIndices indices;

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;
	
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorSetLayout computeSetLayout;

	GraphicsPipeline graphicsPipeline;
	ComputePipeline computePipeline;

	RenderPass renderPass;
	std::vector<ImageView> swapChainImageViews;
	std::vector<FrameBuffer> swapChainFramebuffers;

	CommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	CommandPool computeCommandPool;
	VkCommandBuffer computeCommandBuffers;
	
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;

	//VkBuffer vertexBuffer;
	//VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	
	VkBuffer computeBuffer;
	VkDeviceMemory computeBufferMemory;

	VkBuffer uniformBuffer;
	VkDeviceMemory uniformBufferMemory;

	VkDescriptorPool descriptorPool;
	VkDescriptorSet descriptorSet;

	VkDescriptorSet cDescriptorSet;

	VkFence computeFence;

	VkParticle particles[1024];

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	//std::vector<Vertex> vertices/* = {
		/*{ { -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f } },
	{ { 0.5f, -0.5f },{ 0.0f, 1.0f, 0.0f } },
	{ { 0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f } },
	{ { -0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f } }
	}*/;
	
	std::vector<glm::vec3> velocity = {
		glm::vec3(0.1,0.1,0.1),
		glm::vec3(0.1,0.1,0.1),
		glm::vec3(0.1,0.1,0.1)
	};

	const float dampeningFactor = 0.5;
	glm::vec3 force = glm::vec3(-20, 0,0);

	int xLMax = 500;
	int xLMin = -500;
	int yLMax = 001;
	int yLMin = 000;
	int zLMax = 500;
	int zLMin = -500;

	int xVMax = 100;
	int xVMin = -100;
	int yVMax = 100;
	int yVMin = -200;
	int zVMax = 100;
	int zVMin = -100;

	int maxBirthTime = 1000;
	int MinLifeTime = 5;
	int MaxLifeTime = 10;

	float average;
	int frameCount = 0;

	int maxParticles = 1024;
};
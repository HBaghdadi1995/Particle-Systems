#pragma once
#include "VulkanComponent.h"

class Instance : public VulkanComponent
{
public:
	Instance();
	~Instance();

	virtual void Construct() {
		if (enableValidationLayers && !ValidationManager::Instance()->checkValidationLayerSupport()) {
			throw std::runtime_error("validation layers requested, but not available!");
		}

		//optional information


		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = name.c_str();
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0; //__Compute



		//not optional

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;//__Compute

		//specify desired global extension
		unsigned int glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		//specify vulkan validation layers
		createInfo.enabledLayerCount = 0;

		//Add validationj layer names
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationManager::Instance()->validationLayers.size());
			createInfo.ppEnabledLayerNames = ValidationManager::Instance()->validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		auto requiredExtensions = ValidationManager::Instance()->getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
		createInfo.ppEnabledExtensionNames = requiredExtensions.data();


		//create vulkan instance
		//VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}

		//Checking for Extension Support

		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		//allocate an array to hold details
		std::vector<VkExtensionProperties> extensions;
		extensions.resize(extensionCount);

		//Query Extension Details
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		//list Extentions
		std::cout << "available extensions:" << std::endl;

		for (const auto& extension : extensions) {
			std::cout << "\t" << extension.extensionName << std::endl;
		}
	}

	VkInstance getInstance() {
		return instance;
	}

	void SetName(std::string name) {
		this->name = name;
	}

private:
	std::string name;

	VkInstance instance;

};

Instance::Instance()
{
}

Instance::~Instance()
{
}
#pragma once
#include "Common.h"
#include "../Input/TSingleton.h"

VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	//auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkEnumerateInstanceVersion");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

class ValidationManager : public TSingleton<ValidationManager>
{
	friend class TSingleton<ValidationManager>;
public:
	ValidationManager();
	~ValidationManager();

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

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallBack(
		VkDebugReportFlagsEXT flags, //type of message
		VkDebugReportObjectTypeEXT objType, //the type of the object that is the subject of the method
		uint64_t obj,
		size_t location,
		int32_t code,
		const char* layerPrefix,
		const char* msg, //pointer to messege
		void* userData) { //pass data to call back

		std::cerr << "validation layer: " << msg << std::endl;

		return VK_FALSE;
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

	const std::vector<const char*> validationLayers = { "VK_LAYER_LUNARG_standard_validation" };

	void setupDebugCallback(VkInstance instance) {
		if (!enableValidationLayers) return;

		VkDebugReportCallbackCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT; //filter massege type
		createInfo.pfnCallback = debugCallBack;
		if (CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug callback!");
		}
		//createInfo.pUserData //specify user input
	}

	VkDebugReportCallbackEXT getCallback() {
		return callback;
	}

private:

	VkDebugReportCallbackEXT callback;
};

ValidationManager::ValidationManager()
{
}

ValidationManager::~ValidationManager()
{
}
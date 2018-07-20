//#pragma once
//#include "ValidationManager.h"
//
//class PhysicalDevice
//{
//public:
//	PhysicalDevice();
//	~PhysicalDevice();
//
//	void setInstance(VkInstance instance) {
//		this->instance = instance;
//	}
//
//	bool isDeviceSuitable(VkPhysicalDevice device) {
//		/*VkPhysicalDeviceProperties deviceProperties;
//		vkGetPhysicalDeviceProperties(device, &deviceProperties); //get name, type and supported vulkan versions
//
//		VkPhysicalDeviceFeatures deviceFeatures;
//		vkGetPhysicalDeviceFeatures(device, &deviceFeatures); //get information on optional feature support*/
//
//		QueueFamilyIndices indices = findQueueFamilies(device); //find out if device supports graphic_bit
//
//		bool extensionsSupported = checkDeviceExtensionSupport(device);
//		bool swapChainAdequate = false;
//
//		if (extensionsSupported) {
//			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
//			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
//		}
//
//		return indices.isComplete() && extensionsSupported && swapChainAdequate;
//	}
//
//	void Construct() {
//		uint32_t deviceCount = 0;
//		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
//
//		if (deviceCount == 0) {
//			throw std::runtime_error("failed to find GPUs with Vulkan support!");
//		}
//
//		std::vector<VkPhysicalDevice> devices(deviceCount);
//		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
//
//		for (const auto& device : devices) {
//			if (isDeviceSuitable(device)) {
//				physicalDevice = device;
//				break;
//			}
//		}
//
//		if (physicalDevice == VK_NULL_HANDLE) {
//			throw std::runtime_error("failed to find a suitable GPU!");
//		}
//	}
//
//private:
//
//	VkInstance instance;
//	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
//};
//
//PhysicalDevice::PhysicalDevice()
//{
//}
//
//PhysicalDevice::~PhysicalDevice()
//{
//}
#pragma once
#include "VulkanComponent.h"

class LogicalDevice : public VulkanComponent
{
public:
	LogicalDevice();
	~LogicalDevice();

	virtual void Construct() {
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		float queuePriority = 1.0f;
		for (int queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}


		//specify device features

		VkPhysicalDeviceFeatures deviceFeatures = {};
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		deviceFeatures.largePoints = VK_TRUE;

		//Create Logical Device

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());//__Compute
		createInfo.pQueueCreateInfos = queueCreateInfos.data();//__Compute

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationManager::Instance()->validationLayers.size());
			createInfo.ppEnabledLayerNames = ValidationManager::Instance()->validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
			throw std::runtime_error("failed to create logical device!");
		}
	}

	void setDeviceExtensions(std::vector<const char*> extensions) {
		deviceExtensions = extensions;
	}

	void setQueueFamilies(std::set<int> queues) {
		uniqueQueueFamilies = queues;
	}

	void setPhysicalDevices(VkPhysicalDevice device) {
		physicalDevice = device;
	}

	VkDevice getDevice() {
		return device;
	}
	

private:
	VkDevice device;

	std::set<int> uniqueQueueFamilies;
	std::vector<const char*> deviceExtensions;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
};

LogicalDevice::LogicalDevice()
{
}

LogicalDevice::~LogicalDevice()
{
}
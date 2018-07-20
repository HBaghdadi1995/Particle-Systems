//#pragma once
//#include "VulkanComponent.h"
//
//struct SwapChainSupportDetails {
//	VkSurfaceCapabilitiesKHR capabilities;
//	std::vector<VkSurfaceFormatKHR> formats;
//	std::vector<VkPresentModeKHR> presentModes;
//};
//
//class SwapChain : public VulkanComponent
//{
//public:
//	SwapChain();
//	~SwapChain();
//
//	void setSurface(VkSurfaceKHR surface) {
//		this->surface = surface;
//	}
//
//	void setPhysicalDevice(VkPhysicalDevice device) {
//		this->physicalDevice = device;
//	}
//
//	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) {
//		SwapChainSupportDetails details;
//
//		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
//
//		//quering surface fromats
//
//		uint32_t formatCount;
//		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
//
//		if (formatCount != 0) {
//			details.formats.resize(formatCount);
//			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
//		}
//
//		uint32_t presentModeCount;
//		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
//
//		if (presentModeCount != 0) {
//			details.presentModes.resize(presentModeCount);
//			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
//		}
//
//		return details;
//	}
//
//	virtual void Construct() {
//		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);
//
//		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
//		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
//		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
//
//		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
//		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
//			imageCount = swapChainSupport.capabilities.maxImageCount;
//		}
//
//		VkSwapchainCreateInfoKHR createInfo = {};
//		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
//		createInfo.surface = surface.getSurface();
//
//		createInfo.minImageCount = imageCount;
//		createInfo.imageFormat = surfaceFormat.format;
//		createInfo.imageColorSpace = surfaceFormat.colorSpace;
//		createInfo.imageExtent = extent;
//		createInfo.imageArrayLayers = 1;
//		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //render directly to image
//
//		uint32_t queueFamilyIndices[] = { (uint32_t)indices.getLocation("graphicsFamily"), (uint32_t)indices.getLocation("presentFamily") };
//
//		if (indices.getLocation("graphicsFamily") != indices.getLocation("presentFamily")) {
//			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
//			createInfo.queueFamilyIndexCount = 2;
//			createInfo.pQueueFamilyIndices = queueFamilyIndices;
//		}
//		else {
//			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
//			createInfo.queueFamilyIndexCount = 0; // Optional
//			createInfo.pQueueFamilyIndices = nullptr; // Optional
//		}
//
//		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
//
//		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
//
//		createInfo.presentMode = presentMode;
//		createInfo.clipped = VK_TRUE;
//
//		createInfo.oldSwapchain = VK_NULL_HANDLE;
//
//		if (vkCreateSwapchainKHR(physicalDevice.getDevice(), &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
//			throw std::runtime_error("failed to create swap chain!");
//		}
//
//		vkGetSwapchainImagesKHR(physicalDevice.getDevice(), swapChain, &imageCount, nullptr);
//		swapChainImages.resize(imageCount);
//		vkGetSwapchainImagesKHR(physicalDevice.getDevice(), swapChain, &imageCount, swapChainImages.data());
//
//		swapChainImageFormat = surfaceFormat.format;
//		swapChainExtent = extent;
//	}
//
//private:
//	VkSurfaceKHR surface;
//	VkPhysicalDevice physicalDevice;
//};
//
//SwapChain::SwapChain()
//{
//}
//
//SwapChain::~SwapChain()
//{
//}
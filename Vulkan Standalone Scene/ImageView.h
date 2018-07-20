#pragma once
#include "VulkanComponent.h"

class ImageView : public VulkanComponent
{
public:
	ImageView();
	~ImageView();

	void setSwapChainImages(VkImage swapChainImages) {
		this->swapChainImage = swapChainImages;
	}
	void setSwapChainImageFormat(VkFormat swapChainImageFormat) {
		this->swapChainImageFormat = swapChainImageFormat;
	}
	void setDevice(VkDevice device) {
		this->device = device;
	}

	virtual void Construct() {
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = swapChainImage;

			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = swapChainImageFormat;

			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageView) != VK_SUCCESS) {
				throw std::runtime_error("failed to create image views!");
			}
	}

	VkImageView getSwapChainImageView() {
		return swapChainImageView;
	}

private:
	VkImageView swapChainImageView;

	VkImage swapChainImage;
	VkFormat swapChainImageFormat;
	VkDevice device;
};

ImageView::ImageView()
{
}

ImageView::~ImageView()
{
}
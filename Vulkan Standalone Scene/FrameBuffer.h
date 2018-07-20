#pragma once
#include "VulkanComponent.h"

class FrameBuffer : public VulkanComponent
{
public:
	FrameBuffer();
	~FrameBuffer();

	void setSwapChainImageView(VkImageView swapChainImageView) {
		this->swapChainImageView = swapChainImageView;
	}
	void setRenderPass(VkRenderPass renderPass) {
		this->renderPass = renderPass;
	}
	void setSwapChainExtent(VkExtent2D swapChainExtent) {
		this->swapChainExtent = swapChainExtent;
	}
	void setDevice(VkDevice device) {
		this->device = device;
	}

	virtual void Construct() {
		VkImageView attachments[] = {
			swapChainImageView
		};
		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;
		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}

	VkFramebuffer getFramebuffer() {
		return swapChainFramebuffer;
	}

private:
	VkFramebuffer swapChainFramebuffer;

	VkImageView swapChainImageView;
	VkRenderPass renderPass;
	VkExtent2D swapChainExtent;
	VkDevice device;
};

FrameBuffer::FrameBuffer()
{
}

FrameBuffer::~FrameBuffer()
{
}
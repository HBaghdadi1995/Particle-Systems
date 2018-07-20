//#pragma once
//#include "VulkanComponent.h"
//
//class CommandBuffer : public VulkanComponent
//{
//public:
//	CommandBuffer();
//	~CommandBuffer();
//
//	void setPipeline(VkPipeline pipeline) {
//		this->pipeline = pipeline;
//	}
//	void setFramebuffer(VkFramebuffer swapChainFrameBuffer) {
//		this->swapChainFrameBuffer = swapChainFrameBuffer;
//	}
//	void setRenderPass(VkRenderPass renderPass) {
//		this->renderPass = renderPass;
//	}
//	void setSwapChainExtent(VkExtent2D extent) {
//		this->swapChainExtent = extent;
//	}
//
//	virtual void Construct() {
//		VkCommandBufferBeginInfo beginInfo = {};
//		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
//		beginInfo.pInheritanceInfo = nullptr; // Optional
//		vkBeginCommandBuffer(commandBuffer, &beginInfo);
//
//		VkRenderPassBeginInfo renderPassInfo = {};
//		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//		renderPassInfo.renderPass = renderPass;
//		renderPassInfo.framebuffer = swapChainFrameBuffer;
//
//		renderPassInfo.renderArea.offset = { 0, 0 };
//		renderPassInfo.renderArea.extent = swapChainExtent;
//
//		VkClearValue clearColor = { 0.2f, 0.2f, 0.2f, 1.0f };
//		renderPassInfo.clearValueCount = 1;
//		renderPassInfo.pClearValues = &clearColor;
//
//		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
//
//		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
//
//		vkCmdDraw(commandBuffer, 3, 1, 0, 0);
//
//		vkCmdEndRenderPass(commandBuffer);
//
//		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
//			throw std::runtime_error("failed to record command buffer!");
//		}
//	}
//
//	VkCommandBuffer getCommandBuffer() {
//		return commandBuffer;
//	}
//
//private:
//	VkCommandBuffer commandBuffer;
//
//	VkPipeline pipeline;
//	VkFramebuffer swapChainFrameBuffer;
//	VkRenderPass renderPass;
//	VkExtent2D swapChainExtent;
//};
//
//CommandBuffer::CommandBuffer()
//{
//}
//
//CommandBuffer::~CommandBuffer()
//{
//}
#pragma once
#include "VulkanComponent.h"

class CommandPool : public VulkanComponent
{
public:
	CommandPool();
	~CommandPool();

	void setIndex(int index) {
		this->index = index;
	}
	void setDevice(VkDevice device) {
		this->device = device;
	}

	virtual void Construct() {
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = index;
		poolInfo.flags = 0; // Optional

		if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create command pool!");
		}
	}

	VkCommandPool getCommandPool() {
		return commandPool;
	}
private:
	VkCommandPool commandPool;

	int index;
	VkDevice device;
};

CommandPool::CommandPool()
{
}

CommandPool::~CommandPool()
{
}
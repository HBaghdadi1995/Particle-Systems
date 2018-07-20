#pragma once
#include "VulkanComponent.h"

class Queue: public VulkanComponent
{
public:
	Queue();
	~Queue();

	void setDevice(VkDevice device) {
		this->device = device;
	}
	void setIndex(int index) {
		this->index = index;
	}
	int getIndex() {
		return index;
	}

	VkQueue getQueue() {
		return queue;
	}

	virtual void Construct() {
		vkGetDeviceQueue(device, index, 0, &queue);
	}

private:
	VkQueue queue;

	VkDevice device;
	int index;
};

Queue::Queue()
{
}

Queue::~Queue()
{
}
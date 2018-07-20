#pragma once
#include "VulkanComponent.h"

class Surface : public VulkanComponent
{
public:
	Surface() {

	}
	~Surface() {

	}

	void setInstance(VkInstance instance) {
		this->instance = instance;
	}

	void setWindow(GLFWwindow* window) {
		this->window = window;
	}

	virtual void Construct() {
		if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

	VkSurfaceKHR getSurface() {
		return surface;
	}

protected:
	VkSurfaceKHR surface;

	VkInstance instance;
	GLFWwindow* window;
};

#pragma once
#include "Common.h"
#include "ValidationManager.h"

class VulkanComponent
{
public:
	VulkanComponent();
	~VulkanComponent();

	virtual void Construct() = 0;

private:

};

VulkanComponent::VulkanComponent()
{
}

VulkanComponent::~VulkanComponent()
{
}
#pragma once
#include "VulkanComponent.h"

class VkShader : public VulkanComponent
{
public:
	VkShader();
	~VkShader();



	void setDevice(VkDevice device) {
		this->device = device;
	}

	virtual void readFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		//std::vector<char> buffer(fileSize);
		code.clear();
		code.resize(fileSize);

		file.seekg(0);
		file.read(code.data(), fileSize);

		file.close();
	}

	void setType(VkShaderStageFlagBits type) {
		this->type = type;
	}

	virtual void Construct() {
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		
		if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module!");
		}

		
		shaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderCreateInfo.stage = type;
		shaderCreateInfo.module = shaderModule;
		shaderCreateInfo.pName = "main";
	}

	VkShaderModule getShaderModule() {
		return shaderModule;
	}

	VkPipelineShaderStageCreateInfo getShaderCreateInfo() {
		return shaderCreateInfo;
	}

protected:
	VkDevice device;
	VkExtent2D swapChainExtent;

	VkShaderStageFlagBits type;
	std::vector<char>& code = std::vector<char>();

	VkShaderModule shaderModule;
	VkPipelineShaderStageCreateInfo shaderCreateInfo = {};
};

VkShader::VkShader()
{
}

VkShader::~VkShader()
{
	vkDestroyShaderModule(device, shaderModule, nullptr);
}
#pragma once
#include "VulkanComponent.h"

struct VkParticle {
	glm::vec4 location;
	glm::vec4 velocity;
	glm::vec4 initLocation;
	glm::vec4 initVelocity;

	float lifeTime;
	float initLifetime;
	glm::vec2 doublePadding;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(VkParticle);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 6> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 6> attributeDescriptions = {};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(VkParticle, location);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(VkParticle, velocity);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(VkParticle, initLocation);

		attributeDescriptions[3].binding = 0;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[3].offset = offsetof(VkParticle, initVelocity);

		attributeDescriptions[4].binding = 0;
		attributeDescriptions[4].location = 4;
		attributeDescriptions[4].format = VK_FORMAT_R32_SFLOAT;
		attributeDescriptions[4].offset = offsetof(VkParticle, lifeTime);

		attributeDescriptions[5].binding = 0;
		attributeDescriptions[5].location = 5;
		attributeDescriptions[5].format = VK_FORMAT_R32_SFLOAT;
		attributeDescriptions[5].offset = offsetof(VkParticle, initLifetime);

		return attributeDescriptions;
	}
};

class Pipeline : public VulkanComponent
{
public:
	Pipeline() {

	}
	~Pipeline() {
	}

	void destroy() {
		if (pipeline != VK_NULL_HANDLE) {
			vkDestroyPipeline(device, pipeline, nullptr);
			pipeline = VK_NULL_HANDLE;
		}
		if (pipelineLayout != VK_NULL_HANDLE) {
			vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
			pipelineLayout = VK_NULL_HANDLE;
		}
	}

	void setShader(VkShaderModule shader, VkShaderStageFlagBits type) {
		VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = type;
		vertShaderStageInfo.module = shader;
		vertShaderStageInfo.pName = "main";

		shaders.push_back(shader);
		shaderCIs.push_back(vertShaderStageInfo);
	}

	void setDevice(VkDevice device) {
		this->device = device;
	}

	

	VkPipelineLayout getPipelineLayout() {
		return pipelineLayout;
	}

	VkPipeline getPipeline() {
		return pipeline;
	}

	void setDescriptorSetLayout(VkDescriptorSetLayout * setLayouts, int i) {
		this->setLayouts = setLayouts;
		this->setLayoutsSize = i;
	}

protected:
	VkDescriptorSetLayout * setLayouts = nullptr;
	int setLayoutsSize;

	std::vector<VkPipelineShaderStageCreateInfo> shaderCIs;
	std::vector<VkShaderModule> shaders;

	VkDevice device;

	VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
	VkPipeline pipeline = VK_NULL_HANDLE;
};

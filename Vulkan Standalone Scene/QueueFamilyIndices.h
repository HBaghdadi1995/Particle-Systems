#pragma once
#include <map>
#include <vector>

class QueueFamilyIndices
{
public:
	QueueFamilyIndices();
	~QueueFamilyIndices();

	/*void setRequired(std::vector<std::string> required) {
		for (std::vector<std::string>::iterator it = required.begin(); it != required.end(); it++) {
			queueFamilies[*it] = -1;
		}
	}*/

	void setRequired(std::string required[], int size) {
		for (int i = 0; i < size; i++) {
			queueFamilies[required[i]] = -1;
		}
	}

	int getLocation(std::string queue) {
		return queueFamilies[queue];
	}

	void setLocation(std::string queue, int i) {
		queueFamilies[queue] = i;
	}

	bool isComplete() {
		for (std::map<std::string, int>::iterator it = queueFamilies.begin(); it != queueFamilies.end(); it++) {
			if (it->second < 0) {
				return false;
			}
		}
		return true;
	}

	void findQueueFamilies(VkPhysicalDevice device, Surface surface) {

		for (std::map<std::string, int>::iterator it = queueFamilies.begin(); it != queueFamilies.end(); it++) {
			it->second = -1;
		}

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (isRequired("graphicsFamily") && queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				setLocation("graphicsFamily", i);
			}
			if (isRequired("presentFamily") && queueFamily.queueCount > 0) {
				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface.getSurface(), &presentSupport);
				if(presentSupport)
					setLocation("presentFamily", i);
			}
			if (isRequired("computeFamily")) {
				if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
					setLocation("computeFamily", i);
				}
			}

			if (isComplete()) {
				break;
			}

			i++;
		}
	}

	bool isRequired(std::string str) {
		for (std::map<std::string, int>::iterator it = queueFamilies.begin(); it != queueFamilies.end(); it++) {
			if(it->first.compare(str) == 0) {
				return true;
			}
		}
		return false;
	}

	std::set<int> getIndicies() {
		std::set<int> indicies;
		for (std::map<std::string, int>::iterator it = queueFamilies.begin(); it != queueFamilies.end(); it++) {
			indicies.insert(it->second);
		}
		return indicies;
	}

private:
	std::map<std::string, int> queueFamilies;
};

QueueFamilyIndices::QueueFamilyIndices()
{
}

QueueFamilyIndices::~QueueFamilyIndices()
{
}
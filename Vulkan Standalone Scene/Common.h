#pragma once
 
#define NDEBUG

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan\vulkan.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <stdio.h>
#include <string.h>
#include <stdexcept>
#include <vector>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif
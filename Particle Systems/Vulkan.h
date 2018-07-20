#pragma once
#include "../Vulkan Standalone Scene/Scene.h"

class Vulkan
{
public:
	Vulkan();
	~Vulkan();

	int Run();

private:
	Scene app;

};

Vulkan::Vulkan()
{
}

Vulkan::~Vulkan()
{
}

int Vulkan::Run() {
	app;

	try {
		app.run();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}
}

#include "OpenGL.h"
#include "Vulkan.h"
//

//namespace OpenGL {
//};

int main() {
	int selection = 1;

	std::cout << "what implementation would you like to test ?\n";
	std::cout << "type 0 for the Vulkan implementation with compute shaders.\n";
	std::cout << "type 1 for the OpenGl implementation with transform feedback.\n";

	std::cin >> selection;

	if (selection == 0) {
		Scene app;

		try {
			app.run();
		}
		catch (const std::runtime_error& e) {
			std::cerr << e.what() << std::endl;
			return -1;
		}

		return 0;
	}

	if (selection == 1){

		std::cout << "how many sets of 1024 particles do you want to generate?\n";
		int maxParticles;
		int input;
		std::cin >> input;
		maxParticles = 1024 * input;

		Window w("Coursework", 800, 600, false); //This is all boring win32 window creation stuff!
		if (!w.HasInitialised()) {
			return -1;
		}

		srand((unsigned int)w.GetTimer()->GetMS() * 1000.0f);

		Renderer renderer(w); //This handles all the boring OGL 3.2 initialisation stuff, and sets up our tutorial!
		renderer.GenerateParticles(maxParticles);
		if (!renderer.HasInitialised()) {
			return -1;
		}

		w.LockMouseToWindow(true);
		w.ShowOSPointer(false);

		while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
			renderer.UpdateScene(w.GetTimer()->GetTimedMS());
			renderer.RenderScene();
		}

		return 0;
	}

	return 0;
}
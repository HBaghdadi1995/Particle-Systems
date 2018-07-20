#pragma comment(lib, "nclgl.lib")

#include "../NCLGL/window.h"
#include "Renderer.h"

int main() {

	std::cout << "how many sets of 1024 particles do you want to generate?\n";
	int maxParticles;
	int input;
	std::cin >> input;
	maxParticles = 1024 * input;

	Window w("Particle Systems (Open GL)", 800, 600, false); //This is all boring win32 window creation stuff!
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
#pragma once
#include "../Graphics Coursework/Renderer.h"

class OpenGL
{
public:
	OpenGL();
	~OpenGL();

	int Initialise();
	int Loop();

private:
	Window w;
	Renderer renderer;
};

OpenGL::OpenGL()
{
}

OpenGL::~OpenGL()
{
}

int OpenGL::Initialise() {
	w = Window("Coursework", 800, 600, false); //This is all boring win32 window creation stuff!
	if (!w.HasInitialised()) {
		return -1;
	}

	std::srand((unsigned int)w.GetTimer()->GetMS() * 1000.0f);

	renderer = Renderer(w); //This handles all the boring OGL 3.2 initialisation stuff, and sets up our tutorial!
	if (!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	return 0;
}

int OpenGL::Loop() {
	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}

	return 0;
}
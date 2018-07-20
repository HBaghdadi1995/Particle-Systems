#pragma once

//#include <GLFW\glfw3.h>
#include <map>

using namespace std;

//A List of commands that will be mapped to keyboard inputs

//Author Hasan Al-Baghdadi

enum Commands {
	JUMP,
	SHOOT,
	ALT_SHOOT,
	RELOAD,
	FORWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	BACK,
	QUIT,
	ALT_FORWARD,
	ALT_BACK,
	ALT_LEFT,
	ALT_RIGHT,
	NUM_COMMANDS,
	DEBUG_DRAW,
	PAUSE,
	INFO,
	ACCEPT

};

class KeyMap {
public:
	KeyMap() {}
	~KeyMap() {}

	void AddNew(Commands, int); //Add another command button pair
	int getButton(Commands c); //Get a button using a key
protected:
	map<Commands, int> m_keyMap;
private:
};
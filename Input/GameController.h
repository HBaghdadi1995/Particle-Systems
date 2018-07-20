#pragma once
#include <map>
#include <GLFW\glfw3.h>

#include <string>
#include <iostream>

//Author Hasan AL-Baghdadi

enum ControllerButton {
	Xbox_A = 0, PS4_X = 0, PS4_CROSS = 0,
	Xbox_B = 1, PS4_O = 1, PS4_CIRCLE = 1,
	Xbox_X = 2, PS4_SQUARE = 2,
	Xbox_Y = 3, PS4_TRIANGLE = 3,
	Xbox_LB = 4, PS4_L1 = 4,
	Xbox_RB = 5, PS4_R1 = 5,
	Xbox_BACK = 6, PS4_SHARE = 6,
	Xbox_START = 7, PS4_START = 7,
	Xbox_LEFT_STICK = 8, PS4_L3 = 8,
	Xbox_RIGHT_STICK = 9, PS4_R3 = 9,
	Xbox_UP = 10, PS4_UP= 10,
	Xbox_LEFT = 11, PS4_LEFT = 11,
	Xbox_RIGHT = 12, PS4_RIGHT = 12,
	Xbox_DOWN = 13, PS4_DOWN =13,
	//Please avoid using these buttons, they do not exist on
	//The Xbox Controller
	PS4_HOME = 14, Xbox_MAX = 14,
	PS4_PAD = 15,
	PS4_L2_B = 16,
	PS4_R2_B = 17,
	PS4_MAX = 18
};

enum Axis {
	X_LEFT = 0,
	Y_LEFT = 1,
	X_RIGHT = 2,
	Y_RIGHT = 3,
	Xbox_LT = 4, PS4_L2_A = 5,
	Xbox_RT = 5, PS4_R2_A = 5
};

class GameController {
public:

	GameController(int);
	std::string GetName();
	~GameController();


	int getControllerId();
	bool getButtonState(ControllerButton);

	void SetKeyStates();
	float GetAxisState(Axis);

	void UnbindController();

	bool isConnected;

protected:
	std::string m_name;
	int m_controllerId;

	void mapXb360();
	void mapPS4();

	void insertButtons(ControllerButton, int);
	void insertAxis(Axis, int);

	std::map<ControllerButton, int> m_buttonsMap;
	std::map<int, bool> m_buttonStates;
	std::map<int, float> m_axesStates;
	std::map<Axis, int> m_axisMap;
};
#include "GameController.h"

GameController::GameController(int i) {
	m_controllerId = i;
	m_name = glfwGetJoystickName(i);

	if (!m_name.compare("Xbox 360 Controller")) {
		mapXb360();
	}
	else if (!m_name.compare("Wireless Controller")) {
		mapPS4();
	}
}

std::string GameController::GetName() {
	return m_name;
}
GameController::~GameController() {

}

int GameController::getControllerId() {
	return m_controllerId;
}

void GameController::mapXb360() {
	insertButtons(Xbox_A, 0);
	insertButtons(Xbox_B, 1);
	insertButtons(Xbox_X, 2);
	insertButtons(Xbox_Y, 3);
	insertButtons(Xbox_LB, 4);
	insertButtons(Xbox_RB, 5);
	insertButtons(Xbox_BACK, 6);
	insertButtons(Xbox_START, 7);
	insertButtons(Xbox_LEFT_STICK, 8);
	insertButtons(Xbox_RIGHT_STICK, 9);
	insertButtons(Xbox_UP, 10);
	insertButtons(Xbox_RIGHT, 11);
	insertButtons(Xbox_DOWN, 12);
	insertButtons(Xbox_LEFT, 13);

	insertAxis(X_LEFT, 0);
	insertAxis(Y_LEFT, 1);
	insertAxis(X_RIGHT, 2);
	insertAxis(Y_RIGHT, 3);
	insertAxis(Xbox_LT, 4);
	insertAxis(Xbox_RT, 5);
}

void GameController::mapPS4() {
	insertButtons(PS4_SQUARE, 0);
	insertButtons(PS4_X, 1);
	insertButtons(PS4_O, 2);
	insertButtons(PS4_TRIANGLE, 3);
	insertButtons(PS4_L1, 4);
	insertButtons(PS4_R1, 5);
	insertButtons(PS4_L2_B, 6);
	insertButtons(PS4_R2_B, 7);
	insertButtons(PS4_SHARE, 8);
	insertButtons(PS4_START, 9);
	insertButtons(PS4_L3, 10);
	insertButtons(PS4_R3, 11);
	insertButtons(PS4_HOME, 12);
	insertButtons(PS4_PAD, 13);
	insertButtons(PS4_UP, 14);
	insertButtons(PS4_DOWN, 16);
	insertButtons(PS4_LEFT, 17);
	insertButtons(PS4_RIGHT, 15);

	insertAxis(X_LEFT, 0);
	insertAxis(Y_LEFT, 1);
	insertAxis(X_RIGHT, 2);
	insertAxis(PS4_L2_A, 3);
	insertAxis(PS4_R2_A, 4);
	insertAxis(Y_RIGHT, 5);
}

void GameController::insertButtons(ControllerButton c, int i) {
	std::pair<ControllerButton, int> temp = std::pair<ControllerButton, int>(c, i);
	m_buttonsMap.insert(temp);
}

void GameController::insertAxis(Axis a, int i) {
	std::pair<Axis, int> temp = std::pair<Axis, int>(a, i);
	m_axisMap.insert(temp);
}
bool GameController::getButtonState(ControllerButton c) {
	return m_buttonStates[m_buttonsMap[c]];
}

void GameController::SetKeyStates() {
	int count;
	const unsigned char* buttons = glfwGetJoystickButtons(m_controllerId, &count);

	for (int i = 0; i < count; i++) {
		m_buttonStates[i] = buttons[i];
	}
	
	count;
	const float* axes = glfwGetJoystickAxes(m_controllerId, &count);

	for (int i = 0; i < count; i++) {
		m_axesStates[i] = axes[i];
	}
}

float GameController::GetAxisState(Axis a) {
	return m_axesStates[m_axisMap[a]];
}
void GameController::UnbindController()
{

}

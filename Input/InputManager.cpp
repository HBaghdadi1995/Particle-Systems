#include "InputManager.h"

//initialise all static components here



multimap<int, VoidFunction> InputManager::m_CommandFuncMap = multimap<int, VoidFunction>();
multimap<int, VoidFunction> InputManager::m_MouseCommandFuncMap = multimap<int, VoidFunction>();
multimap<int, VoidFunction> InputManager::m_HeldFuncMap = multimap<int, VoidFunction>();
KeyMap *InputManager::m_currentKeyMap = NULL;
KeyMap *InputManager::m_currentMouseMap = NULL;
vector<DoubleFunction> InputManager::m_MouseFuncVector = vector<DoubleFunction>();
vector<DoubleFunction> InputManager::m_ScrollFuncVector = vector<DoubleFunction>();
vector<VoidFunction> InputManager::m_enterFunctions = vector<VoidFunction>();
vector<VoidFunction> InputManager::m_leaveFunctions = vector<VoidFunction>();
map<Commands, bool> InputManager::m_HeldMap = map<Commands, bool>();
multimap<int, VoidFunction> InputManager::m_disconnectFunctions = multimap<int, VoidFunction>();
multimap<int, VoidFunction> InputManager::m_connectFunctions = multimap<int, VoidFunction>();
map<int, bool> InputManager::m_controllerIsConnected = map<int, bool>();

vector<GameController> InputManager::m_controllers = vector<GameController>();
map<int, multimap<int, VoidFunction>> InputManager::m_controllerButtonMap = map<int, multimap<int, VoidFunction>>();
map<int, multimap<Axis, SingleFunction>> InputManager::m_axisMap = map<int, multimap<Axis, SingleFunction>>();
map<ControllerButton, bool> InputManager::m_HeldControllerMap = map<ControllerButton, bool>();


int InputManager::m_numberOfControllers = 0;

InputManager::InputManager() {
	InitialiseControllers();
}

InputManager::~InputManager() {
}



void InputManager::AttachToGLFW(GLFWwindow * w) {
	m_window = w;

	ConstructDefaultKeyBindings();

	glfwSetKeyCallback(w, KeyCallback);
	glfwSetCursorPosCallback(w, CursorPosCallback);
	glfwSetCursorEnterCallback(w, CursorEnterCallback);
	glfwSetMouseButtonCallback(w, MouseButtonCallback);
	glfwSetScrollCallback(w, ScrollCallBack);
	glfwSetJoystickCallback(JoystickConnectCallback);
}

int InputManager::OnKeyPress(Commands c, CommandFunc func) {
	
	VoidFunction f = VoidFunction(m_funcNums, func);
	
	AddNewCommand(c, PRESS, f);
	
	return f.GetID();
}

int InputManager::OnKeyRelease(Commands c, CommandFunc func) {

	VoidFunction f = VoidFunction(m_funcNums, func);

	AddNewCommand(c, RELEASE, f);

	return f.GetID();
}

int InputManager::OnKeyHold(Commands c, CommandFunc func) {
	VoidFunction f = VoidFunction(m_funcNums, func);
	//Creates events for bothing pressing and releasing the button
	AddNewCommand(c, PRESS, f);
	AddNewCommand(c, RELEASE, f);
	//Adds function to map holding functions that should be ran on hold
	
	m_HeldFuncMap.insert(pair<int, VoidFunction>(c, f));

	return f.GetID();
}

int InputManager::OnMousePress(Commands c, CommandFunc func) {
	VoidFunction f = VoidFunction(m_funcNums, func);

	AddNewMouseCommand(c, PRESS, f);

	return f.GetID();
}

int InputManager::OnMouseRelease(Commands c, CommandFunc func) {
	VoidFunction f = VoidFunction(m_funcNums, func);

	AddNewMouseCommand(c, RELEASE, f);

	return f.GetID();
}

int InputManager::OnMouseHold(Commands c, CommandFunc func) {
	VoidFunction f = VoidFunction(m_funcNums, func);
	//Creates events for bothing pressing and releasing the button
	AddNewMouseCommand(c, PRESS, f);
	AddNewMouseCommand(c, RELEASE, f);
	//Adds function to map holding functions that should be ran on hold

	m_HeldFuncMap.insert(pair<int, VoidFunction>(c, f));

	return f.GetID();
}

int InputManager::OnCursorMovement(CursorPositionUpdate func) {
	DoubleFunction f = DoubleFunction(m_funcNums, func);

	m_MouseFuncVector.push_back(f);

	return f.GetID();
}


void InputManager::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{

	for (map<int, VoidFunction>::iterator it = m_CommandFuncMap.begin(); it != m_CommandFuncMap.end();) {

		//This line has been created to extract both action and command from m_CommandFuncMap
		//the number 3 is used because there is a maximum of three action values

		int c = (it->first)/3;
		int a = (it->first)%3;

		if (key == m_currentKeyMap->getButton((Commands)c) && action == a) {

			//Sets key as held
			if (action == PRESS) {
				m_HeldMap[(Commands)c] = true;
			}

			//Sets key as no longer held (given press and release are a pair of events)
			if (action == RELEASE) {
				m_HeldMap[(Commands)c] = false;
			}

			int size = m_CommandFuncMap.size();
			(it->second).Run();
			if (m_CommandFuncMap.size() < size) {
				it = m_CommandFuncMap.begin();
			}
			else {
				++it;
			}
		}
		else {
			++it;
		}
	}
}

void InputManager::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
	if (m_MouseCommandFuncMap.size() == 1) {

		int c = (m_MouseCommandFuncMap.begin()->first) / 3;
		int a = (m_MouseCommandFuncMap.begin()->first) % 3;

		if (button == m_currentMouseMap->getButton((Commands)c) && action == a) {

			//Sets key as held
			if (action == PRESS) {
				m_HeldMap[(Commands)c] = true;
			}

			//Sets key as no longer held (given press and release are a pair of events)
			if (action == RELEASE) {
				m_HeldMap[(Commands)c] = false;
			}

			(m_MouseCommandFuncMap.begin()->second).Run();

		}
	}
	else {
		for (map<int, VoidFunction>::iterator it = m_MouseCommandFuncMap.begin(); it != m_MouseCommandFuncMap.end();) {

			//This line has been created to extract both action and command from m_CommandFuncMap
			//the number 3 is used because there is a maximum of three action values

			int c = (it->first) / 3;
			int a = (it->first) % 3;
			if (button == m_currentMouseMap->getButton((Commands)c) && action == a) {

				//Sets key as held
				if (action == PRESS) {
					m_HeldMap[(Commands)c] = true;
				}

				//Sets key as no longer held (given press and release are a pair of events)
				if (action == RELEASE) {
					m_HeldMap[(Commands)c] = false;
				}

				int size = m_MouseCommandFuncMap.size();
				(it->second).Run();
				if (size > m_MouseCommandFuncMap.size()) {
					it = m_MouseCommandFuncMap.begin();
				}
				else {
					++it;
				}
			}
			else {
				++it;
			}
		}
	}

}

void InputManager::CursorPosCallback(GLFWwindow *, double x, double y)
{
	for (vector<DoubleFunction>::iterator it = m_MouseFuncVector.begin(); it != m_MouseFuncVector.end(); it++) {
		(*it).Run(x, y);
	}
}

void InputManager::CursorEnterCallback(GLFWwindow *, int i) {
	if (i) {
		for (vector<VoidFunction>::iterator it = m_enterFunctions.begin(); it != m_enterFunctions.end(); it++) {
			(*it).Run();
		}
	}
	else {
		for (vector<VoidFunction>::iterator it = m_leaveFunctions.begin(); it != m_leaveFunctions.end(); it++) {
			(*it).Run();
		}
	}
};

void InputManager::AddNewCommand(Commands c, Action a, VoidFunction func) {
	//This line has been created so that both action and command could account for in m_CommandFuncMap
	//the number 3 is used because there is a maximum of three action values
	int cAndA = 3 * c + a;
	
	pair<int, VoidFunction> temp = pair<int, VoidFunction>(cAndA, func);
	m_CommandFuncMap.insert(temp);
}

void InputManager::AddNewMouseCommand(Commands c, Action a, VoidFunction func) {
	//This line has been created so that both action and command could account for in m_CommandFuncMap
	//the number 3 is used because there is a maximum of three action values
	int cAndA = 3 * c + a;

	pair<int, VoidFunction> temp = pair<int, VoidFunction>(cAndA, func);
	m_MouseCommandFuncMap.insert(temp);
}

void InputManager::ConstructDefaultKeyBindings() {
	KeyMap keyMap = KeyMap();
	keyMap.AddNew(JUMP, GLFW_KEY_SPACE);
	keyMap.AddNew(FORWARD, GLFW_KEY_W);
	keyMap.AddNew(LEFT, GLFW_KEY_A);
	keyMap.AddNew(RIGHT, GLFW_KEY_D);
	keyMap.AddNew(UP, GLFW_KEY_E);
	keyMap.AddNew(DOWN, GLFW_KEY_Q);
	keyMap.AddNew(BACK, GLFW_KEY_S);
	keyMap.AddNew(RELOAD, GLFW_KEY_R);
	keyMap.AddNew(QUIT, GLFW_KEY_ESCAPE);
	keyMap.AddNew(ALT_FORWARD, GLFW_KEY_UP);
	keyMap.AddNew(ALT_BACK, GLFW_KEY_DOWN);
	keyMap.AddNew(ALT_LEFT, GLFW_KEY_LEFT);
	keyMap.AddNew(ALT_RIGHT, GLFW_KEY_RIGHT);
	keyMap.AddNew(DEBUG_DRAW, GLFW_KEY_O);
	keyMap.AddNew(PAUSE, GLFW_KEY_P);
	keyMap.AddNew(INFO, GLFW_KEY_I);
	keyMap.AddNew(ACCEPT, GLFW_KEY_ENTER);

	m_keyMaps.push_back(keyMap);
	m_currentKeyMap = &(*(m_keyMaps.end() - 1));

	KeyMap mouseMap = KeyMap();

	mouseMap.AddNew(SHOOT, GLFW_MOUSE_BUTTON_1);
	mouseMap.AddNew(ALT_SHOOT, GLFW_MOUSE_BUTTON_2);

	m_mouseMaps.push_back(mouseMap);
	m_currentMouseMap = &(*(m_mouseMaps.end() - 1));
}

void InputManager::ScrollCallBack(GLFWwindow* w, double x, double y) {
	for (vector<DoubleFunction>::iterator it = m_ScrollFuncVector.begin(); it != m_ScrollFuncVector.end(); it++) {
		(*it).Run(x, y);
	}
}

void InputManager::JoystickConnectCallback(int joy, int event) {
	if (event == GLFW_CONNECTED)
	{
		// The joystick was connected
		addController(joy);
		for (multimap<int, VoidFunction>::iterator it = m_connectFunctions.begin(); it != m_connectFunctions.end(); it++) {
			if (it->first == joy) {
				it->second.Run();
			}
		}
	}
	else if (event == GLFW_DISCONNECTED)
	{

		for (multimap<int, VoidFunction>::iterator it = m_disconnectFunctions.begin(); it != m_disconnectFunctions.end(); it++) {
			if (it->first == joy) {
				it->second.Run();
			}
		}
		
		int candidateForDeletion = NULL;
		for (vector<GameController>::iterator it = m_controllers.begin(); it != m_controllers.end(); it++) {
			if (it->getControllerId() == joy) {
				candidateForDeletion = (int)(it - m_controllers.begin());
			}
		}
		if (candidateForDeletion != NULL) {
			m_controllers.erase(m_controllers.begin() + candidateForDeletion);
		}
	}
}

void InputManager::RunHeldFunctions()
{
	//Checks each command for whether it is held down
		for (map<Commands, bool>::iterator i = m_HeldMap.begin(); i != m_HeldMap.end(); i++) {
			if ((*i).second == true) {
				pair<multimap<int, VoidFunction>::iterator, multimap<int, VoidFunction>::iterator> result = m_HeldFuncMap.equal_range((*i).first);

				//Runs each on hold function for the command
				for (multimap<int, VoidFunction>::iterator j = result.first; j != result.second; j++) {
					(*j).second.Run();
				}
			}
		}
}

void InputManager::LockCursor() {

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


}
void InputManager::HideCursor() {

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

}
void InputManager::ReleaseCursor() {

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

int InputManager::OnCursorEnter(CommandFunc func) {
	VoidFunction f = VoidFunction(m_funcNums, func);
	m_enterFunctions.push_back(f);
	return f.GetID();
}
int InputManager::OnCursorLeave(CommandFunc func) {
	VoidFunction f = VoidFunction(m_funcNums, func);
	m_leaveFunctions.push_back(f);
	return f.GetID();
}


void InputManager::CreateCursor(const GLFWimage* i, int x, int y) {
	m_cursor = glfwCreateCursor(i, x, y);
	glfwSetCursor(m_window, m_cursor);
}

void InputManager::DestroyCursor() {
	glfwDestroyCursor(m_cursor);
}




int InputManager::OnMouseScroll(CursorPositionUpdate func) {
	DoubleFunction f = DoubleFunction(m_funcNums, func);
	m_ScrollFuncVector.push_back(f);
	return f.GetID();
}

int InputManager::onControllerPress(int i, ControllerButton c, CommandFunc func) {
	VoidFunction f = VoidFunction(m_funcNums, func);
	m_controllerButtonMap[i].insert(std::pair<int, VoidFunction>(3 * c + PRESS, f));
	return f.GetID();
}
int InputManager::onControllerRelease(int i, ControllerButton c, CommandFunc func) {
	VoidFunction f = VoidFunction(m_funcNums, func);
	m_controllerButtonMap[i].insert(std::pair<int, VoidFunction>(3 * c + RELEASE, f));
	return f.GetID();
}
int InputManager::onControllerHold(int i, ControllerButton c, CommandFunc func) {
	VoidFunction f = VoidFunction(m_funcNums, func);
		m_controllerButtonMap[i].insert(std::pair<int, VoidFunction>(3 * c + PRESS, f));
		m_controllerButtonMap[i].insert(std::pair<int, VoidFunction>(3 * c + REPEAT, f));
		m_controllerButtonMap[i].insert(std::pair<int, VoidFunction>(3 * c + RELEASE, f));
		return f.GetID();
}

int InputManager::onAxisMovement(int i, Axis a, AxisPostionFunction func) {
	SingleFunction f = SingleFunction(m_funcNums, func);
	m_axisMap[i].insert(std::pair<Axis, SingleFunction>(a, f));
	return f.GetID();
}

void InputManager::Run() {

	for (map<int, multimap<int, VoidFunction>>::iterator it = m_controllerButtonMap.begin(); it != m_controllerButtonMap.end(); it++) {
		if (ControllerExists(it->first) ) {

			m_controllers[it->first].SetKeyStates();
			if(m_controllers[it->first].isConnected){
				for (multimap<int, VoidFunction>::iterator jt = it->second.begin(); jt != it->second.end(); jt++) {

					int c = (jt->first) / 3;
					int a = (jt->first) % 3;


					if (m_controllers.at(it->first).getButtonState((ControllerButton)c) && !m_HeldControllerMap[(ControllerButton)c]) {
						m_HeldControllerMap[(ControllerButton)c] = true;
						if (a == PRESS) {
							(jt->second).Run();
						}
					}
					else if (!m_controllers.at(it->first).getButtonState((ControllerButton)c) && m_HeldControllerMap[(ControllerButton)c]) {
						m_HeldControllerMap[(ControllerButton)c] = false;
						if (a == RELEASE) {
							(jt->second).Run();
						}
					}
					else if (m_controllers.at(it->first).getButtonState((ControllerButton)c) && m_HeldControllerMap[(ControllerButton)c] && a == REPEAT) {
						(jt->second).Run();
					}
				}
			}
		}

	}


	for (map<int, multimap<Axis, SingleFunction>>::iterator it = m_axisMap.begin(); it != m_axisMap.end(); it++) {
		if (ControllerExists(it->first) && m_controllers[it->first].isConnected) {
			for (multimap<Axis, SingleFunction>::iterator jt = it->second.begin(); jt != it->second.end(); jt++) {

				if (!m_controllers.at(it->first).GetName().compare("Wireless Controller")) {
					double s = m_controllers.at(it->first).GetAxisState((Axis)jt->first);
					if (!(s < 0.25 &&
						s > -0.25)) {
						if (jt->first != X_RIGHT) {
							(jt->second).Run(s);
						}
						else {
							(jt->second).Run(-s);
						}
					}
				}
				else if (!m_controllers.at(it->first).GetName().compare("Xbox 360 Controller")) {
					if (jt->first == X_LEFT) {
						(jt->second).Run(m_controllers.at(it->first).GetAxisState((Axis)jt->first));
					}
					else if (jt->first == Y_RIGHT || jt->first == X_RIGHT) {
						(jt->second).Run(-m_controllers.at(it->first).GetAxisState((Axis)jt->first) * 5);
					}
					else if(jt->first == Xbox_RT || jt->first == Xbox_LT){
						(jt->second).Run((m_controllers.at(it->first).GetAxisState((Axis)jt->first) +1 )/2);
					}else {
						(jt->second).Run(-m_controllers.at(it->first).GetAxisState((Axis)jt->first));
					}
				}
				else {
					(jt->second).Run(-m_controllers.at(it->first).GetAxisState((Axis)jt->first));
				}
			}
		}
	}

}

bool InputManager::ControllerExists(int i) {

	if (m_controllers.size() <= i) {
		return false;
	}

	return true;
}


void InputManager::addController(int i) {
	m_controllers.push_back(GameController(i));
}


void InputManager::InitialiseControllers() {

	for (int i = 0; i < 16; i++) {
		if (glfwJoystickPresent(i)) {
			addController(i);
			pair<int, multimap<int, VoidFunction>> temp = pair<int, multimap<int, VoidFunction>>(m_controllers.size() - 1, multimap<int, VoidFunction>());
			m_controllerButtonMap.insert(temp);
			m_numberOfControllers++;
		}
	}
}

int InputManager::GetNumberOfControllers() {
	return m_numberOfControllers;
}

void InputManager::RemoveKeyCommand(Commands c) {
	m_CommandFuncMap.erase((int)c * 3);
	m_CommandFuncMap.erase((int)c * 3 + 1);
	m_CommandFuncMap.erase((int)c * 3 + 2);
}
void InputManager::RemoveHeldKeyCommand(Commands c)
{
	RemoveKeyCommand(c);
	m_HeldFuncMap.erase(c);
}
void InputManager::RemoveMouseButtonCommand(Commands c) {
	//multimap<int, CommandFunc> newMap = multimap<int, CommandFunc>();
	//for (multimap<int, CommandFunc>::iterator it = m_MouseCommandFuncMap.begin(); it != m_MouseCommandFuncMap.end(); it++) {
	//	//m_MouseCommandFuncMap.erase(it);
	//}
	m_MouseCommandFuncMap.erase((int)c * 3);
	m_MouseCommandFuncMap.erase((int)c * 3 + 1);
	m_MouseCommandFuncMap.erase((int)c * 3 + 2);
	
}

void InputManager::RemoveController(int i) {
	m_controllers[i].UnbindController();
	m_controllers.erase(m_controllers.begin() + i);
}

int InputManager::OnControllerDisconnect(int i, CommandFunc c) {
	VoidFunction f = VoidFunction(m_funcNums, c);
	pair<int, VoidFunction> temp = pair<int, VoidFunction>(i, f);
	m_disconnectFunctions.insert(temp);
	return f.GetID();
}

int InputManager::OnControllerConnect(int i, CommandFunc c) {
	VoidFunction f = VoidFunction(m_funcNums, c);
	pair<int, VoidFunction> temp = pair<int, VoidFunction>(i, f);
	m_connectFunctions.insert(temp);
	return f.GetID();
}

void InputManager::RemoveAllKeyCommands() {
	m_CommandFuncMap.clear();
	m_HeldFuncMap.clear();
}

void InputManager::RemoveAllMouseCommands() {
	m_MouseCommandFuncMap.clear();
	m_HeldFuncMap.clear();
}

void InputManager::RemoveCursorMovement() {
	m_MouseFuncVector.clear();
}

void InputManager::RemoveMouseScroll() {
	m_ScrollFuncVector.clear();
}

void InputManager::RemoveCursorEnter() {
	m_enterFunctions.clear();
}

void InputManager::RemoveCursorLeave() {
	m_leaveFunctions.clear();
}

void InputManager::RemoveControllerCommand(int i, ControllerButton c) {
	m_controllerButtonMap[i].erase((int)c * 3);
	m_controllerButtonMap[i].erase((int)c * 3 + 1);
	m_controllerButtonMap[i].erase((int)c * 3 + 2);
}

void InputManager::RemoveAllControllerCommands(int i) {
	m_controllerButtonMap[i].clear();
	m_controllerButtonMap[i] = multimap<int, VoidFunction>();
}

void InputManager::RemoveAxisCommand(int i, Axis c) {
	m_axisMap[i].erase(c);
}

void InputManager::RemoveAllAxisCommands(int i) {
	m_axisMap[i].clear();
}

void InputManager::ClearControllers() {
	m_axisMap.clear();
}

void InputManager::RemoveKeyCommand(int i) {
	for (multimap<int, VoidFunction>::iterator it = m_CommandFuncMap.begin(); it != m_CommandFuncMap.end(); it++) {
		if (it->second.GetID() == i) {
			m_CommandFuncMap.erase(it);
			return;
		}
	}
}
void InputManager::RemoveHeldKeyCommand(int i) {
	for (multimap<int, VoidFunction>::iterator it = m_HeldFuncMap.begin(); it != m_HeldFuncMap.end(); it++) {
		if (it->second.GetID() == i) {
			m_HeldFuncMap.erase(it);
			return;
		}
	}
}
void InputManager::RemoveMouseButtonCommand(int i) {
	for (multimap<int, VoidFunction>::iterator it = m_MouseCommandFuncMap.begin(); it != m_MouseCommandFuncMap.end(); it++) {
		if (it->second.GetID() == i) {
			int key = it->first;
			m_MouseCommandFuncMap.erase(it);
			for (multimap<int, VoidFunction>::iterator it = m_HeldFuncMap.begin(); it != m_HeldFuncMap.end(); it++) {
				if (it->second.GetID() == i) {
					int key = it->first;
					m_MouseCommandFuncMap.erase(it);
					return;
				}
			}
			return;
		}
	}
}
void InputManager::RemoveCursorMovement(int i) {
	for (vector<DoubleFunction>::iterator it = m_MouseFuncVector.begin(); it != m_MouseFuncVector.end(); it++) {
		if (it->GetID() == i) {
			m_MouseFuncVector.erase(it);
			return;
		}
	}
}
void InputManager::RemoveMouseScroll(int i) {
	for (vector<DoubleFunction>::iterator it = m_ScrollFuncVector.begin(); it != m_ScrollFuncVector.end(); it++) {
		if (it->GetID() == i) {
			m_ScrollFuncVector.erase(it);
			return;
		}
	}
}
void InputManager::RemoveCursorEnter(int i) {
	for (vector<VoidFunction>::iterator it = m_enterFunctions.begin(); it != m_enterFunctions.end(); it++) {
		if (it->GetID() == i) {
			m_enterFunctions.erase(it);
			return;
		}
	}
}
void InputManager::RemoveCursorLeave(int i) {
	for (vector<VoidFunction>::iterator it = m_leaveFunctions.begin(); it != m_leaveFunctions.end(); it++) {
		if (it->GetID() == i) {
			m_leaveFunctions.erase(it);
			return;
		}
	}
}
void InputManager::RemoveControllerCommand(int c, int i) {
	for (map<int, multimap<int, VoidFunction>>::iterator it = m_controllerButtonMap.begin(); it != m_controllerButtonMap.end(); it++) {
		for (multimap<int, VoidFunction>::iterator jt = it->second.begin();jt != it->second.end(); jt++) {
			if (jt->second.GetID() == i) {
				m_controllerButtonMap[c].erase(jt);
				return;
			}
		}
	}
}
void InputManager::RemoveAxisCommand(int c, int i) {
	for (map<int, multimap<Axis, SingleFunction>>::iterator it = m_axisMap.begin(); it != m_axisMap.end(); it++) {
		for (multimap<Axis, SingleFunction>::iterator jt = it->second.begin();jt != it->second.end(); jt++) {
			if (jt->second.GetID() == i) {
				m_axisMap[c].erase(jt);
				return;
			}
		}
	}
}
void InputManager::RemoveControllerConnect(int c, int i) {
	for (multimap<int, VoidFunction>::iterator it = m_connectFunctions.begin(); it != m_connectFunctions.end(); it++) {
			if (it->second.GetID() == i) {
				m_connectFunctions.erase(it);
				return;
		}
	}
}
void InputManager::RemoveControllerDisconnect(int c, int i) {
	for (multimap<int, VoidFunction>::iterator it = m_disconnectFunctions.begin(); it != m_disconnectFunctions.end(); it++) {
		if (it->second.GetID() == i) {
			m_disconnectFunctions.erase(it);
			return;
		}
	}
}
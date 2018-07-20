#pragma once

#include <vector>
#include <glm\glm.hpp>
#include "GameController.h"
#include <map>
#include "KeyMap.h"
#include "Functions.h"
#include "TSingleton.h"

using namespace std;

//Created by Hasan Al-Baghdadi
//Contact me if you don't understand how this class works

//Modified by Luke and Josh

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~Class F.A.Q.~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//Q1. How do I start an instance of this class?
//A1. as a singletn, you should call InputManager::Instance and
//    then use the function AttachToGLFW which takes in a GLFWwindow.
//
//Q2. How do I call functions after a key is pressed?
//A2. This class uses a callback method rather than returning a
//    bool. To do this you basically call one of the on...
//    functions and wait for it to register on the next frame
//    update. A release function should be added later on to debind
//    a function from a key.
//
//Q3. How do I use the on... functions?
//A3. At the moment there are 3 types of on functions, each will
//    have their own sections.
//    a) Action Functions:
//
//       Examples of these functions include:
//			OnKeyPress, OnKeyRelease, OnKeyHold, OnMousePress,
//			OnMouseRelease, OnMouseHold
//
//       These functions take a command and a function as an input.
//
//       The commands are defined in the keyMap class and are designed
//       to allow for Key Rebinding.
//       
//       Function inputs should all return NULLs and take no values,
//       if the function input is not a member of a class then then
//       putting it in is as simple as just writing in the function
//       name. However if the function is a member of a class then
//       your input should look like this:
//			std::bind(&className::function, &instanceOfClass)
//
//    b) Coordinate Functions:
//       
//       Examples of these functions include:
//			OnCursorMovement, OnMouseScroll
//
//		 These functions take a function as an input.
//
//       Function inputs should all return NULLs and take two doubles,
//       if the function input is not a member of a class then then
//       putting it in is as simple as just writing in the function
//       name. However if the function is a member of a class then
//       your input should look like this:
//			std::bind(&className::function, &instanceOfClass,
//			std::placeholders::_1,std::placeholders::_2)
//
//    c) bool Functions:
//
//		 Examples of these functions include:
//			OnCursorEnter, OnCursorLeave
//
//		 These functions take a function as an input.
//
//       Function inputs should all return NULLs and take no values,
//       if the function input is not a member of a class then then
//       putting it in is as simple as just writing in the function
//       name. However if the function is a member of a class then
//       your input should look like this:
//			std::bind(&className::function, &instanceOfClass)
//
//		TODO:
//			-Allow for multiple controllers, when multile players are implemented
//			-Bind controlleres to player
//			-Allow for unbinding inputs


//The Action used on the button

enum Action {
	RELEASE = 0,
	PRESS = 1,
	REPEAT = 2
};


class InputManager : public TSingleton<InputManager> {
	friend class TSingleton<InputManager>;
public:


	bool PollEvents() { 

		if (glfwWindowShouldClose(m_window)) {
			return true;
		}
		else {
			glfwPollEvents();
			RunHeldFunctions();
			Run();
			return false;

		}
	 }

	void AttachToGLFW(GLFWwindow* w);
	void ConstructDefaultKeyBindings(); //Constructs the defult keybindings
										//the default key bunding should no longer be in use after file IO is implemented

	//Functions used to bind a command to a function

	//~~~Action Functions~~~
	int OnKeyPress(Commands,CommandFunc); //takes in a command and void function with no input, runs when a button is first pressed
	int OnKeyRelease(Commands, CommandFunc); //takes in a command and void function with no input, runs when a button is released
	int OnKeyHold(Commands, CommandFunc); //takes in a command and void function with no input, , runs when a button is held down

	int OnMousePress(Commands, CommandFunc); //takes in a command and void function with no input, runs when a mouse button is first pressed
	int OnMouseRelease(Commands, CommandFunc);
	int OnMouseHold(Commands, CommandFunc);

	//~~~Coordinate Functions~~~
	int OnCursorMovement(CursorPositionUpdate func); //takes in a command with 2 doubles as inputs, the x and y value are relative to the top-left
	int OnMouseScroll(CursorPositionUpdate);

	//~~~bool Functions~~~
	int OnCursorEnter(CommandFunc);
	int OnCursorLeave(CommandFunc);

	void LockCursor(); //locks the cursor to the screen and hides it
	void HideCursor(); //hides the cursor when above the screen
	void ReleaseCursor(); //the cursor is never hidden nor locked

	void RemoveKeyCommand(Commands);
	void RemoveHeldKeyCommand(Commands);
	void RemoveMouseButtonCommand(Commands);
	void RemoveAllKeyCommands();
	void RemoveAllMouseCommands();
	void RemoveCursorMovement();
	void RemoveMouseScroll();
	void RemoveCursorEnter();
	void RemoveCursorLeave();

	void RemoveKeyCommand(int);
	void RemoveHeldKeyCommand(int);
	void RemoveMouseButtonCommand(int);
	void RemoveCursorMovement(int);
	void RemoveMouseScroll(int);
	void RemoveCursorEnter(int);
	void RemoveCursorLeave(int);


	void CreateCursor(const GLFWimage*, int, int); //create a custom cursor and set its custom location, the coordinates are relative to the top-left, call DestroyCursor at end

	void DestroyCursor(); //Destory the custom cursor

	inline glm::vec2 GetCursorPosition() {
		double x, y;
		glfwGetCursorPos(m_window, &x, &y);
		return glm::vec2(x, y);
		return glm::vec2(0, 0);
		
	}


	//Controllers

	void static addController(int); //should add a new controller to the system

	int onControllerPress(int, ControllerButton, CommandFunc);
	int onControllerRelease(int, ControllerButton, CommandFunc);
	int onControllerHold(int, ControllerButton, CommandFunc);

	int onAxisMovement(int, Axis, AxisPostionFunction);

	int OnControllerDisconnect(int, CommandFunc);
	int OnControllerConnect(int, CommandFunc);

	void RemoveControllerCommand(int, ControllerButton);
	void RemoveAllControllerCommands(int);
	void RemoveAxisCommand(int, Axis);
	void RemoveAllAxisCommands(int);
	void ClearControllers();

	void RemoveControllerCommand(int, int);
	void RemoveAxisCommand(int, int);

	void RemoveControllerConnect(int, int);
	void RemoveControllerDisconnect(int, int);

	void Run();

	void static InitialiseControllers();
	bool ControllerExists(int i);

	int GetNumberOfControllers();

	void RemoveController(int);

protected:
	InputManager(); //Call this constructor at the beginning of the class
	~InputManager();

	GLFWwindow *m_window; //referenece to the window

	static KeyMap *m_currentKeyMap; //pointer to the current keybinding
	vector<KeyMap> m_keyMaps; //a list of all key bindings

	static KeyMap *m_currentMouseMap;
	vector<KeyMap> m_mouseMaps;

	static multimap<int, VoidFunction> m_CommandFuncMap; //binds commands and action to void functions with no input
	static multimap<int, VoidFunction> m_HeldFuncMap; //binds commands and action to void functions with no input
	static multimap<int, VoidFunction> m_MouseCommandFuncMap;
	static vector<DoubleFunction> m_MouseFuncVector;
	static vector<DoubleFunction> m_ScrollFuncVector;
	static vector<VoidFunction> m_enterFunctions;
	static vector<VoidFunction> m_leaveFunctions;

	void AddNewCommand(Commands c, Action p, VoidFunction func); //Adds another value pair of command & action to void function with no input
	void AddNewMouseCommand(Commands c, Action p, VoidFunction func);

	static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods); //A static class that is called everytime pollEvents is called
	static void CursorPosCallback(GLFWwindow *, double, double);
	static void CursorEnterCallback(GLFWwindow *, int);
	static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
	static void ScrollCallBack(GLFWwindow*, double, double);
	static void JoystickConnectCallback(int, int);

	GLFWcursor* m_cursor;


	//Map holding whether commands are considered held down
	static map<Commands, bool> m_HeldMap;
	//Runs any functions that are flagged as being held down
	void RunHeldFunctions();

	

	//controllers

	static vector<GameController> m_controllers;

	static map<int, multimap<int, VoidFunction>> m_controllerButtonMap;
	static map<int, multimap<Axis, SingleFunction>> m_axisMap;


	static map<ControllerButton, bool> m_HeldControllerMap;

	static multimap<int, VoidFunction> m_disconnectFunctions;
	static multimap<int, VoidFunction> m_connectFunctions;

	static map<int, bool> m_controllerIsConnected;

	static int m_numberOfControllers;

	int m_funcNums = 0;
private:
};
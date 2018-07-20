#pragma once
#include <functional>
using namespace std;

typedef std::function<void(double x, double y)> CursorPositionUpdate;
typedef std::function<void(void)> CommandFunc;
typedef std::function<void(double d)> AxisPostionFunction;

//Author Hasan Al-Baghdadi

class Function {
public:
	Function(int &i) {
		m_id = i;
		i++;
	};
	~Function() {};

	int GetID() { return m_id; };
protected:
	int m_id;
};

class DoubleFunction :public Function {
public:
	DoubleFunction(int &i, CursorPositionUpdate f): Function(i), m_function(f) {

	}

	void Run(double x, double y) { m_function(x,y); } 

protected:
	CursorPositionUpdate m_function;
};

class VoidFunction :public Function {
public:
	VoidFunction(int &i, CommandFunc f) : Function(i), m_function(f) {

	}

	void Run() { m_function(); }

protected:
	CommandFunc m_function;
};

class SingleFunction :public Function {
public:
	SingleFunction(int &i, AxisPostionFunction f) : Function(i), m_function(f) {

	}

	void Run(double x) { m_function(x); }

protected:
	AxisPostionFunction m_function;
};
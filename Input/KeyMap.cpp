#include "KeyMap.h"

void KeyMap::AddNew(Commands c, int i) {
	m_keyMap.insert(std::pair<Commands,int>(c,i));
}

int KeyMap::getButton(Commands c) {
	map<Commands, int>::iterator it = m_keyMap.find(c);
	if (it != m_keyMap.end()) {
		return m_keyMap.find(c)->second;
	}
	else {
		return -1;
	}
}
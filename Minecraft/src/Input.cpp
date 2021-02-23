#include "Input.h"

INPUT_STATE Input::s_KeyState[1024];
double Input::s_ScrollX = 0.0;
double Input::s_ScrollY = 0.0;
float Input::s_CursorX = 400.0f;
float Input::s_CursorY = 300.0f;
float Input::s_Yaw = 0.0f;
float Input::s_Pitch = 0.0f;
float Input::s_PitchConstraint = 89.0f;
std::vector<std::uint16_t> Input::s_KeysPressed;

bool Input::GetKey(std::uint16_t key) {
	if (s_KeyState[key] == KEY_PRESS)
		return true;
	else return false;
}


bool Input::GetKeyUp(std::uint16_t key) {
	if (s_KeyState[key] == KEY_RELEASE) {
		s_KeysPressed.push_back(key);
		return true;
	}
	else return false;
}

bool Input::GetKeyDown(std::uint16_t key) {
	if (s_KeyState[key] == KEY_PRESS || s_KeyState[key] == KEY_REPEAT)
		return true;
	else return false;
}

bool Input::GetKeyRepeat(std::uint16_t key) {
	if (s_KeyState[key] == KEY_REPEAT)
		return true;
	else return false;
}

bool Input::GetMouse(std::uint16_t key) {
	if (s_KeyState[key] == KEY_PRESS)
		return true;
	else return false;
}

bool Input::GetMouseUp(std::uint16_t key) {
	if (s_KeyState[key] == KEY_RELEASE) {
		s_KeysPressed.push_back(key);
		return true;
	}
	else return false;
}

bool Input::GetMouseDown(std::uint16_t key) {
	if (s_KeyState[key] == KEY_PRESS || s_KeyState[key] == KEY_REPEAT)
		return true;
	else return false;
}

void Input::SetNone() {
	s_ScrollX = 0.0;
	s_ScrollY = 0.0;
	for (const auto& x : s_KeysPressed) {
		s_KeyState[x] = KEY_NONE;
	}
	s_KeysPressed.clear();
}
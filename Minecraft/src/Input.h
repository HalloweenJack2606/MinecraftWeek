#pragma once
#include <vector>
#include <cstdint>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define Key_Backspace GLFW_KEY_BACKSPACE
#define Key_Delete GLFW_KEY_DELETE
#define Key_Tab GLFW_KEY_TAB
#define Key_Return GLFW_KEY_ENTER
#define Key_Escape GLFW_KEY_ESCAPE
#define Key_Space GLFW_KEY_SPACE
#define Key_Keypad0 GLFW_KEY_KP_0
#define Key_Keypad1 GLFW_KEY_KP_1
#define Key_Keypad2 GLFW_KEY_KP_2
#define Key_Keypad3 GLFW_KEY_KP_3
#define Key_Keypad4 GLFW_KEY_KP_4
#define Key_Keypad5 GLFW_KEY_KP_5
#define Key_Keypad6 GLFW_KEY_KP_6
#define Key_Keypad7 GLFW_KEY_KP_7
#define Key_Keypad8 GLFW_KEY_KP_8
#define Key_Keypad9 GLFW_KEY_KP_9
#define Key_KeypadPeriod GLFW_KEY_KP_DECIMAL
#define Key_KeypadDivide GLFW_KEY_KP_DIVIDE
#define Key_KeypadMultiply GLFW_KEY_KP_MULTIPLY
#define Key_KeypadMinus GLFW_KEY_KP_SUBTRACT
#define Key_KeypadPlus GLFW_KEY_KP_ADD
#define Key_KeypadEnter GLFW_KEY_KP_ENTER
#define Key_KeypadEquals GLFW_KEY_KP_EQUAL
#define Key_UpArrow GLFW_KEY_UP
#define Key_DownArrow GLFW_KEY_DOWN
#define Key_RightArrow GLFW_KEY_RIGHT
#define Key_LeftArrow GLFW_KEY_LEFT
#define Key_Insert GLFW_KEY_INSERT
#define Key_Home GLFW_KEY_HOME
#define Key_End GLFW_KEY_END
#define Key_PageUp GLFW_KEY_PAGE_UP
#define Key_PageDown GLFW_KEY_PAGE_DOWN
#define Key_F1 GLFW_KEY_F1
#define Key_F2 GLFW_KEY_F2
#define Key_F3 GLFW_KEY_F3
#define Key_F4 GLFW_KEY_F4
#define Key_F5 GLFW_KEY_F5
#define Key_F6 GLFW_KEY_F6
#define Key_F7 GLFW_KEY_F7
#define Key_F8 GLFW_KEY_F8
#define Key_F9 GLFW_KEY_F9
#define Key_F10 GLFW_KEY_F10
#define Key_F11 GLFW_KEY_F11
#define Key_F12 GLFW_KEY_F12
#define Key_F13 GLFW_KEY_F13
#define Key_F14 GLFW_KEY_F14
#define Key_F15 GLFW_KEY_F15
#define Key_Alpha0 GLFW_KEY_0
#define Key_Alpha1 GLFW_KEY_1
#define Key_Alpha2 GLFW_KEY_2
#define Key_Alpha3 GLFW_KEY_3
#define Key_Alpha4 GLFW_KEY_4
#define Key_Alpha5 GLFW_KEY_5
#define Key_Alpha6 GLFW_KEY_6
#define Key_Alpha7 GLFW_KEY_7
#define Key_Alpha8 GLFW_KEY_8
#define Key_Alpha9 GLFW_KEY_9
#define Key_Comma GLFW_KEY_COMMA
#define Key_Period GLFW_KEY_PERIOD
#define Key_Slash GLFW_KEY_SLASH
#define Key_Semicolon GLFW_KEY_SEMICOLON
#define Key_A GLFW_KEY_A
#define Key_B GLFW_KEY_B
#define Key_C GLFW_KEY_C
#define Key_D GLFW_KEY_D
#define Key_E GLFW_KEY_E
#define Key_F GLFW_KEY_F
#define Key_G GLFW_KEY_G
#define Key_H GLFW_KEY_H
#define Key_I GLFW_KEY_I
#define Key_J GLFW_KEY_J
#define Key_K GLFW_KEY_K
#define Key_L GLFW_KEY_L
#define Key_M GLFW_KEY_M
#define Key_N GLFW_KEY_N
#define Key_O GLFW_KEY_O
#define Key_P GLFW_KEY_P
#define Key_Q GLFW_KEY_Q
#define Key_R GLFW_KEY_R
#define Key_S GLFW_KEY_S
#define Key_T GLFW_KEY_T
#define Key_U GLFW_KEY_U
#define Key_V GLFW_KEY_V
#define Key_W GLFW_KEY_W
#define Key_X GLFW_KEY_X
#define Key_Y GLFW_KEY_Y
#define Key_Z GLFW_KEY_Z
#define Key_Minus GLFW_KEY_MINUS
#define Key_KeyLast GLFW_KEY_LAST
#define Key_NumLock GLFW_KEY_NUM_LOCK
#define Key_CapsLock GLFW_KEY_CAPS_LOCK
#define Key_ScrollLock GLFW_KEY_SCROLL_LOCK
#define Key_RightShift GLFW_KEY_RIGHT_SHIFT
#define Key_LeftShift GLFW_KEY_LEFT_SHIFT
#define Key_RightControl GLFW_KEY_RIGHT_CONTROL
#define Key_LeftControl GLFW_KEY_LEFT_CONTROL
#define Key_RightAlt GLFW_KEY_RIGHT_ALT
#define Key_LeftAlt GLFW_KEY_LEFT_ALT
#define Key_Mouse0 GLFW_MOUSE_BUTTON_1
#define Key_Mouse1 GLFW_MOUSE_BUTTON_2
#define Key_Mouse2 GLFW_MOUSE_BUTTON_3
#define Key_Mouse3 GLFW_MOUSE_BUTTON_4
#define Key_Mouse4 GLFW_MOUSE_BUTTON_5
#define Key_Mouse5 GLFW_MOUSE_BUTTON_6
#define Key_Mouse6 GLFW_MOUSE_BUTTON_7
#define Key_MouseRight GLFW_MOUSE_BUTTON_RIGHT
#define Key_MouseLeft GLFW_MOUSE_BUTTON_LEFT
#define Key_MouseMiddle GLFW_MOUSE_BUTTON_MIDDLE
#define Key_MouseLast GLFW_MOUSE_BUTTON_LAST

enum INPUT_STATE {
	KEY_NONE,
	KEY_PRESS,
	KEY_REPEAT,
	KEY_RELEASE
};

class Input
{
public:
	static INPUT_STATE s_KeyState[1024];
	static double s_ScrollX;
	static double s_ScrollY;
	static float s_CursorX;
	static float s_CursorY;
	static float s_Yaw;
	static float s_Pitch;
	static float s_PitchConstraint;
private:
	static std::vector<std::uint16_t> s_KeysPressed;
public:
	static bool GetKey(std::uint16_t key);
	static bool GetKeyUp(std::uint16_t key);
	static bool GetKeyDown(std::uint16_t key);
	static bool GetKeyRepeat(std::uint16_t key);

	static bool GetMouse(std::uint16_t key);
	static bool GetMouseUp(std::uint16_t key);
	static bool GetMouseDown(std::uint16_t key);

	static void SetNone();
};


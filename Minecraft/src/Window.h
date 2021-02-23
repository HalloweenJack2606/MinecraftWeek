#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Input.h"

class Window
{
private:
	const int m_Width = 1280;
	const int m_Height = 720;
	GLFWwindow* m_Window = nullptr;
public:
	Window();

	void Clear();
	void Update();
	void Destroy();

	GLFWwindow* GetWindow() const { return m_Window; }
};

void window_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void window_mouseCallback(GLFWwindow* window, int button, int action, int mods);
void window_mousePosCallback(GLFWwindow* window, double xpos, double ypos);
void window_scrollCallback(GLFWwindow* window, double xoffset, double yoffset);


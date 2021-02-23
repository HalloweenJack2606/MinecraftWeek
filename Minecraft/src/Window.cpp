#define GLEW_STATIC
#include "Window.h"

Window::Window()
{
	if (!glfwInit()) std::cout << "Failed to initialize GLFW" << std::endl;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(m_Width, m_Height, "Minecraft", nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);

	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetKeyCallback(m_Window, window_keyCallback);
	glfwSetMouseButtonCallback(m_Window, window_mouseCallback);
	glfwSetCursorPosCallback(m_Window, window_mousePosCallback);
	glfwSetScrollCallback(m_Window, window_scrollCallback);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) std::cout << "Failed to initialize GLEW" << std::endl;
	glGetError();
	glViewport(0, 0, m_Width, m_Height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glfwSwapInterval(0);
}

void Window::Destroy()
{
	glfwTerminate();
}

void Window::Clear()
{
	glClearColor(0.53, 0.81f, 0.92f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Update()
{
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

void window_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key >= 0 && key < 1024) {
		switch (action) {
		case GLFW_PRESS:
			Input::s_KeyState[key] = KEY_PRESS;
			break;
		case GLFW_REPEAT:
			Input::s_KeyState[key] = KEY_REPEAT;
			break;
		case GLFW_RELEASE:
			Input::s_KeyState[key] = KEY_RELEASE;
			break;
		default:
			Input::s_KeyState[key] = KEY_NONE;
			break;
		}
	}
}

void window_mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button >= 0 && button < 12) {
		switch (action) {
		case GLFW_PRESS:
			Input::s_KeyState[button] = KEY_PRESS;
			break;
		case GLFW_REPEAT:
			Input::s_KeyState[button] = KEY_REPEAT;
			break;
		case GLFW_RELEASE:
			Input::s_KeyState[button] = KEY_RELEASE;
			break;
		default:
			Input::s_KeyState[button] = KEY_NONE;
			break;
		}
	}
}

bool first_mouse = true;

void window_mousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (first_mouse)
	{
		Input::s_CursorX = xpos;
		Input::s_CursorY = ypos;
		first_mouse = false;
	}
	float xOffset = xpos - Input::s_CursorX;
	float yOffset = Input::s_CursorY - ypos;
	Input::s_CursorX = xpos;
	Input::s_CursorY = ypos;

	const float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	Input::s_Yaw += xOffset;
	Input::s_Pitch += yOffset;

	//@note: pitch head constraint
	if (Input::s_Pitch >  Input::s_PitchConstraint) Input::s_Pitch =  Input::s_PitchConstraint;
	if (Input::s_Pitch < -Input::s_PitchConstraint) Input::s_Pitch = -Input::s_PitchConstraint;
}

void window_scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Input::s_ScrollX = xoffset;
	Input::s_ScrollY = yoffset;
}

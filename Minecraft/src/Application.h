#pragma once
#include "Window.h"
#include "Resources.h"
#include "Time.h"
#include "Camera.h"
#include "GUI.h"
#include <time.h>

class Time;
class Application
{
private:
	std::uint32_t m_InitialTime = 0;
	std::uint32_t m_FinalTime = 0;
	std::uint32_t m_FrameCount = 0;
	bool m_bWireframe = false;

	Camera* camera = nullptr;
	World* world = nullptr;
	GUI* gui = nullptr;

	Window m_Window = Window();
public:
	void Init();
	void Update();
	void Destroy();
};


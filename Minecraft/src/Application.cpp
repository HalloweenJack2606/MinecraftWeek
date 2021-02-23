#include "Application.h"

void Application::Init()
{
	m_InitialTime = static_cast<std::uint32_t>(time(NULL));
	world = new World();
	camera = new Camera(world);
	gui = new GUI(glm::vec2(0.0f));
	Resources::LoadTexture("stone.png", "default");
	Resources::LoadTexture("blocks.png", "block_atlas");
	Resources::LoadTexture("crosshair.png", "crosshair");
}

void Application::Destroy()
{
	m_Window.Destroy();
	delete camera;
	delete world;
	delete gui;
}

void Application::Update()
{
	float deltaTime = 0.0f;
	float lastTime = 0.0f;

	while (!glfwWindowShouldClose(m_Window.GetWindow()))
	{
		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		Time::SetTime(deltaTime);
		m_Window.Clear();
		world->Update(camera->position);
		world->Render();
		camera->Update();
		gui->Render();
		Input::SetNone();
		if (Input::GetKeyUp(Key_T)) m_bWireframe = !m_bWireframe;
		if (m_bWireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		m_Window.Update();

		m_FrameCount++;
		m_FinalTime = static_cast<std::uint32_t>(time(NULL));
		if (m_FinalTime - m_InitialTime > 0)
		{
			m_FrameCount /= (m_FinalTime - m_InitialTime);
			Time::frameCount = m_FrameCount;
			m_FrameCount = 0;
			m_InitialTime = m_FinalTime;
		}
	}
}

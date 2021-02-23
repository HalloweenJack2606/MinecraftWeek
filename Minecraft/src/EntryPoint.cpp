#include "Application.h"

int main()
{
	srand(time(NULL));
	Application* app = new Application();
	app->Init();
	app->Update();
	app->Destroy();
	delete app;
}
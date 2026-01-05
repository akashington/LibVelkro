#include <Velkro/Velkro.h>

#include <iostream>
#include <chrono>

namespace Project
{
	using namespace Velkro;

	Velkro::Engine* Engine;

	Entity* entity;
	WindowComponent* window;

	ExitCode Entry()
	{
		entity = new Entity();

		window = new WindowComponent("Velkro Engine", 800, 600);

		entity->AddComponent(window);
		entity->AddComponent(new SpriteComponent(window));

		Engine->AddEntity(entity);

		return ExitCode::Success;
	}

	ExitCode Loop()
	{
		if (window->GetWindowClosed())
		{
			return ExitCode::Exit;
		}

		return ExitCode::Success;
	}

	ExitCode Exit()
	{
		return ExitCode::Exit;
	}	
}

int main()
{
	Project::Engine = new Velkro::Engine();	

	Project::Engine->Run(Project::Entry, Project::Loop, Project::Exit);

	return 0;
}
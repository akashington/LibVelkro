#include <Velkro/Velkro.h>

#include <iostream>
#include <chrono>

namespace Project
{
	using namespace Velkro;

	Velkro::Engine* Engine;

	Entity* entity;
	WindowComponent* Window;
	CameraComponent3D* Camera;

	ExitCode Entry()
	{
		entity = new Entity();

		const int width = 800;
		const int height = 600;

		Window = new WindowComponent("Velkro Engine", width, height);

		Camera = new CameraComponent3D(80.0f, static_cast<float>(width) / height, 0.1f, 100.0f, CameraComponent3D::vec3(0.0f, 0.0f, 1.0f));

		entity->AddComponent(Window);
		entity->AddComponent(new SpriteComponent(Window, Camera, 64, 64, 0, 0));

		Engine->AddEntity(entity);

		return ExitCode::Success;
	}

	bool cameraMoving = false;
	int keyCode;

	void HandleCamera(CameraComponent3D* camera, float cameraSpeed, float smoothFactor, float deltaTimeInSeconds)
	{
		CameraComponent3D::vec3 targetPosition = camera->GetPosition();
		CameraComponent3D::vec3 cameraPosition = camera->GetPosition();

		switch (keyCode)
		{
			case KEY_W:
			{
				cameraPosition.z -= cameraSpeed * deltaTimeInSeconds;
				break;
			}
			case KEY_A:
			{
				cameraPosition.x -= cameraSpeed * deltaTimeInSeconds;
				break;
			}
			case KEY_S:
			{
				cameraPosition.z += cameraSpeed * deltaTimeInSeconds;
				break;
			}
			case KEY_D:
			{
				cameraPosition.x += cameraSpeed * deltaTimeInSeconds;
				break;
			}
			case KEY_SPACE:
			{
				cameraPosition.y -= cameraSpeed * deltaTimeInSeconds;
				break;
			}
			case KEY_LEFT_SHIFT:
			{
				cameraPosition.y += cameraSpeed * deltaTimeInSeconds;
				break;
			}
		}

		cameraPosition.x += (targetPosition.x - cameraPosition.x) * smoothFactor;
		cameraPosition.y += (targetPosition.y - cameraPosition.y) * smoothFactor;
		cameraPosition.z += (targetPosition.z - cameraPosition.z) * smoothFactor;

		camera->SetPosition(cameraPosition);
	}

	ExitCode Loop()
	{
		static std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();

		auto currentFrameTime = std::chrono::steady_clock::now();

		std::chrono::duration<float> deltaTime = currentFrameTime - lastFrameTime;

		lastFrameTime = currentFrameTime;

		float deltaTimeInSeconds = deltaTime.count();

		if (Window->GetWindowClosed())
		{
			return ExitCode::Exit;
		}
		
		if (cameraMoving)
		{
			HandleCamera(Camera, 1.0f, 0.1f, deltaTimeInSeconds);
		}

		return ExitCode::Success;
	}

	ExitCode Exit()
	{
		return ExitCode::Exit;
	}

	ExitCode Event(Velkro::Event* event, Velkro::WindowComponent* windowComponent)
	{
		if (Window->GetUUID() != windowComponent->GetUUID())
		{
			return ExitCode::Success;
		}

		if (WindowResizeEvent* resizeEvent = event->Get<WindowResizeEvent>())
		{
			Camera->SetProjection(80.0f, static_cast<float>(resizeEvent->GetWidth()) / resizeEvent->GetHeight(), 0.1f, 100.0f);
		}
		else if (KeyEvent* keyEvent = event->Get<KeyEvent>())
		{
			if (keyEvent->GetAction() == KEY_RELEASE)
			{
				cameraMoving = false;

				return ExitCode::Success;
			}
			
			cameraMoving = true;

			keyCode = keyEvent->GetCode();
		}

		return ExitCode::Success;
	}
}

int main()
{
	Project::Engine = new Velkro::Engine();	

	Project::Engine->Run(Project::Entry, Project::Loop, Project::Exit, Project::Event);

	return 0;
}
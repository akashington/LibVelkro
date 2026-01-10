#include <unordered_map>

#include <Velkro/Velkro.h>

#include "Window.h"
#include "Log.h"
#include "UUID.h"

namespace Velkro
{
	class Data
	{
	public:
		Data() = default;
		~Data() = default;

		std::unordered_map<std::string, Entity*>& GetEntities()
		{
			return m_Entities;
		}

		Entity* GetEntity(std::string UUID)
		{
			if (auto iterator = GetEntities().find(UUID); iterator != GetEntities().end())
			{
				return iterator->second;
			}

			VLK_CORE_ERROR("Requested entity of UUID \"{}\" does not exist. Returning nullptr.", UUID);

			return nullptr;
		}

	private:
		std::unordered_map<std::string, Entity*> m_Entities;
	};

	void Engine::Run(OnEnterFunction onEnterFunction, OnUpdateFunction onUpdateFunction, OnExitFunction onExitFunction, OnEventFunctionEngine onEventFunction)
	{
		m_Data = new Data();

		m_OnEventFunction = onEventFunction;

		Window::Initialize();

		Window::SetEventFunction(OnEvent);

		int entryExitCode = onEnterFunction();

		if (entryExitCode == Error)
		{
			VLK_CORE_FATAL("Error in entrypoint. Exiting program.");

			exit(entryExitCode);
		}
		else if (entryExitCode == Exit)
		{
			VLK_CORE_DEBUG("Exiting program on enter.");

			exit(0);
		}
		
		VLK_CORE_DEBUG("Entering program.");

		for (std::pair<std::string, Entity*> entity : m_Data->GetEntities())
		{
			entity.second->OnStart();
		}

		while (m_Running)
		{
			ExitCode updateExitCode = onUpdateFunction();

			if (updateExitCode == Error)
			{
				VLK_CORE_FATAL("Error in update loop. Exiting program.");

				break;
			}
			else if (updateExitCode == Exit)
			{
				VLK_CORE_DEBUG("Exiting program on update.");

				break;
			}

			for (std::pair<std::string, Entity*> entity : m_Data->GetEntities())
			{
				entity.second->OnUpdate();
			}

			Window::PollEvents();
		}

		ExitCode exitCode = onExitFunction();

		if (exitCode == Error)
		{
			VLK_CORE_FATAL("Error in exitpoint. Exiting program.");

			exit(exitCode);
		}
		else if (exitCode == Exit || exitCode == Success)
		{
			VLK_CORE_DEBUG("Exiting program.");
		}

		for (std::pair<std::string, Entity*> entity : m_Data->GetEntities())
		{
			entity.second->OnExit();

			delete entity.second;
		}

		m_Data->GetEntities().clear();

		delete m_Data;

		Window::Terminate();
	}

	void Engine::AddEntity(Entity* entity)
	{
		m_Data->GetEntities()[entity->GetUUID()] = entity;
	}

	void Engine::OnEvent(Event* event, const char* windowComponentUUID, const char* entityUUID)
	{
		WindowComponent* windowComponent = m_Data->GetEntity(entityUUID)->GetComponent<WindowComponent>(windowComponentUUID);

		ExitCode exitCode = m_OnEventFunction(event, windowComponent);

		if (exitCode == Error)
		{
			VLK_CORE_FATAL("Error in event. Exiting program.");

			delete event;

			exit(exitCode);
		}
		else if (exitCode == Exit)
		{
			VLK_CORE_DEBUG("Exiting program.");

			delete event;

			exit(0);
		}

		for (std::pair<std::string, Entity*> entity : m_Data->GetEntities())
		{
			entity.second->OnEvent(event, windowComponent);
		}

		delete event;
	}
}
#pragma once

#include "../../src/Entity.h" // TODO: Fix up the include system a bit and change this

namespace Velkro
{
	enum ExitCode
	{
		Success, Exit, Error
	};

	typedef ExitCode (*OnEnterFunction)();
	typedef ExitCode (*OnUpdateFunction)();
	typedef ExitCode (*OnExitFunction)();

	class Entity;
	class Event;
	class Data; // TODO: make this private

	class Engine
	{
	public:
		void Run(OnEnterFunction onEnterFunction, OnUpdateFunction onUpdateFunction, OnExitFunction onExitFunction);

		static void AddEntity(Entity* entity);

	private:
		static void OnEvent(Event* event, const char* windowComponentUUID, const char* entityUUID);

		bool m_Running = true;
		
		static inline Data* m_Data;
	};
}
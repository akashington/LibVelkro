#pragma once

#include "Component.h"

namespace Velkro
{
	class Entity
	{
	public:
		Entity(const char* ID = "");
		~Entity();

		const char* GetID();
		
		UUIDv4::UUID& GetUUID();

		template <typename Typename>
		Typename* GetComponent(const char* UUID)
		{
			if (Component* component = m_GetComponent(UUID))
			{
				return dynamic_cast<Typename*>(component);
			}			

			return nullptr;
		}

		void AddComponent(Component* component);

		void OnStart();
		void OnUpdate();
		void OnEvent(Event* event, WindowComponent* windowComponent);
		void OnExit();

	private:
		class Data;
		Data* m_Data;

		Component* m_GetComponent(const char* UUID); // Helper function for GetComponent, gets component without casting.
	};
}
#pragma once

namespace Velkro
{
	class Component;
	class WindowComponent;
	class Event;
	class UUID;

	class Entity
	{
	public:
		Entity(const char* ID = "");
		~Entity();

		const char* GetID();
		
		const char* GetUUID();

		template <typename Typename>
		Typename* GetComponent(const char* uuid)
		{
			if (Component* component = m_GetComponent(uuid))
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

		Component* m_GetComponent(const char* uuid); // Helper function for GetComponent, gets component without casting.
	};
}
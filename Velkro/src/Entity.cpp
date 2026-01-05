#include "Entity.h"
#include "Log.h"

#include <map>
#include <unordered_map>
#include <string>

namespace Velkro
{
	class Entity::Data
	{
	public:
		Data() = default;
		~Data() = default;

		std::map<std::string, Component*>& GetComponents()
		{
			return m_Components;
		}

		std::vector<Component*>& GetComponentsIndex()
		{
			return m_ComponentsIndex;
		}

		UUIDv4::UUID& GetUUID()
		{
			return m_UUID;
		}

		std::string& GetID()
		{
			return m_ID;
		}

	private:
		std::map<std::string /* UUID */, Component*> m_Components;
		std::vector<Component*> m_ComponentsIndex; // Sorted by insertion order

		UUIDv4::UUID m_UUID;
		std::string m_ID;

		static inline int currentIndex = 0;
	};

	Entity::Entity(const char* ID)
	{
		m_Data = new Data();

		m_Data->GetUUID() = UUID::GenerateUUID();
		m_Data->GetID() = ID;
	}

	Entity::~Entity()
	{
		delete m_Data;
	}

	const char* Entity::GetID()
	{
		return m_Data->GetID().c_str();
	}

	UUIDv4::UUID& Entity::GetUUID()
	{
		return m_Data->GetUUID();
	}

	Component* Entity::m_GetComponent(const char* UUID)
	{
		if (auto iterator = m_Data->GetComponents().find(UUID); iterator != m_Data->GetComponents().end())
		{
			return iterator->second;
		}

		VLK_CORE_ERROR("Requested component of UUID \"{}\" does not exist. Returning nullptr.", UUID);

		return nullptr;
	}

	void Entity::AddComponent(Component* component)
	{
		m_Data->GetComponentsIndex().push_back(component);
		m_Data->GetComponents()[component->GetUUID().str()] = component;
	}

	void Entity::OnStart()
	{
		for (Component* component : m_Data->GetComponentsIndex())
		{
			component->OnStart(GetUUID().str().c_str());
		}
	}
	void Entity::OnUpdate()
	{
		for (Component* component : m_Data->GetComponentsIndex())
		{
			component->OnUpdate();
		}
	}
	void Entity::OnExit()
	{
		for (Component* component : m_Data->GetComponentsIndex())
		{
			component->OnExit();

			delete component;
		}

		m_Data->GetComponentsIndex().clear();
		m_Data->GetComponents().clear();
	}
	void Entity::OnEvent(Event* event, WindowComponent* windowComponent)
	{
		for (Component* component : m_Data->GetComponentsIndex())
		{
			component->OnEvent(event, windowComponent);
		}
	}
}
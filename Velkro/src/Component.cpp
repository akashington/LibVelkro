#include "Component.h"

#include "Renderer.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"

#include "Event.h"

#include "Log.h"

namespace Velkro
{
	class Data
	{
	public:
		std::vector<const char*>& GetDependencies()
		{
			return m_Components;
		}

	private:
		std::vector<const char*> m_Components;
	};

	class Dependencies
	{
	public:
		std::vector<const char*>& GetDependencies()
		{
			return m_Components;
		}

	private:
		std::vector<const char*> m_Components;
	};

	WindowComponent::WindowComponent(const char* title, int width, int height)
		: m_UUID(UUID::GenerateUUID()), m_Title(title), m_Width(width), m_Height(height)
	{
	}

	void WindowComponent::OnStart(const char* parentUUID)
	{
		m_Window = new Window(m_UUID.str().c_str(), parentUUID, m_Title, m_Width, m_Height);

		Renderer::Initialize();
	}
	void WindowComponent::OnUpdate()
	{
		m_Window->Update();
	}
	void WindowComponent::OnExit()
	{
		delete m_Window;
	}

	void WindowComponent::GetWindowSize(int& width, int& height)
	{
		m_Window->GetSize(width, height);
	}

	bool WindowComponent::GetWindowClosed()
	{
		return static_cast<Window*>(m_Window)->WindowClosed();
	}

	UUIDv4::UUID& WindowComponent::GetUUID()
	{
		return m_UUID;
	}

	RenderComponent::RenderComponent(WindowComponent* windowComponent)
		: m_UUID(UUID::GenerateUUID()), m_WindowComponent(windowComponent)
	{
		if (!windowComponent)
		{
			VLK_CORE_ERROR("Window component required by render component is uninitialized, preventing initialization.");
		}
	}

	void RenderComponent::OnStart(const char* parentUUID)
	{
		if (!m_WindowComponent)
		{
			VLK_CORE_ERROR("Window component required by render component is uninitialized, preventing OnStart.");

			return;
		}

		m_WindowComponent->GetWindowSize(m_Width, m_Height);

		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(Index), m_Indices.data(), GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		uint32_t lastIndex = static_cast<uint32_t>(m_Vertices.size());

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(Index), m_Indices.data(), GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);

		m_ShaderProgram = Renderer::LoadShaderFromFile("assets/vertex.glsl", "assets/fragment.glsl");
	}

	void RenderComponent::AddData(std::vector<Vertex>& vertices, std::vector<Index>& indices)
	{
		m_Vertices.reserve(m_Vertices.size() + vertices.size());
		m_Indices.reserve(m_Indices.size() + indices.size());

		VLK_CORE_FATAL("Width: {} Height: {}", m_Width, m_Height);

		const float invHalfWidth = 2.0f / m_Width;
		const float invHalfHeight = 2.0f / m_Height;

		uint32_t offset = static_cast<uint32_t>(m_Vertices.size());

		for (Vertex& vertex : vertices)
		{
			VLK_CORE_FATAL("x: {}", vertex.x);
			VLK_CORE_FATAL("y: {}", vertex.y);
			VLK_CORE_FATAL("z: {}", vertex.z);

			vertex.x *= invHalfWidth;
			vertex.y *= invHalfHeight;
		}

		for (Index& index : indices)
		{
			index.a += offset;
			index.b += offset;
			index.c += offset;
		}

		m_Vertices.insert(m_Vertices.end(), vertices.begin(), vertices.end());
		m_Indices.insert(m_Indices.end(), indices.begin(), indices.end());

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(Index), m_Indices.data(), GL_DYNAMIC_DRAW);
	}

	void RenderComponent::OnUpdate()
	{
		if (!m_WindowComponent)
		{
			VLK_CORE_ERROR("Window component required by render component is uninitialized, preventing OnUpdate.");

			return;
		}

		glViewport(0, 0, m_Width, m_Height);

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(m_ShaderProgram);
		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_Indices.size()) * (sizeof(Index) / sizeof(uint32_t)), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}
	void RenderComponent::OnEvent(Event* event, WindowComponent* windowComponent)
	{
		if (!m_WindowComponent)
		{
			VLK_CORE_ERROR("Window component required by render component is uninitialized, preventing OnEvent.");

			return;
		}

		if (windowComponent->GetUUID() != m_WindowComponent->GetUUID())
		{
			return;
		}

		if (WindowResizeEvent* resizeEvent = event->Get<WindowResizeEvent>())
		{
			m_Width = resizeEvent->GetWidth();
			m_Height = resizeEvent->GetHeight();

			glViewport(0, 0, resizeEvent->GetWidth(), resizeEvent->GetHeight());
		}
	}
	void RenderComponent::OnExit()
	{
		if (!m_WindowComponent)
		{
			VLK_CORE_ERROR("Window component required by render component is uninitialized, preventing OnExit.");

			return;
		}

		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteProgram(m_ShaderProgram);
	}

	UUIDv4::UUID& RenderComponent::GetUUID()
	{
		return m_UUID;
	}

	SpriteComponent::SpriteComponent(WindowComponent* windowComponent)
		: m_UUID(UUID::GenerateUUID()), m_WindowComponent(windowComponent)
	{
		if (!m_WindowComponent)
		{
			VLK_CORE_ERROR("Window component required by sprite component is nullptr, preventing render component from initializing.");

			return;
		}
		
		m_RenderComponent = new RenderComponent(m_WindowComponent);
	}

	void SpriteComponent::OnStart(const char* parentUUID)
	{
		m_RenderComponent->OnStart(parentUUID);

		std::vector<RenderComponent::Vertex> vertices =
		{
			{ RenderComponent::Vertex( 32.0f,  32.0f, 0.0f) }, // top right
			{ RenderComponent::Vertex( 32.0f, -32.0f, 0.0f) }, // bottom right
			{ RenderComponent::Vertex(-32.0f, -32.0f, 0.0f) }, // bottom left
			{ RenderComponent::Vertex(-32.0f,  32.0f, 0.0f) }  // top left 			
		};

		std::vector<RenderComponent::Index> indices =
		{
			{ RenderComponent::Index(0, 1, 3) },
			{ RenderComponent::Index(1, 2, 3) }
		};

		m_RenderComponent->AddData(vertices, indices);
	}
	void SpriteComponent::OnUpdate()
	{
		m_RenderComponent->OnUpdate();
	}
	void SpriteComponent::OnEvent(Event* event, WindowComponent* windowComponent)
	{
		m_RenderComponent->OnEvent(event, windowComponent);
	}
	void SpriteComponent::OnExit()
	{
		m_RenderComponent->OnExit();
	}

	UUIDv4::UUID& SpriteComponent::GetUUID()
	{
		return m_UUID;
	}
}
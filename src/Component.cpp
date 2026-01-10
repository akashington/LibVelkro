#include "Component.h"

#include "Renderer.h"

#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "UUID.h"

#include "Window.h"

#include "Event.h"

#include "Log.h"

namespace Velkro
{
	class WindowComponent::Data
	{
	public:
		Data() = default;
		~Data() = default;

		std::string& GetUUID()
		{
			return m_UUID;
		}

	private:
		std::string m_UUID;
	};

	WindowComponent::WindowComponent(const char* title, int width, int height)
		: m_Title(title), m_Width(width), m_Height(height)
	{
		UUID uuid;
		uuid.GenerateUUID();

		m_Data = new Data();

		m_Data->GetUUID() = uuid.GetUUIDString();
	}

	void WindowComponent::OnStart(const char* entityUUID)
	{
		m_Window = new Window(m_Data->GetUUID().c_str(), entityUUID, m_Title, m_Width, m_Height);

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

	const char* WindowComponent::GetUUID()
	{
		return m_Data->GetUUID().c_str();
	}

	class CameraComponent3D::Data
	{
	public:
		Data() = default;
		~Data() = default;

		glm::vec3& GetPosition()
		{
			return m_Position;
		}		
		glm::vec3& GetRotation()
		{
			return m_Rotation;
		}

		glm::vec3& GetForward()
		{
			return m_Forward;
		}

		glm::vec3& GetUp()
		{
			return m_Up;
		}

		glm::vec3& GetRight()
		{
			return m_Right;
		}

		glm::mat4& GetProjectionMatrix()
		{
			return m_ProjectionMatrix;
		}
		glm::mat4& GetViewMatrix()
		{
			return m_ViewMatrix;
		}
		
		std::string GetUUID()
		{
			return m_UUID;
		}

	private:
		std::string m_UUID;

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Forward;
		glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_Right = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
	};

	CameraComponent3D::CameraComponent3D(float fov, float aspectRatio, float nearPlane, float farPlane, vec3 position, vec3 rotation)
	{
		UUID uuid;
		uuid.GenerateUUID();

		m_Data = new Data();

		m_Data->GetPosition() = glm::vec3(position.x, position.y, position.z);

		rotation.y = -90.0f;

		glm::vec3 forward;
		forward.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
		forward.y = sin(glm::radians(rotation.x));
		forward.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
		forward = glm::normalize(forward);

		m_Data->GetForward() = forward;

		m_Data->GetProjectionMatrix() = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);

		m_Data->GetViewMatrix() = glm::lookAt(m_Data->GetPosition(), m_Data->GetPosition() + m_Data->GetForward(), m_Data->GetUp());
	}

	void CameraComponent3D::SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		m_Data->GetProjectionMatrix() = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
	}

	void CameraComponent3D::SetPosition(vec3 position)
	{
		m_Data->GetPosition() = glm::vec3(position.x, position.y, position.z);

		m_Data->GetViewMatrix() = glm::lookAt(m_Data->GetPosition(), m_Data->GetPosition() + m_Data->GetForward(), m_Data->GetUp());
	}
	void CameraComponent3D::SetRotation(vec3 rotation)
	{
		rotation.x = glm::clamp(rotation.x, -89.0f, 89.0f);

		glm::vec3 forward;
		forward.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
		forward.y = sin(glm::radians(rotation.x));
		forward.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
		forward = glm::normalize(forward);

		m_Data->GetForward() = forward;
		m_Data->GetRight() = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
		m_Data->GetUp() = glm::normalize(glm::cross(m_Data->GetRight(), forward));

		m_Data->GetViewMatrix() = glm::lookAt(m_Data->GetPosition(), m_Data->GetPosition() + forward, m_Data->GetUp());
	}

	float* CameraComponent3D::GetProjectionMatrix()
	{
		return glm::value_ptr(m_Data->GetProjectionMatrix());
	}
	float* CameraComponent3D::GetViewMatrix()
	{
		return glm::value_ptr(m_Data->GetViewMatrix());
	}

	CameraComponent3D::vec3 CameraComponent3D::GetPosition()
	{
		glm::vec3& position = m_Data->GetPosition();

		return vec3(position.x, position.y, position.z);
	}

	const char* CameraComponent3D::GetUUID()
	{
		return m_Data->GetUUID().c_str();
	}

	void CameraComponent3D::OnStart(const char* entityUUID)
	{
		return;
	}
	void CameraComponent3D::OnUpdate()
	{
		return;
	}
	void CameraComponent3D::OnEvent(Event* event, WindowComponent* windowComponent)
	{
		return;
	}
	void CameraComponent3D::OnExit()
	{
		delete m_Data;
	}

	class RenderComponent::Data
	{
	public:
		std::vector<Vertex>& GetVertices()
		{
			return m_Vertices;
		}
		std::vector<Index>& GetIndices()
		{
			return m_Indices;
		}
		std::string& GetUUID()
		{
			return m_UUID;
		}

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<Index> m_Indices;
		std::string m_UUID;
	};

	RenderComponent::RenderComponent(WindowComponent* windowComponent, CameraComponent3D* cameraComponent)
		: m_WindowComponent(windowComponent), m_CameraComponent(cameraComponent)
	{
		UUID uuid;
		uuid.GenerateUUID();

		m_Data = new Data();
		m_Data->GetUUID() = uuid.GetUUIDString();
	}

	void RenderComponent::AddData(Vertex* vertices, size_t verticesCount, Index* indices, size_t indicesCount)
	{
		m_Data->GetVertices().reserve(m_Data->GetVertices().size() + verticesCount);
		m_Data->GetIndices().reserve(m_Data->GetIndices().size() + indicesCount);

		uint32_t offset = static_cast<uint32_t>(m_Data->GetVertices().size());

		for (int i = 0; i < indicesCount; i++)
		{
			indices[i].a += offset;
			indices[i].b += offset;
			indices[i].c += offset;
		}

		m_Data->GetVertices().insert(m_Data->GetVertices().end(), vertices, vertices + verticesCount);
		m_Data->GetIndices().insert(m_Data->GetIndices().end(), indices, indices + indicesCount);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Data->GetVertices().size() * sizeof(Vertex), m_Data->GetVertices().data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Data->GetIndices().size() * sizeof(Index), m_Data->GetIndices().data(), GL_DYNAMIC_DRAW);
	}

	void RenderComponent::SetCamera(CameraComponent3D* cameraComponent)
	{
		m_CameraComponent = cameraComponent;
	}

	uint32_t texture;

	void RenderComponent::OnStart(const char* entityUUID)
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
		glBufferData(GL_ARRAY_BUFFER, m_Data->GetVertices().size() * sizeof(Vertex), m_Data->GetVertices().data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Data->GetIndices().size() * sizeof(Index), m_Data->GetIndices().data(), GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		m_ShaderProgram = Renderer::LoadShaderFromFile("assets/vertex.glsl", "assets/fragment.glsl");

		glUseProgram(m_ShaderProgram);

		glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, "u_Projection"), 1, GL_FALSE, m_CameraComponent->GetProjectionMatrix());
		glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, "u_View"), 1, GL_FALSE, m_CameraComponent->GetViewMatrix());

		glUseProgram(0);

		texture = Renderer::LoadTexture("assets/sprite.png");
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
		
		glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, "u_Projection"), 1, GL_FALSE, m_CameraComponent->GetProjectionMatrix());
		glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, "u_View"), 1, GL_FALSE, m_CameraComponent->GetViewMatrix());

		glBindTexture(GL_TEXTURE_2D, texture);

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_Data->GetIndices().size()) * (sizeof(Index) / sizeof(uint32_t)), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}
	void RenderComponent::OnEvent(Event* event, WindowComponent* windowComponent)
	{
		return;
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

		delete m_Data;
	}

	const char* RenderComponent::GetUUID()
	{
		return m_Data->GetUUID().c_str();
	}

	class SpriteComponent::Data
	{
	public:
		Data() = default;
		~Data() = default;

		std::string& GetUUID()
		{
			return m_UUID;
		}

	private:
		std::string m_UUID;
	};

	SpriteComponent::SpriteComponent(WindowComponent* windowComponent, CameraComponent3D* cameraComponent, float width, float height, float x, float y)
		: m_WindowComponent(windowComponent), m_CameraComponent(cameraComponent), m_Width(width), m_Height(height), m_X(x), m_Y(y)
	{
		if (!m_WindowComponent)
		{
			VLK_CORE_ERROR("Window component required by sprite component is nullptr, preventing render component from initializing.");

			return;
		}
		
		UUID uuid;
		uuid.GenerateUUID();

		m_Data = new Data();

		m_Data->GetUUID() = uuid.GetUUIDString();

		m_RenderComponent = new RenderComponent(m_WindowComponent, m_CameraComponent);
	}

	void SpriteComponent::OnStart(const char* entityUUID)
	{
		m_RenderComponent->OnStart(entityUUID);

		std::vector<RenderComponent::Vertex> vertices =
		{
			{ RenderComponent::Vertex( 1.0f,  1.0f, 0.0f, 1.0f, 1.0f) }, // top right
			{ RenderComponent::Vertex( 1.0f, -1.0f, 0.0f, 1.0f, 0.0f) }, // bottom right
			{ RenderComponent::Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f) }, // bottom left
			{ RenderComponent::Vertex(-1.0f,  1.0f, 0.0f, 0.0f, 1.0f) }  // top left
		};

		std::vector<RenderComponent::Index> indices =
		{
			{ RenderComponent::Index(0, 1, 3) },
			{ RenderComponent::Index(1, 2, 3) }
		};

		m_RenderComponent->AddData(vertices.data(), vertices.size(), indices.data(), indices.size());
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

		delete m_Data;
	}

	const char* SpriteComponent::GetUUID()
	{
		return m_Data->GetUUID().c_str();
	}
}
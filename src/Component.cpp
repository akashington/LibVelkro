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

	WindowComponent::WindowComponent(const char* entityUUID, const char* title, int width, int height)
	{
		UUID uuid;

		uuid.GenerateUUID();

		m_Data = new Data();

		m_Data->GetUUID() = uuid.GetUUIDString();

		m_Window = new Window(m_Data->GetUUID().c_str(), entityUUID, title, width, height);

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

	class ShaderComponent::Data
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

	ShaderComponent::ShaderComponent(const char* vertexShaderPath, const char* fragmentShaderPath)
		: m_VertexShaderPath(vertexShaderPath), m_FragmentShaderPath(fragmentShaderPath)
	{
		m_Data = new Data();

		UUID uuid;

		uuid.GenerateUUID();

		m_Data->GetUUID() = uuid.GetUUIDString();

		m_ID = Renderer::LoadShaderFromFile(m_VertexShaderPath, m_FragmentShaderPath);
	}

	void ShaderComponent::SetUniformMat4(const char* id, float* mat4)
	{
		glUseProgram(m_ID);

		glUniformMatrix4fv(glGetUniformLocation(m_ID, id), 1, GL_FALSE, mat4);

		glUseProgram(0);
	}

	void ShaderComponent::SetUniformVec3(const char* id, vec3 vec3)
	{
		glUseProgram(m_ID);

		glUniform3f(glGetUniformLocation(m_ID, id), vec3.x, vec3.y, vec3.z);

		glUseProgram(0);
	}

	void ShaderComponent::Bind()
	{
		glUseProgram(m_ID);
	}

	const char* ShaderComponent::GetUUID()
	{
		return m_Data->GetUUID().c_str();
	}

	void ShaderComponent::OnUpdate()
	{
	}
	void ShaderComponent::OnEvent(Event* event, WindowComponent* windowComponent)
	{
	}
	void ShaderComponent::OnExit()
	{
		glDeleteProgram(m_ID);
	}

	class Texture2DComponent::Data
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
	
	Texture2DComponent::Texture2DComponent(const char* texturePath, bool linear)
	{
		m_ID = Renderer::LoadTexture2D(texturePath, m_Width, m_Height, m_Channels, linear);
	}

	void Texture2DComponent::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	int Texture2DComponent::GetWidth()
	{
		return m_Width;
	}
	int Texture2DComponent::GetHeight()
	{
		return m_Height;
	}
	int Texture2DComponent::GetChannels()
	{
		return m_Channels;
	}

	const char* Texture2DComponent::GetUUID()
	{
		return m_Data->GetUUID().c_str();
	}

	void Texture2DComponent::OnUpdate()
	{
	}
	void Texture2DComponent::OnEvent(Event* event, WindowComponent* windowComponent)
	{
	}
	void Texture2DComponent::OnExit()
	{
	}

	class TextureAtlasComponent::Data
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

	TextureAtlasComponent::TextureAtlasComponent(const char* textureAtlasPath, bool linear, int textureWidth, int textureHeight)
		: m_TextureWidth(textureWidth), m_TextureHeight(textureHeight)
	{
		m_Data = new Data();

		m_Atlas = new Texture2DComponent(textureAtlasPath, linear);

		UUID uuid;

		uuid.GenerateUUID();

		m_Data->GetUUID() = uuid.GetUUIDString();
	}

	void TextureAtlasComponent::Bind()
	{
		m_Atlas->Bind();
	}

	void TextureAtlasComponent::GetUV(int textureID, float* UV)
	{
		int atlasWidth = m_Atlas->GetWidth();
		int atlasHeight = m_Atlas->GetHeight();

		int tilesPerRow = atlasWidth / m_TextureWidth;
		int tilesPerColumn = atlasHeight / m_TextureHeight;

		float tileWidth = static_cast<float>(m_TextureWidth) / atlasWidth;
		float tileHeight = static_cast<float>(m_TextureHeight) / atlasHeight;

		int column = textureID % tilesPerRow;
		int row = textureID / tilesPerRow;

		float uMin = column * tileWidth;
		float uMax = uMin + tileWidth;
		float vMax = 1.0f - row * tileHeight;
		float vMin = vMax - tileHeight;

		UV[0] = uMax; UV[1] = vMax;
		UV[2] = uMax; UV[3] = vMin;
		UV[4] = uMin; UV[5] = vMin;
		UV[6] = uMin; UV[7] = vMax;
	}

	Texture2DComponent* TextureAtlasComponent::GetTexture()
	{
		return m_Atlas;
	}

	const char* TextureAtlasComponent::GetUUID()
	{
		return m_Data->GetUUID().c_str();
	}

	void TextureAtlasComponent::OnUpdate()
	{
	}

	void TextureAtlasComponent::OnEvent(Event* event, WindowComponent* windowComponent)
	{
	}

	void TextureAtlasComponent::OnExit()
	{
		m_Atlas->OnExit();
	}

	class Camera3DComponent::Data
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

	Camera3DComponent::Camera3DComponent(float fov, float aspectRatio, float nearPlane, float farPlane, vec3 position, vec3 rotation)
	{
		m_Data = new Data();

		UUID uuid;

		uuid.GenerateUUID();		

		m_Data->GetUUID() = uuid.GetUUIDString();
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

	void Camera3DComponent::SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		m_Data->GetProjectionMatrix() = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
	}

	void Camera3DComponent::SetPosition(vec3 position)
	{
		m_Data->GetPosition() = glm::vec3(position.x, position.y, position.z);

		m_Data->GetViewMatrix() = glm::lookAt(m_Data->GetPosition(), m_Data->GetPosition() + m_Data->GetForward(), m_Data->GetUp());
	}
	void Camera3DComponent::SetRotation(vec3 rotation)
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

	float* Camera3DComponent::GetProjectionMatrix()
	{
		return glm::value_ptr(m_Data->GetProjectionMatrix());
	}
	float* Camera3DComponent::GetViewMatrix()
	{
		return glm::value_ptr(m_Data->GetViewMatrix());
	}

	vec3 Camera3DComponent::GetPosition()
	{
		glm::vec3& position = m_Data->GetPosition();

		return vec3(position.x, position.y, position.z);
	}

	const char* Camera3DComponent::GetUUID()
	{
		return m_Data->GetUUID().c_str();
	}

	void Camera3DComponent::OnUpdate()
	{
	}
	void Camera3DComponent::OnEvent(Event* event, WindowComponent* windowComponent)
	{
	}
	void Camera3DComponent::OnExit()
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

	uint32_t texture;

	RenderComponent::RenderComponent(WindowComponent* windowComponent, ShaderComponent* shaderComponent, Texture2DComponent* texture)
		: m_WindowComponent(windowComponent), m_ShaderComponent(shaderComponent), m_TextureComponent(texture)
	{
		m_Data = new Data();

		UUID uuid;

		uuid.GenerateUUID();

		m_Data->GetUUID() = uuid.GetUUIDString();

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

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		m_ShaderComponent->Bind();
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

	void RenderComponent::OnUpdate()
	{
		glViewport(0, 0, m_Width, m_Height);

		glClear(GL_COLOR_BUFFER_BIT);

		m_ShaderComponent->Bind();

		m_TextureComponent->Bind();

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_Data->GetIndices().size()) * (sizeof(Index) / sizeof(uint32_t)), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	}
	void RenderComponent::OnEvent(Event* event, WindowComponent* windowComponent)
	{
		return;
	}
	void RenderComponent::OnExit()
	{
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
		glDeleteVertexArrays(1, &m_VAO);

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

	SpriteComponent::SpriteComponent(WindowComponent* windowComponent, ShaderComponent* shaderComponent, Texture2DComponent* textureComponent, vec3 colour, float width, float height, float x, float y)
		: m_Width(width), m_Height(height), m_X(x), m_Y(y)
	{
		m_Data = new Data();

		UUID uuid;

		uuid.GenerateUUID();

		m_Data->GetUUID() = uuid.GetUUIDString();

		m_RenderComponent = new RenderComponent(windowComponent, shaderComponent, textureComponent);

		std::vector<RenderComponent::Vertex> vertices =
		{
			{ RenderComponent::Vertex( 1.0f,  1.0f, 0.0f, colour.x, colour.y, colour.z, 1.0f, 1.0f) }, // top right
			{ RenderComponent::Vertex( 1.0f, -1.0f, 0.0f, colour.x, colour.y, colour.z, 1.0f, 0.0f) }, // bottom right
			{ RenderComponent::Vertex(-1.0f, -1.0f, 0.0f, colour.x, colour.y, colour.z, 0.0f, 0.0f) }, // bottom left
			{ RenderComponent::Vertex(-1.0f,  1.0f, 0.0f, colour.x, colour.y, colour.z, 0.0f, 1.0f) }  // top left
		};

		std::vector<RenderComponent::Index> indices =
		{
			{ RenderComponent::Index(0, 1, 3) },
			{ RenderComponent::Index(1, 2, 3) }
		};

		m_RenderComponent->AddData(vertices.data(), vertices.size(), indices.data(), indices.size());
	}

	SpriteComponent::SpriteComponent(WindowComponent* windowComponent, ShaderComponent* shaderComponent, TextureAtlasComponent* textureAtlasComponent, int textureID, vec3 colour, float width, float height, float x, float y)
		: m_Width(width), m_Height(height), m_X(x), m_Y(y)
	{
		m_Data = new Data();

		UUID uuid;

		uuid.GenerateUUID();		

		m_Data->GetUUID() = uuid.GetUUIDString();

		m_RenderComponent = new RenderComponent(windowComponent, shaderComponent, textureAtlasComponent->GetTexture());

		float UV[8] =
		{
			0.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 0.0f
		};
		
		textureAtlasComponent->GetUV(textureID, UV);

		std::vector<RenderComponent::Vertex> vertices =
		{
			{ RenderComponent::Vertex( 1.0f,  1.0f, 0.0f, colour.x, colour.y, colour.z, UV[0], UV[1]) }, // top right
			{ RenderComponent::Vertex( 1.0f, -1.0f, 0.0f, colour.x, colour.y, colour.z, UV[2], UV[3]) }, // bottom right
			{ RenderComponent::Vertex(-1.0f, -1.0f, 0.0f, colour.x, colour.y, colour.z, UV[4], UV[5]) }, // bottom left
			{ RenderComponent::Vertex(-1.0f,  1.0f, 0.0f, colour.x, colour.y, colour.z, UV[6], UV[7]) }  // top left
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
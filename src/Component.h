#pragma once

//TODO: Potentially not include this?
#include "Types.h"

namespace Velkro
{
	class Window;
	class Event;
	class WindowComponent;
	class UUID;

	enum ComponentType
	{
		WindowType, RenderType, SpriteType
	};
	
	class Component
	{
	public:
		virtual void OnUpdate() = 0;
		virtual void OnEvent(Event* event, WindowComponent* windowComponent) {};
		virtual void OnExit() = 0;

		virtual const char* GetUUID() = 0;

		template<typename Typename>
		Typename* GetType()
		{
			return dynamic_cast<Typename*>(this);
		}
	};
	
	class WindowComponent : public Component
	{
	public:
		WindowComponent(const char* entityUUID, const char* title, int width, int height);

		void OnUpdate() override;
		void OnExit() override;

		void GetWindowSize(int& width, int& height);

		bool GetWindowClosed();

		const char* GetUUID() override;

	private:
		Window* m_Window;

		class Data;
		Data* m_Data;
	};

	struct vec3
	{
		float x, y, z;
	};

	class ShaderComponent : public Component
	{
	public:
		ShaderComponent(const char* vertexShaderPath, const char* fragmentShaderPath);

		void SetUniformMat4(const char* id, float* mat4);
		void SetUniformVec3(const char* id, vec3 vec3);

		void Bind();

		const char* GetUUID() override;

		void OnUpdate() override;
		void OnEvent(Event* event, WindowComponent* windowComponent) override;
		void OnExit() override;

	private:
		class Data;
		Data* m_Data;

		uint32_t m_ID;

		const char* m_VertexShaderPath;
		const char* m_FragmentShaderPath;
	};

	class Texture2DComponent : public Component
	{
	public:
		Texture2DComponent(const char* texturePath, bool linear);

		void Bind();

		int GetWidth();
		int GetHeight();
		int GetChannels();

		const char* GetUUID() override;

		void OnUpdate() override;
		void OnEvent(Event* event, WindowComponent* windowComponent) override;
		void OnExit() override;

	private:
		uint32_t m_ID;

		int m_Width;
		int m_Height;
		int m_Channels;

		class Data;
		Data* m_Data;
	};

	class TextureAtlasComponent : public Component
	{
	public:
		TextureAtlasComponent(const char* textureAtlasPath, bool linear, int texureWidth, int textureHeight);

		void Bind();

		void GetUV(int textureID, float* UV);

		Texture2DComponent* GetTexture();

		const char* GetUUID() override;

		void OnUpdate() override;
		void OnEvent(Event* event, WindowComponent* windowComponent) override;
		void OnExit() override;

	private:
		uint32_t m_ID;

		class Data;
		Data* m_Data;

		Texture2DComponent* m_Atlas;

		int m_TextureWidth;
		int m_TextureHeight;
	};

	class Camera3DComponent : public Component
	{
	public:
		Camera3DComponent(float fov, float aspectRatio, float nearPlane, float farPlane, vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 rotation = vec3(0.0f, 0.0f, 0.0f));

		void SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane);

		void SetPosition(vec3 position);
		void SetRotation(vec3 rotation);

		float* GetProjectionMatrix();
		float* GetViewMatrix();

		vec3 GetPosition();

		const char* GetUUID() override;

		void OnUpdate() override;
		void OnEvent(Event* event, WindowComponent* windowComponent) override;
		void OnExit() override;

	private:
		class Data;
		Data* m_Data;
	};

	class RenderComponent : public Component
	{
	public:
		struct Vertex
		{
			float x, y, z;
			float r, g, b;
			float uvX, uvY;
		};

		struct Index
		{
			uint32_t a, b, c;
		};

		RenderComponent(WindowComponent* windowComponent, ShaderComponent* shaderComponent, Texture2DComponent* texture);

		void AddData(Vertex* vertices, size_t verticesCount, Index* indices, size_t indicesCount);
		
		void OnUpdate() override;
		void OnEvent(Event* event, WindowComponent* windowComponent) override;
		void OnExit() override;		

		const char* GetUUID() override;

	private:
		WindowComponent* m_WindowComponent;
		ShaderComponent* m_ShaderComponent;
		Texture2DComponent* m_TextureComponent;
		TextureAtlasComponent* m_TextureAtlasComponent;

		bool m_UsingAtlas = false;

		int m_Width, m_Height;

		static inline uint32_t m_EBO = 0;
		static inline uint32_t m_VBO = 0;
		static inline uint32_t m_VAO = 0;

		static inline uint32_t m_InstanceVBO = 0;
		static inline uint32_t m_InstanceCount = 0;

		class Data;
		Data* m_Data;
	};

	class SpriteComponent : public Component
	{
	public:
		SpriteComponent(WindowComponent* windowComponent, ShaderComponent* shaderComponent, Texture2DComponent* textureComponent, vec3 colour, float width, float height, float x, float y);
		SpriteComponent(WindowComponent* windowComponent, ShaderComponent* shaderComponent, TextureAtlasComponent* textureAtlasComponent, int textureID, vec3 colour, float width, float height, float x, float y);

		void OnUpdate() override;
		void OnEvent(Event* event, WindowComponent* windowComponent) override;
		void OnExit() override;

		const char* GetUUID() override;

	private:
		RenderComponent* m_RenderComponent;

		bool m_UsingAtlas = false;

		float m_Width, m_Height;
		float m_X, m_Y;
				
		class Data;
		Data* m_Data;
	};
}
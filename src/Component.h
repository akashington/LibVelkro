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
		virtual void OnStart(const char* entityUUID) = 0;
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
		WindowComponent(const char* title, int width, int height);

		void OnStart(const char* entityUUID) override;
		void OnUpdate() override;
		void OnExit() override;

		void GetWindowSize(int& width, int& height);

		bool GetWindowClosed();

		const char* GetUUID();

	private:
		const char* m_Title;
		int m_Width, m_Height;

		Window* m_Window;

		class Data;
		Data* m_Data;
	};

	class CameraComponent3D : public Component
	{
	public:
		struct vec3
		{
			float x, y, z;
		};

		CameraComponent3D(float fov, float aspectRatio, float nearPlane, float farPlane, vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 rotation = vec3(0.0f, 0.0f, 0.0f));

		void SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane);

		void SetPosition(vec3 position);
		void SetRotation(vec3 rotation);

		float* GetProjectionMatrix();
		float* GetViewMatrix();

		vec3 GetPosition();

		const char* GetUUID();

		void OnStart(const char* entityUUID) override;
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
			float uvX, uvY;
		};

		struct Index
		{
			uint32_t a, b, c;
		};

		RenderComponent(WindowComponent* windowComponent, CameraComponent3D* cameraComponent);

		void AddData(Vertex* vertices, size_t verticesCount, Index* indices, size_t indicesCount);
		
		void SetCamera(CameraComponent3D* cameraComponent);

		void OnStart(const char* entityUUID) override;
		void OnUpdate() override;
		void OnEvent(Event* event, WindowComponent* windowComponent) override;
		void OnExit() override;		

		const char* GetUUID();

	private:
		WindowComponent* m_WindowComponent;
		CameraComponent3D* m_CameraComponent;

		int m_Width, m_Height;

		static inline uint32_t m_EBO = 0;
		static inline uint32_t m_VBO = 0;
		static inline uint32_t m_VAO = 0;

		static inline uint32_t m_InstanceVBO = 0;
		static inline uint32_t m_InstanceCount = 0;

		static inline uint32_t m_ShaderProgram;

		class Data;
		Data* m_Data;
	};

	class SpriteComponent : public Component
	{
	public:
		SpriteComponent(WindowComponent* windowComponent, CameraComponent3D* cameraComponent, float width, float height, float x, float y);

		void OnStart(const char* entityUUID) override;
		void OnUpdate() override;
		void OnEvent(Event* event, WindowComponent* windowComponent) override;
		void OnExit() override;

		const char* GetUUID() override;

	private:
		RenderComponent* m_RenderComponent;
		WindowComponent* m_WindowComponent;
		CameraComponent3D* m_CameraComponent;

		float m_Width, m_Height;
		float m_X, m_Y;

		struct Sprite
		{
			float x, y;
			float width, height;
			float rotation;
		};
		
		class Data;
		Data* m_Data;
	};
}
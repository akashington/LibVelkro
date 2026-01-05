#pragma once

#include "UUID.h"

namespace Velkro
{
	class Window;
	class Event;
	class WindowComponent; // Fixes Component class's circular dependency.
	class Data;

	enum ComponentType
	{
		WindowType, RenderType, SpriteType
	};

	class Component
	{
	public:
		virtual void OnStart(const char* parentUUID) = 0;
		virtual void OnUpdate() = 0;
		virtual void OnEvent(Event* event, WindowComponent* windowComponent)
		{
		};
		virtual void OnExit() = 0;

		virtual UUIDv4::UUID& GetUUID() = 0;

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

		void OnStart(const char* parentUUID) override;
		void OnUpdate() override;
		void OnExit() override;

		void GetWindowSize(int& width, int& height);

		bool GetWindowClosed();

		UUIDv4::UUID& GetUUID();

	private:
		const char* m_Title;
		int m_Width, m_Height;

		UUIDv4::UUID m_UUID;

		Window* m_Window;
	};

	class RenderComponent : public Component
	{
	public:
		struct Vertex
		{
			float x, y, z;
		};

		struct Index
		{
			uint32_t a, b, c;
		};

		RenderComponent(WindowComponent* windowComponent);

		void OnStart(const char* parentUUID) override;
		
		void AddData(std::vector<Vertex>& vertices, std::vector<Index>& indices);

		void OnUpdate() override;
		void OnEvent(Event* event, WindowComponent* windowComponent) override;
		void OnExit() override;		

		UUIDv4::UUID& GetUUID();

	private:
		WindowComponent* m_WindowComponent;

		UUIDv4::UUID m_UUID;

		int m_Width, m_Height;

		uint32_t m_EBO = 0;
		uint32_t m_VBO = 0;
		uint32_t m_VAO = 0;

		uint32_t m_ShaderProgram;

		std::vector<Vertex> m_Vertices;
		std::vector<Index> m_Indices;
	};

	class SpriteComponent : public Component
	{
	public:
		SpriteComponent(WindowComponent* windowComponent);

		void OnStart(const char* parentUUID) override;
		void OnUpdate() override;
		void OnEvent(Event* event, WindowComponent* windowComponent) override;
		void OnExit() override;

		UUIDv4::UUID& GetUUID() override;

	private:
		RenderComponent* m_RenderComponent;
		WindowComponent* m_WindowComponent;

		UUIDv4::UUID m_UUID;
	};
}
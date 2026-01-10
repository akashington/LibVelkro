#pragma once

struct GLFWwindow;

namespace Velkro
{
	class Event;

	typedef void(*OnEventFunction)(Event* event, const char* UUID, const char* ParentUUID);

	class Window
	{
	public:
		static void Initialize();
		static void Terminate();

		Window() = default;
		Window(const char* UUID, const char* EntityUUID, const char* title, int width, int height);
		~Window();

		void Update();

		const char* GetTitle();
		void GetSize(int& width, int& height);
		void GetPos(int& x, int& y);

		bool WindowClosed();

		static void SetEventFunction(OnEventFunction onEventFunction);

		static void PollEvents();

	private:
		GLFWwindow* m_Window;
	};
}
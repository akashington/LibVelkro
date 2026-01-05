#include "Window.h"

#include <GLFW/glfw3.h>

#include "Log.h"
#include "Event.h"

#include <unordered_map>

namespace Velkro
{
	static OnEventFunction OnEvent;

	static std::unordered_map<GLFWwindow*, std::pair<std::string /* UUID */, std::string /* Parent UUID */>> GLFWWindowMap;

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		KeyEvent* keyEvent = new KeyEvent(key, scancode, action, mods);

		OnEvent(keyEvent, GLFWWindowMap[window].first.c_str(), GLFWWindowMap[window].second.c_str());
	}
	static void CharCallback(GLFWwindow* window, unsigned int codepoint)
	{
		CharacterEvent* charEvent = new CharacterEvent(codepoint);

		OnEvent(charEvent, GLFWWindowMap[window].first.c_str(), GLFWWindowMap[window].second.c_str());
	}
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		MouseButtonEvent* mouseButtonEvent = new MouseButtonEvent(button, action, mods);

		OnEvent(mouseButtonEvent, GLFWWindowMap[window].first.c_str(), GLFWWindowMap[window].second.c_str());
	}
	static void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		MouseScrollEvent* mouseScrollEvent = new MouseScrollEvent(xOffset, yOffset);

		OnEvent(mouseScrollEvent, GLFWWindowMap[window].first.c_str(), GLFWWindowMap[window].second.c_str());
	}
	static void MouseMoveCallback(GLFWwindow* window, double xPos, double yPos)
	{
		MouseMoveEvent* mouseMoveEvent = new MouseMoveEvent(xPos, yPos);

		OnEvent(mouseMoveEvent, GLFWWindowMap[window].first.c_str(), GLFWWindowMap[window].second.c_str());
	}
	static void WindowResizeCallback(GLFWwindow* window, int width, int height)
	{
		WindowResizeEvent* windowResizeEvent = new WindowResizeEvent(width, height);

		OnEvent(windowResizeEvent, GLFWWindowMap[window].first.c_str(), GLFWWindowMap[window].second.c_str());
	}
	static void WindowMoveCallback(GLFWwindow* window, int xPos, int yPos)
	{
		WindowMoveEvent* windowMoveEvent = new WindowMoveEvent(xPos, yPos);

		OnEvent(windowMoveEvent, GLFWWindowMap[window].first.c_str(), GLFWWindowMap[window].second.c_str());
	}
	static void WindowMaximizeCallback(GLFWwindow* window, int maximized)
	{
		WindowMaximizeEvent* windowMaximizeEvent = new WindowMaximizeEvent(maximized);

		OnEvent(windowMaximizeEvent, GLFWWindowMap[window].first.c_str(), GLFWWindowMap[window].second.c_str());
	}
	static void WindowFocusCallback(GLFWwindow* window, int focused)
	{
		WindowFocusEvent* windowFocusEvent = new WindowFocusEvent(focused);

		OnEvent(windowFocusEvent, GLFWWindowMap[window].first.c_str(), GLFWWindowMap[window].second.c_str());
	}
	static void WindowIconifyCallback(GLFWwindow* window, int iconified)
	{
		WindowIconifyEvent* windowIconifyEvent = new WindowIconifyEvent(iconified);

		OnEvent(windowIconifyEvent, GLFWWindowMap[window].first.c_str(), GLFWWindowMap[window].second.c_str());
	}

	void Window::Initialize()
	{
		if (!glfwInit())
		{
			VLK_CORE_FATAL("Failed to initialize GLFW.");

			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}
	void Window::Terminate()
	{
		glfwTerminate();
	}

	Window::Window(const char* UUID, const char* ParentUUID, const char* title, int width, int height)
	{
		m_Window = glfwCreateWindow(width, height, title, NULL, NULL);

		glfwMakeContextCurrent(m_Window);

		glfwSetKeyCallback(m_Window, KeyCallback);
		glfwSetCharCallback(m_Window, CharCallback);
		glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
		glfwSetScrollCallback(m_Window, MouseScrollCallback);
		glfwSetCursorPosCallback(m_Window, MouseMoveCallback);
		glfwSetWindowSizeCallback(m_Window, WindowResizeCallback);
		glfwSetWindowPosCallback(m_Window, WindowMoveCallback);
		glfwSetWindowMaximizeCallback(m_Window, WindowMaximizeCallback);
		glfwSetWindowFocusCallback(m_Window, WindowFocusCallback);
		glfwSetWindowIconifyCallback(m_Window, WindowIconifyCallback);

		GLFWWindowMap[m_Window] = std::make_pair(UUID, ParentUUID);
	}
	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
	}

	const char* Window::GetTitle()
	{
		return glfwGetWindowTitle(m_Window);
	}
	void Window::GetSize(int& width, int& height)
	{
		glfwGetWindowSize(m_Window, &width, &height);
	}
	void Window::GetPos(int& x, int& y)
	{
		glfwGetWindowPos(m_Window, &x, &y);
	}

	bool Window::WindowClosed()
	{
		return glfwWindowShouldClose(m_Window);
	}

	void Window::SetEventFunction(OnEventFunction onEventFunction)
	{
		OnEvent = onEventFunction;
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	void Window::Update()
	{
		glfwSwapBuffers(m_Window);
	}
}
#pragma once

namespace Velkro
{
	class Event
	{
	public:
		Event() = default;
		virtual ~Event() = default;

		template<typename Typename>
		Typename* Get()
		{
			return dynamic_cast<Typename*>(this);
		}
	};

	class KeyEvent : public Event
	{
	public:
		KeyEvent() = default;
		KeyEvent(int code, int scancode, int action, int mods);
		~KeyEvent() = default;

		int GetCode() const;
		int GetScancode() const;
		int GetAction() const;
		int GetMods() const;

	private:
		int m_Code, m_Scancode, m_Action, m_Mods;
	};

	class CharacterEvent : public Event
	{
	public:
		CharacterEvent() = default;
		CharacterEvent(int codepoint);
		~CharacterEvent() = default;

		int GetCodepoint() const;

	private:
		int m_Codepoint;
	};

	class MouseButtonEvent : public Event
	{
	public:
		MouseButtonEvent() = default;
		MouseButtonEvent(int code, int action, int mods);
		~MouseButtonEvent() = default;

		int GetCode() const;
		int GetAction() const;
		int GetMods() const;

	private:
		int m_Code, m_Action, m_Mods;
	};

	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent() = default;
		MouseScrollEvent(double xOffset, double yOffset);
		~MouseScrollEvent() = default;

		double GetXOffset() const;
		double GetYOffset() const;

	private:
		double m_XOffset, m_YOffset;
	};

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent() = default;
		MouseMoveEvent(double xpos, double ypos);
		~MouseMoveEvent() = default;

		double GetXPos() const;
		double GetYPos() const;

	private:
		double m_XPos, m_YPos;
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent() = default;
		WindowResizeEvent(int width, int height);
		~WindowResizeEvent() = default;

		int GetWidth() const;
		int GetHeight() const;

	private:
		int m_Width, m_Height;
	};

	class WindowMoveEvent : public Event
	{
	public:
		WindowMoveEvent() = default;
		WindowMoveEvent(int xPos, int yPos);
		~WindowMoveEvent() = default;

		int GetXPos() const;
		int GetYPos() const;

	private:
		int m_XPos, m_YPos;
	};

	class WindowMaximizeEvent : public Event
	{
	public:
		WindowMaximizeEvent() = default;
		WindowMaximizeEvent(int maximized);
		~WindowMaximizeEvent() = default;

		int GetMaximized() const;

	private:
		int m_Maximized;
	};

	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent() = default;
		WindowFocusEvent(int focused);
		~WindowFocusEvent() = default;

		int GetFocused() const;

	private:
		int m_Focused;
	};

	class WindowIconifyEvent : public Event
	{
	public:
		WindowIconifyEvent() = default;
		WindowIconifyEvent(int iconified);
		~WindowIconifyEvent() = default;

		int GetFocused() const;

	private:
		int m_Iconified;
	};
}
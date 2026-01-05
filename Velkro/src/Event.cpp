#include "Event.h"

namespace Velkro
{
	KeyEvent::KeyEvent(int code, int scancode, int action, int mods)
		: m_Code(code), m_Scancode(scancode), m_Action(action), m_Mods(mods)
	{
	}

	int KeyEvent::GetCode() const
	{
		return m_Code;
	}
	int KeyEvent::GetScancode() const
	{
		return m_Scancode;
	}
	int KeyEvent::GetAction() const
	{
		return m_Action;
	}
	int KeyEvent::GetMods() const
	{
		return m_Mods;
	}

	CharacterEvent::CharacterEvent(int codepoint)
		: m_Codepoint(codepoint)
	{
	}

	int CharacterEvent::GetCodepoint() const
	{
		return m_Codepoint;
	}

	MouseButtonEvent::MouseButtonEvent(int code, int action, int mods)
		: m_Code(code), m_Action(action), m_Mods(mods)
	{
	}

	int MouseButtonEvent::GetCode() const
	{
		return m_Code;
	}
	int MouseButtonEvent::GetAction() const
	{
		return m_Action;
	}
	int MouseButtonEvent::GetMods() const
	{
		return m_Mods;
	}

	MouseScrollEvent::MouseScrollEvent(double xOffset, double yOffset)
		: m_XOffset(xOffset), m_YOffset(yOffset)
	{
	}

	double MouseScrollEvent::GetXOffset() const
	{
		return m_XOffset;
	}
	double MouseScrollEvent::GetYOffset() const
	{
		return m_YOffset;
	}

	MouseMoveEvent::MouseMoveEvent(double xPos, double yPos)
		: m_XPos(xPos), m_YPos(yPos)
	{
	}

	double MouseMoveEvent::GetXPos() const
	{
		return m_XPos;
	}
	double MouseMoveEvent::GetYPos() const
	{
		return m_YPos;
	}

	WindowResizeEvent::WindowResizeEvent(int width, int height)
		: m_Width(width), m_Height(height)
	{
	}

	int WindowResizeEvent::GetWidth() const
	{
		return m_Width;
	}
	int WindowResizeEvent::GetHeight() const
	{
		return m_Height;
	}

	WindowMoveEvent::WindowMoveEvent(int xPos, int yPos)
		: m_XPos(xPos), m_YPos(yPos)
	{
	}

	int WindowMoveEvent::GetXPos() const
	{
		return m_XPos;
	}
	int WindowMoveEvent::GetYPos() const
	{
		return m_YPos;
	}

	WindowMaximizeEvent::WindowMaximizeEvent(int maximized)
		: m_Maximized(maximized)
	{
	}

	int WindowMaximizeEvent::GetMaximized() const
	{
		return m_Maximized;
	}

	WindowFocusEvent::WindowFocusEvent(int focused)
		: m_Focused(focused)
	{
	}

	int WindowFocusEvent::GetFocused() const
	{
		return m_Focused;
	}

	WindowIconifyEvent::WindowIconifyEvent(int iconified)
		: m_Iconified(iconified)
	{
	}

	int WindowIconifyEvent::GetFocused() const
	{
		return m_Iconified;
	}
}
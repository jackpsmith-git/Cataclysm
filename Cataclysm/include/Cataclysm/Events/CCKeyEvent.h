#pragma once

#include "CCEvent.h"
#include "Cataclysm/Input/CCInput.h"

namespace Cataclysm
{
	class CCKeyEvent : public CCEvent
	{
	public:
		CCKeyCode GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		CCKeyEvent(const CCKeyCode keycode)
			: m_KeyCode(keycode) {}

		CCKeyCode m_KeyCode;
	};

	class CCKeyPressedEvent : public CCKeyEvent
	{
	public:
		CCKeyPressedEvent(const CCKeyCode keycode, bool isRepeat = false)
			: CCKeyEvent(keycode), m_IsRepeat(isRepeat) {}

		bool IsRepeat() const { return m_IsRepeat; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "CCKeyPressedEvent: " << m_KeyCode << " (repeat = " << m_IsRepeat << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool m_IsRepeat;
	};

	class CCKeyReleasedEvent : public CCKeyEvent
	{
	public:
		CCKeyReleasedEvent(const CCKeyCode keycode)
			: CCKeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "CCKeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class CCKeyTypedEvent : public CCKeyEvent
	{
	public:
		CCKeyTypedEvent(const CCKeyCode keycode)
			: CCKeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "CCKeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}

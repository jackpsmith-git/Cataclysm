#pragma once

#include "CCEvent.h"
#include "Cataclysm/Input/CCInput.h"

namespace Cataclysm
{
	class CCMouseMovedEvent : public CCEvent
	{
	public:
		CCMouseMovedEvent(float x, float y)
			: m_MouseX(x), m_MouseY(y) {}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "CCMouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_MouseX, m_MouseY;
	};

	class CCMouseScrolledEvent : public CCEvent
	{
	public:
		CCMouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "CCMouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_XOffset, m_YOffset;
	};

	class CCMouseButtonEvent : public CCEvent
	{
	public:
		inline CCMouseCode GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		CCMouseButtonEvent(CCMouseCode button)
			: m_Button(button) {}

		CCMouseCode m_Button;
	};

	class CCMouseButtonPressedEvent : public CCMouseButtonEvent
	{
	public:
		CCMouseButtonPressedEvent(CCMouseCode button)
			: CCMouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "CCMouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class CCMouseButtonReleasedEvent : public CCMouseButtonEvent
	{
	public:
		CCMouseButtonReleasedEvent(CCMouseCode button)
			: CCMouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "CCMouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}

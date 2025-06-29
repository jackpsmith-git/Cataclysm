#pragma once

#include "ccpch.h"
#include "Cataclysm/Core/CCCore.h"

namespace Cataclysm
{
	// Events in Cataclysm are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealth with right then and there.
	// For the future, a better strategy may be to buffer events in an event bus
	// and process them during the "event" phase of the update stage.

	enum class CCEventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum CCEventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static CCEventType GetStaticType() { return CCEventType::##type; }\
								virtual CCEventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class CCEvent
	{
	public:
		virtual ~CCEvent() = default;

		bool Handled = false;

		virtual CCEventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(CCEventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	class CCEventDispatcher
	{
	public:
		CCEventDispatcher(CCEvent& event)
			: m_Event(event)
		{
		}

		// F will be deduced by the compiler
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		CCEvent& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const CCEvent& e)
	{
		return os << e.ToString();
	}
}

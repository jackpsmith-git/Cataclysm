#pragma once

namespace Cataclysm
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define CC_MOUSE_BUTTON_0      ::Cataclysm::Mouse::Button0
#define CC_MOUSE_BUTTON_1      ::Cataclysm::Mouse::Button1
#define CC_MOUSE_BUTTON_2      ::Cataclysm::Mouse::Button2
#define CC_MOUSE_BUTTON_3      ::Cataclysm::Mouse::Button3
#define CC_MOUSE_BUTTON_4      ::Cataclysm::Mouse::Button4
#define CC_MOUSE_BUTTON_5      ::Cataclysm::Mouse::Button5
#define CC_MOUSE_BUTTON_6      ::Cataclysm::Mouse::Button6
#define CC_MOUSE_BUTTON_7      ::Cataclysm::Mouse::Button7
#define CC_MOUSE_BUTTON_LAST   ::Cataclysm::Mouse::ButtonLast
#define CC_MOUSE_BUTTON_LEFT   ::Cataclysm::Mouse::ButtonLeft
#define CC_MOUSE_BUTTON_RIGHT  ::Cataclysm::Mouse::ButtonRight
#define CC_MOUSE_BUTTON_MIDDLE ::Cataclysm::Mouse::ButtonMiddle
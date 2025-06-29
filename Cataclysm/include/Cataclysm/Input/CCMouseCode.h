#pragma once

namespace Cataclysm
{
	typedef enum class CCMouseCode : uint16_t
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
	} CCMouse;

	inline std::ostream& operator<<(std::ostream& os, CCMouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define CC_MOUSE_BUTTON_0      ::Cataclysm::CCMouse::Button0
#define CC_MOUSE_BUTTON_1      ::Cataclysm::CCMouse::Button1
#define CC_MOUSE_BUTTON_2      ::Cataclysm::CCMouse::Button2
#define CC_MOUSE_BUTTON_3      ::Cataclysm::CCMouse::Button3
#define CC_MOUSE_BUTTON_4      ::Cataclysm::CCMouse::Button4
#define CC_MOUSE_BUTTON_5      ::Cataclysm::CCMouse::Button5
#define CC_MOUSE_BUTTON_6      ::Cataclysm::CCMouse::Button6
#define CC_MOUSE_BUTTON_7      ::Cataclysm::CCMouse::Button7
#define CC_MOUSE_BUTTON_LAST   ::Cataclysm::CCMouse::ButtonLast
#define CC_MOUSE_BUTTON_LEFT   ::Cataclysm::CCMouse::ButtonLeft
#define CC_MOUSE_BUTTON_RIGHT  ::Cataclysm::CCMouse::ButtonRight
#define CC_MOUSE_BUTTON_MIDDLE ::Cataclysm::CCMouse::ButtonMiddle

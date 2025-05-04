#pragma once

namespace Cataclysm
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define CC_KEY_SPACE           ::Cataclysm::Key::Space
#define CC_KEY_APOSTROPHE      ::Cataclysm::Key::Apostrophe    /* ' */
#define CC_KEY_COMMA           ::Cataclysm::Key::Comma         /* , */
#define CC_KEY_MINUS           ::Cataclysm::Key::Minus         /* - */
#define CC_KEY_PERIOD          ::Cataclysm::Key::Period        /* . */
#define CC_KEY_SLASH           ::Cataclysm::Key::Slash         /* / */
#define CC_KEY_0               ::Cataclysm::Key::D0
#define CC_KEY_1               ::Cataclysm::Key::D1
#define CC_KEY_2               ::Cataclysm::Key::D2
#define CC_KEY_3               ::Cataclysm::Key::D3
#define CC_KEY_4               ::Cataclysm::Key::D4
#define CC_KEY_5               ::Cataclysm::Key::D5
#define CC_KEY_6               ::Cataclysm::Key::D6
#define CC_KEY_7               ::Cataclysm::Key::D7
#define CC_KEY_8               ::Cataclysm::Key::D8
#define CC_KEY_9               ::Cataclysm::Key::D9
#define CC_KEY_SEMICOLON       ::Cataclysm::Key::Semicolon     /* ; */
#define CC_KEY_EQUAL           ::Cataclysm::Key::Equal         /* = */
#define CC_KEY_A               ::Cataclysm::Key::A
#define CC_KEY_B               ::Cataclysm::Key::B
#define CC_KEY_C               ::Cataclysm::Key::C
#define CC_KEY_D               ::Cataclysm::Key::D
#define CC_KEY_E               ::Cataclysm::Key::E
#define CC_KEY_F               ::Cataclysm::Key::F
#define CC_KEY_G               ::Cataclysm::Key::G
#define CC_KEY_H               ::Cataclysm::Key::H
#define CC_KEY_I               ::Cataclysm::Key::I
#define CC_KEY_J               ::Cataclysm::Key::J
#define CC_KEY_K               ::Cataclysm::Key::K
#define CC_KEY_L               ::Cataclysm::Key::L
#define CC_KEY_M               ::Cataclysm::Key::M
#define CC_KEY_N               ::Cataclysm::Key::N
#define CC_KEY_O               ::Cataclysm::Key::O
#define CC_KEY_P               ::Cataclysm::Key::P
#define CC_KEY_Q               ::Cataclysm::Key::Q
#define CC_KEY_R               ::Cataclysm::Key::R
#define CC_KEY_S               ::Cataclysm::Key::S
#define CC_KEY_T               ::Cataclysm::Key::T
#define CC_KEY_U               ::Cataclysm::Key::U
#define CC_KEY_V               ::Cataclysm::Key::V
#define CC_KEY_W               ::Cataclysm::Key::W
#define CC_KEY_X               ::Cataclysm::Key::X
#define CC_KEY_Y               ::Cataclysm::Key::Y
#define CC_KEY_Z               ::Cataclysm::Key::Z
#define CC_KEY_LEFT_BRACKET    ::Cataclysm::Key::LeftBracket   /* [ */
#define CC_KEY_BACKSLASH       ::Cataclysm::Key::Backslash     /* \ */
#define CC_KEY_RIGHT_BRACKET   ::Cataclysm::Key::RightBracket  /* ] */
#define CC_KEY_GRAVE_ACCENT    ::Cataclysm::Key::GraveAccent   /* ` */
#define CC_KEY_WORLD_1         ::Cataclysm::Key::World1        /* non-US #1 */
#define CC_KEY_WORLD_2         ::Cataclysm::Key::World2        /* non-US #2 */

/* Function keys */
#define CC_KEY_ESCAPE          ::Cataclysm::Key::Escape
#define CC_KEY_ENTER           ::Cataclysm::Key::Enter
#define CC_KEY_TAB             ::Cataclysm::Key::Tab
#define CC_KEY_BACKSPACE       ::Cataclysm::Key::Backspace
#define CC_KEY_INSERT          ::Cataclysm::Key::Insert
#define CC_KEY_DELETE          ::Cataclysm::Key::Delete
#define CC_KEY_RIGHT           ::Cataclysm::Key::Right
#define CC_KEY_LEFT            ::Cataclysm::Key::Left
#define CC_KEY_DOWN            ::Cataclysm::Key::Down
#define CC_KEY_UP              ::Cataclysm::Key::Up
#define CC_KEY_PAGE_UP         ::Cataclysm::Key::PageUp
#define CC_KEY_PAGE_DOWN       ::Cataclysm::Key::PageDown
#define CC_KEY_HOME            ::Cataclysm::Key::Home
#define CC_KEY_END             ::Cataclysm::Key::End
#define CC_KEY_CAPS_LOCK       ::Cataclysm::Key::CapsLock
#define CC_KEY_SCROLL_LOCK     ::Cataclysm::Key::ScrollLock
#define CC_KEY_NUM_LOCK        ::Cataclysm::Key::NumLock
#define CC_KEY_PRINT_SCREEN    ::Cataclysm::Key::PrintScreen
#define CC_KEY_PAUSE           ::Cataclysm::Key::Pause
#define CC_KEY_F1              ::Cataclysm::Key::F1
#define CC_KEY_F2              ::Cataclysm::Key::F2
#define CC_KEY_F3              ::Cataclysm::Key::F3
#define CC_KEY_F4              ::Cataclysm::Key::F4
#define CC_KEY_F5              ::Cataclysm::Key::F5
#define CC_KEY_F6              ::Cataclysm::Key::F6
#define CC_KEY_F7              ::Cataclysm::Key::F7
#define CC_KEY_F8              ::Cataclysm::Key::F8
#define CC_KEY_F9              ::Cataclysm::Key::F9
#define CC_KEY_F10             ::Cataclysm::Key::F10
#define CC_KEY_F11             ::Cataclysm::Key::F11
#define CC_KEY_F12             ::Cataclysm::Key::F12
#define CC_KEY_F13             ::Cataclysm::Key::F13
#define CC_KEY_F14             ::Cataclysm::Key::F14
#define CC_KEY_F15             ::Cataclysm::Key::F15
#define CC_KEY_F17             ::Cataclysm::Key::F17
#define CC_KEY_F18             ::Cataclysm::Key::F18
#define CC_KEY_F19             ::Cataclysm::Key::F19
#define CC_KEY_F20             ::Cataclysm::Key::F20
#define CC_KEY_F21             ::Cataclysm::Key::F21
#define CC_KEY_F22             ::Cataclysm::Key::F22
#define CC_KEY_F23             ::Cataclysm::Key::F23
#define CC_KEY_F24             ::Cataclysm::Key::F24
#define CC_KEY_F25             ::Cataclysm::Key::F25

/* Keypad */
#define CC_KEY_KP_0            ::Cataclysm::Key::KP0
#define CC_KEY_KP_1            ::Cataclysm::Key::KP1
#define CC_KEY_KP_2            ::Cataclysm::Key::KP2
#define CC_KEY_KP_3            ::Cataclysm::Key::KP3
#define CC_KEY_KP_4            ::Cataclysm::Key::KP4
#define CC_KEY_KP_5            ::Cataclysm::Key::KP5
#define CC_KEY_KP_6            ::Cataclysm::Key::KP6
#define CC_KEY_KP_7            ::Cataclysm::Key::KP7
#define CC_KEY_KP_8            ::Cataclysm::Key::KP8
#define CC_KEY_KP_9            ::Cataclysm::Key::KP9
#define CC_KEY_KP_DECIMAL      ::Cataclysm::Key::KPDecimal
#define CC_KEY_KP_DIVIDE       ::Cataclysm::Key::KPDivide
#define CC_KEY_KP_MULTIPLY     ::Cataclysm::Key::KPMultiply
#define CC_KEY_KP_SUBTRACT     ::Cataclysm::Key::KPSubtract
#define CC_KEY_KP_ADD          ::Cataclysm::Key::KPAdd
#define CC_KEY_KP_ENTER        ::Cataclysm::Key::KPEnter
#define CC_KEY_KP_EQUAL        ::Cataclysm::Key::KPEqual

#define CC_KEY_LEFT_SHIFT      ::Cataclysm::Key::LeftShift
#define CC_KEY_LEFT_CONTROL    ::Cataclysm::Key::LeftControl
#define CC_KEY_LEFT_ALT        ::Cataclysm::Key::LeftAlt
#define CC_KEY_LEFT_SUPER      ::Cataclysm::Key::LeftSuper
#define CC_KEY_RIGHT_SHIFT     ::Cataclysm::Key::RightShift
#define CC_KEY_RIGHT_CONTROL   ::Cataclysm::Key::RightControl
#define CC_KEY_RIGHT_ALT       ::Cataclysm::Key::RightAlt
#define CC_KEY_RIGHT_SUPER     ::Cataclysm::Key::RightSuper
#define CC_KEY_MENU            ::Cataclysm::Key::Menu

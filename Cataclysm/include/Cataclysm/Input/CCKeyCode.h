#pragma once

namespace Cataclysm
{
	typedef enum class CCKeyCode : uint16_t
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
	} CCKey;

	inline std::ostream& operator<<(std::ostream& os, CCKeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define CC_KEY_SPACE           ::Cataclysm::CCKey::Space
#define CC_KEY_APOSTROPHE      ::Cataclysm::CCKey::Apostrophe    /* ' */
#define CC_KEY_COMMA           ::Cataclysm::CCKey::Comma         /* , */
#define CC_KEY_MINUS           ::Cataclysm::CCKey::Minus         /* - */
#define CC_KEY_PERIOD          ::Cataclysm::CCKey::Period        /* . */
#define CC_KEY_SLASH           ::Cataclysm::CCKey::Slash         /* / */
#define CC_KEY_0               ::Cataclysm::CCKey::D0
#define CC_KEY_1               ::Cataclysm::CCKey::D1
#define CC_KEY_2               ::Cataclysm::CCKey::D2
#define CC_KEY_3               ::Cataclysm::CCKey::D3
#define CC_KEY_4               ::Cataclysm::CCKey::D4
#define CC_KEY_5               ::Cataclysm::CCKey::D5
#define CC_KEY_6               ::Cataclysm::CCKey::D6
#define CC_KEY_7               ::Cataclysm::CCKey::D7
#define CC_KEY_8               ::Cataclysm::CCKey::D8
#define CC_KEY_9               ::Cataclysm::CCKey::D9
#define CC_KEY_SEMICOLON       ::Cataclysm::CCKey::Semicolon     /* ; */
#define CC_KEY_EQUAL           ::Cataclysm::CCKey::Equal         /* = */
#define CC_KEY_A               ::Cataclysm::CCKey::A
#define CC_KEY_B               ::Cataclysm::CCKey::B
#define CC_KEY_C               ::Cataclysm::CCKey::C
#define CC_KEY_D               ::Cataclysm::CCKey::D
#define CC_KEY_E               ::Cataclysm::CCKey::E
#define CC_KEY_F               ::Cataclysm::CCKey::F
#define CC_KEY_G               ::Cataclysm::CCKey::G
#define CC_KEY_H               ::Cataclysm::CCKey::H
#define CC_KEY_I               ::Cataclysm::CCKey::I
#define CC_KEY_J               ::Cataclysm::CCKey::J
#define CC_KEY_K               ::Cataclysm::CCKey::K
#define CC_KEY_L               ::Cataclysm::CCKey::L
#define CC_KEY_M               ::Cataclysm::CCKey::M
#define CC_KEY_N               ::Cataclysm::CCKey::N
#define CC_KEY_O               ::Cataclysm::CCKey::O
#define CC_KEY_P               ::Cataclysm::CCKey::P
#define CC_KEY_Q               ::Cataclysm::CCKey::Q
#define CC_KEY_R               ::Cataclysm::CCKey::R
#define CC_KEY_S               ::Cataclysm::CCKey::S
#define CC_KEY_T               ::Cataclysm::CCKey::T
#define CC_KEY_U               ::Cataclysm::CCKey::U
#define CC_KEY_V               ::Cataclysm::CCKey::V
#define CC_KEY_W               ::Cataclysm::CCKey::W
#define CC_KEY_X               ::Cataclysm::CCKey::X
#define CC_KEY_Y               ::Cataclysm::CCKey::Y
#define CC_KEY_Z               ::Cataclysm::CCKey::Z
#define CC_KEY_LEFT_BRACKET    ::Cataclysm::CCKey::LeftBracket   /* [ */
#define CC_KEY_BACKSLASH       ::Cataclysm::CCKey::Backslash     /* \ */
#define CC_KEY_RIGHT_BRACKET   ::Cataclysm::CCKey::RightBracket  /* ] */
#define CC_KEY_GRAVE_ACCENT    ::Cataclysm::CCKey::GraveAccent   /* ` */
#define CC_KEY_WORLD_1         ::Cataclysm::CCKey::World1        /* non-US #1 */
#define CC_KEY_WORLD_2         ::Cataclysm::CCKey::World2        /* non-US #2 */

/* Function keys */
#define CC_KEY_ESCAPE          ::Cataclysm::CCKey::Escape
#define CC_KEY_ENTER           ::Cataclysm::CCKey::Enter
#define CC_KEY_TAB             ::Cataclysm::CCKey::Tab
#define CC_KEY_BACKSPACE       ::Cataclysm::CCKey::Backspace
#define CC_KEY_INSERT          ::Cataclysm::CCKey::Insert
#define CC_KEY_DELETE          ::Cataclysm::CCKey::Delete
#define CC_KEY_RIGHT           ::Cataclysm::CCKey::Right
#define CC_KEY_LEFT            ::Cataclysm::CCKey::Left
#define CC_KEY_DOWN            ::Cataclysm::CCKey::Down
#define CC_KEY_UP              ::Cataclysm::CCKey::Up
#define CC_KEY_PAGE_UP         ::Cataclysm::CCKey::PageUp
#define CC_KEY_PAGE_DOWN       ::Cataclysm::CCKey::PageDown
#define CC_KEY_HOME            ::Cataclysm::CCKey::Home
#define CC_KEY_END             ::Cataclysm::CCKey::End
#define CC_KEY_CAPS_LOCK       ::Cataclysm::CCKey::CapsLock
#define CC_KEY_SCROLL_LOCK     ::Cataclysm::CCKey::ScrollLock
#define CC_KEY_NUM_LOCK        ::Cataclysm::CCKey::NumLock
#define CC_KEY_PRINT_SCREEN    ::Cataclysm::CCKey::PrintScreen
#define CC_KEY_PAUSE           ::Cataclysm::CCKey::Pause
#define CC_KEY_F1              ::Cataclysm::CCKey::F1
#define CC_KEY_F2              ::Cataclysm::CCKey::F2
#define CC_KEY_F3              ::Cataclysm::CCKey::F3
#define CC_KEY_F4              ::Cataclysm::CCKey::F4
#define CC_KEY_F5              ::Cataclysm::CCKey::F5
#define CC_KEY_F6              ::Cataclysm::CCKey::F6
#define CC_KEY_F7              ::Cataclysm::CCKey::F7
#define CC_KEY_F8              ::Cataclysm::CCKey::F8
#define CC_KEY_F9              ::Cataclysm::CCKey::F9
#define CC_KEY_F10             ::Cataclysm::CCKey::F10
#define CC_KEY_F11             ::Cataclysm::CCKey::F11
#define CC_KEY_F12             ::Cataclysm::CCKey::F12
#define CC_KEY_F13             ::Cataclysm::CCKey::F13
#define CC_KEY_F14             ::Cataclysm::CCKey::F14
#define CC_KEY_F15             ::Cataclysm::CCKey::F15
#define CC_KEY_F17             ::Cataclysm::CCKey::F17
#define CC_KEY_F18             ::Cataclysm::CCKey::F18
#define CC_KEY_F19             ::Cataclysm::CCKey::F19
#define CC_KEY_F20             ::Cataclysm::CCKey::F20
#define CC_KEY_F21             ::Cataclysm::CCKey::F21
#define CC_KEY_F22             ::Cataclysm::CCKey::F22
#define CC_KEY_F23             ::Cataclysm::CCKey::F23
#define CC_KEY_F24             ::Cataclysm::CCKey::F24
#define CC_KEY_F25             ::Cataclysm::CCKey::F25

/* Keypad */
#define CC_KEY_KP_0            ::Cataclysm::CCKey::KP0
#define CC_KEY_KP_1            ::Cataclysm::CCKey::KP1
#define CC_KEY_KP_2            ::Cataclysm::CCKey::KP2
#define CC_KEY_KP_3            ::Cataclysm::CCKey::KP3
#define CC_KEY_KP_4            ::Cataclysm::CCKey::KP4
#define CC_KEY_KP_5            ::Cataclysm::CCKey::KP5
#define CC_KEY_KP_6            ::Cataclysm::CCKey::KP6
#define CC_KEY_KP_7            ::Cataclysm::CCKey::KP7
#define CC_KEY_KP_8            ::Cataclysm::CCKey::KP8
#define CC_KEY_KP_9            ::Cataclysm::CCKey::KP9
#define CC_KEY_KP_DECIMAL      ::Cataclysm::CCKey::KPDecimal
#define CC_KEY_KP_DIVIDE       ::Cataclysm::CCKey::KPDivide
#define CC_KEY_KP_MULTIPLY     ::Cataclysm::CCKey::KPMultiply
#define CC_KEY_KP_SUBTRACT     ::Cataclysm::CCKey::KPSubtract
#define CC_KEY_KP_ADD          ::Cataclysm::CCKey::KPAdd
#define CC_KEY_KP_ENTER        ::Cataclysm::CCKey::KPEnter
#define CC_KEY_KP_EQUAL        ::Cataclysm::CCKey::KPEqual

#define CC_KEY_LEFT_SHIFT      ::Cataclysm::CCKey::LeftShift
#define CC_KEY_LEFT_CONTROL    ::Cataclysm::CCKey::LeftControl
#define CC_KEY_LEFT_ALT        ::Cataclysm::CCKey::LeftAlt
#define CC_KEY_LEFT_SUPER      ::Cataclysm::CCKey::LeftSuper
#define CC_KEY_RIGHT_SHIFT     ::Cataclysm::CCKey::RightShift
#define CC_KEY_RIGHT_CONTROL   ::Cataclysm::CCKey::RightControl
#define CC_KEY_RIGHT_ALT       ::Cataclysm::CCKey::RightAlt
#define CC_KEY_RIGHT_SUPER     ::Cataclysm::CCKey::RightSuper
#define CC_KEY_MENU            ::Cataclysm::CCKey::Menu

#pragma once

#include "CCScriptField.h"

namespace Cataclysm
{
	struct CCScriptFieldInstance
	{
		CCScriptField Field;

		CCScriptFieldInstance()
		{
			memset(m_Buffer, 0, sizeof(m_Buffer));
		}

		template<typename T>
		T GetValue() const
		{
			static_assert(sizeof(T) <= 64, "Type too large!");
			return *(T*)m_Buffer;
		}

		template<typename T>
		void SetValue(T value)
		{
			static_assert(sizeof(T) <= 64, "Type too large!");
			memcpy(m_Buffer, &value, sizeof(T));
		}

		template<>
		inline std::string GetValue<std::string>() const
		{
			std::string* str = nullptr;
			memcpy(&str, m_Buffer, sizeof(std::string*));
			return str ? *str : std::string();
		}

		template<> 
		inline void SetValue<std::string>(std::string value)
		{
			std::string* str = new std::string(std::move(value));
			memcpy(m_Buffer, &str, sizeof(std::string*));
		}
	private:
		uint8_t m_Buffer[64];

		friend class CCScriptEngine;
		friend class CCScriptInstance;
	};
}

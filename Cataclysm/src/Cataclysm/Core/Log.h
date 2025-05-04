#pragma once

#include "Cataclysm/Core/Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <memory>

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Cataclysm
{
	class Log
	{
	public:
		static void Init();
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternio)
{
	return os << glm::to_string(quaternio);
}

// Core log macros
#define CC_CORE_TRACE(...)     ::Cataclysm::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CC_CORE_INFO(...)      ::Cataclysm::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CC_CORE_WARN(...)      ::Cataclysm::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CC_CORE_ERROR(...)     ::Cataclysm::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CC_CORE_CRITICAL(...)  ::Cataclysm::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define CC_TRACE(...)          ::Cataclysm::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CC_INFO(...)           ::Cataclysm::Log::GetClientLogger()->info(__VA_ARGS__)
#define CC_WARN(...)           ::Cataclysm::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CC_ERROR(...)          ::Cataclysm::Log::GetClientLogger()->error(__VA_ARGS__)
#define CC_CRITICAL(...)       ::Cataclysm::Log::GetClientLogger()->critical(__VA_ARGS__)

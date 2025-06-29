#pragma once

#include "Cataclysm/Core/CCCore.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <memory>

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Cataclysm
{
	class CCLog
	{
	public:
		static void Init();
		static CCRef<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		static CCRef<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::filesystem::path GetLogFilePath();
	private:
		static CCRef<spdlog::logger> s_CoreLogger;
		static CCRef<spdlog::logger> s_ClientLogger;
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
#define CC_CORE_TRACE(...)     ::Cataclysm::CCLog::GetCoreLogger()->trace(__VA_ARGS__)
#define CC_CORE_INFO(...)      ::Cataclysm::CCLog::GetCoreLogger()->info(__VA_ARGS__)
#define CC_CORE_WARN(...)      ::Cataclysm::CCLog::GetCoreLogger()->warn(__VA_ARGS__)
#define CC_CORE_ERROR(...)     ::Cataclysm::CCLog::GetCoreLogger()->error(__VA_ARGS__)
#define CC_CORE_CRITICAL(...)  ::Cataclysm::CCLog::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define CC_TRACE(...)          ::Cataclysm::CCLog::GetClientLogger()->trace(__VA_ARGS__)
#define CC_INFO(...)           ::Cataclysm::CCLog::GetClientLogger()->info(__VA_ARGS__)
#define CC_WARN(...)           ::Cataclysm::CCLog::GetClientLogger()->warn(__VA_ARGS__)
#define CC_ERROR(...)          ::Cataclysm::CCLog::GetClientLogger()->error(__VA_ARGS__)
#define CC_CRITICAL(...)       ::Cataclysm::CCLog::GetClientLogger()->critical(__VA_ARGS__)

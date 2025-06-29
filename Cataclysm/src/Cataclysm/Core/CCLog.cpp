#include "ccpch.h"
#include "Cataclysm/Core/CCLog.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <filesystem>

#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>  

namespace Cataclysm
{
	CCRef<spdlog::logger> CCLog::s_CoreLogger;
	CCRef<spdlog::logger> CCLog::s_ClientLogger;

	void CCLog::Init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());

		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(GetLogFilePath().string().c_str(), true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		s_CoreLogger = std::make_shared<spdlog::logger>("Cataclysm", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);
	}

	std::filesystem::path CCLog::GetLogFilePath()
	{
		PWSTR path = nullptr;
		SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &path);
		std::filesystem::path appDataPath = path;
		CoTaskMemFree(path);
		appDataPath /= "Cataclysm";
		appDataPath /= "Logs";
		appDataPath /= "Cataclysm.log";

		return appDataPath;
	}
}

#pragma once
#include "Cataclysm/Core/CCCore.h"
#include "Cataclysm/Core/CCApplication.h"

#include <filesystem>

#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>  

#ifdef CC_PLATFORM_WINDOWS

extern Cataclysm::CCApplication* Cataclysm::CreateApplication(CCApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	// Uncomment this to make double clicking .ccproj files open caterpillar.
	// We still need to fix so that it actually opens the project after opening caterpillar
	// std::filesystem::current_path("S:/repos/c++/Cataclysm/Caterpillar");

	Cataclysm::CCLog::Init();

	PWSTR path = nullptr;
	SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &path);
	std::filesystem::path startupPath = path;
	std::filesystem::path runtimePath = path;
	std::filesystem::path shutdownPath = path;
	CoTaskMemFree(path);

	startupPath /= "Cataclysm";
	startupPath /= "Profiles";
	startupPath /= "CataclysmProfile-Startup.json";

	runtimePath /= "Cataclysm";
	runtimePath /= "Profiles";
	runtimePath /= "CataclysmProfile-Runtime.json";

	shutdownPath /= "Cataclysm";
	shutdownPath /= "Profiles";
	shutdownPath /= "CataclysmProfile-Shutdown.json";

	std::filesystem::create_directories(startupPath.parent_path());
	std::filesystem::create_directories(runtimePath.parent_path());
	std::filesystem::create_directories(shutdownPath.parent_path());

	std::filesystem::remove(startupPath);
	std::filesystem::remove(runtimePath);
	std::filesystem::remove(shutdownPath);

	std::ofstream startupFile(startupPath);
	startupFile.close();
	std::ofstream runtimeFile(runtimePath);
	runtimeFile.close();
	std::ofstream shutdownFile(shutdownPath);
	shutdownFile.close();

	CC_PROFILE_BEGIN_SESSION("Startup", startupPath.string().c_str());
	auto app = Cataclysm::CreateApplication({ argc, argv });
	CC_PROFILE_END_SESSION();

	CC_PROFILE_BEGIN_SESSION("Runtime", runtimePath.string().c_str());
	app->Run();	
	CC_PROFILE_END_SESSION();

	CC_PROFILE_BEGIN_SESSION("Shutdown", shutdownPath.string().c_str());
	delete app;
	CC_PROFILE_END_SESSION();
}

#endif

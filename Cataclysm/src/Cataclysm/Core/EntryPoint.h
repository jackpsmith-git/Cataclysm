#pragma once
#include "Cataclysm/Core/Core.h"
#include "Cataclysm/Core/Application.h"

#ifdef CC_PLATFORM_WINDOWS

extern Cataclysm::Application* Cataclysm::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	Cataclysm::Log::Init();

	CC_PROFILE_BEGIN_SESSION("Startup", "CataclysmProfile-Startup.json");
	auto app = Cataclysm::CreateApplication({ argc, argv });
	CC_PROFILE_END_SESSION();

	CC_PROFILE_BEGIN_SESSION("Runtime", "CataclysmProfile-Runtime.json");
	app->Run();	
	CC_PROFILE_END_SESSION();

	CC_PROFILE_BEGIN_SESSION("Shutdown", "CataclysmProfile-Shutdown.json");
	delete app;
	CC_PROFILE_END_SESSION();
}

#endif
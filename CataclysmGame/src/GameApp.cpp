#include <Cataclysm.h>
#include <Cataclysm/Core/EntryPoint.h>

#include "../include/GameLayer.h"

namespace Cataclysm
{
	class Game : public Cataclysm::CCApplication
	{
	public:
		Game(const Cataclysm::CCApplicationSpecification& spec)
			: Cataclysm::CCApplication(spec)
		{
			PushLayer(new Cataclysm::GameLayer());
		}
	};

	Cataclysm::CCApplication* CreateApplication(Cataclysm::CCApplicationCommandLineArgs args)
	{
		Cataclysm::CCApplicationSpecification spec;
		spec.Name = "Game";
		spec.CommandLineArgs = args;

		return new Game(spec);
	}
}

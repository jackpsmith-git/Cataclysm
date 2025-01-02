#include <Cataclysm.h>
#include <Cataclysm/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Cataclysm
{
	class Caterpillar : public Cataclysm::Application
	{
	public:
		Caterpillar(const Cataclysm::ApplicationSpecification& spec)
			: Cataclysm::Application(spec)
		{
			PushLayer(new Cataclysm::EditorLayer());
		}
	};

	Cataclysm::Application* CreateApplication(Cataclysm::ApplicationCommandLineArgs args)
	{
		Cataclysm::ApplicationSpecification spec;
		spec.Name = "Caterpillar";
		spec.CommandLineArgs = args;

		return new Caterpillar(spec);
	}
}

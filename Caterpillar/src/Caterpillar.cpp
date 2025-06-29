#include <Cataclysm.h>
#include <Cataclysm/Core/EntryPoint.h>

#include "CPEditorLayer.h"

namespace Cataclysm
{
	class Caterpillar : public CCApplication
	{
	public:
		Caterpillar(const Cataclysm::CCApplicationSpecification& spec)
			: Cataclysm::CCApplication(spec)
		{
			PushLayer(new CPEditorLayer());
		}
	};

	CCApplication* CreateApplication(CCApplicationCommandLineArgs args)
	{
		Cataclysm::CCApplicationSpecification spec;
		spec.Name = "Caterpillar";
		spec.CommandLineArgs = args;

		return new Caterpillar(spec);
	}
}

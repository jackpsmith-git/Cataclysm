#include <Cataclysm.h>
#include <Cataclysm/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "ExampleLayer.h"

class Sandbox : public Cataclysm::Application
{
public:
	Sandbox()
	{
		// PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}
};

Cataclysm::Application* Cataclysm::CreateApplication()
{
	return new Sandbox();
}
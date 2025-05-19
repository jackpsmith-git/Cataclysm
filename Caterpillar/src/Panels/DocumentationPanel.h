#pragma once

#include "Cataclysm.h"

namespace Cataclysm
{
	struct Property {
		std::string name;
		std::string type;
		std::string summary;
	};

	struct Method {
		std::string signature;
		std::string returnType;
		std::string summary;
	};

	struct DocEntry {
		std::string name;
		std::string path;
		std::string summary;
		std::vector<Property> properties;
		std::vector<Method> methods;
	};

	class DocumentationPanel
	{
	public:
		DocumentationPanel();

		void OnImGuiRender();
	private:
		static std::vector<DocEntry> s_Documentation;
	};
}

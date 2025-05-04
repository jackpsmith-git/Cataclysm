#pragma once

#include "Cataclysm.h"

namespace Cataclysm
{
	class OutputPanel
	{
	private:
		static void OutputPanel::ParseMessageType(std::string message, std::string type);
	public:
		OutputPanel() = default;

		void OnImGuiRender();
	};
}

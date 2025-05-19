#pragma once

#include "Cataclysm.h"

namespace Cataclysm
{
	class OutputPanel
	{
	private:
		static void OutputPanel::ParseMessageType(std::string message, std::string type);

		bool warningsEnabled = true;
		bool errorsEnabled = true;
		bool traceEnabled = true;
		bool infoEnabled = true;
	public:
		OutputPanel() = default;

		void OnImGuiRender(
			Cataclysm::Ref<Cataclysm::Texture2D> clearIcon,
			Cataclysm::Ref<Cataclysm::Texture2D> errorIcon,
			Cataclysm::Ref<Cataclysm::Texture2D> warningIcon,
			Cataclysm::Ref<Cataclysm::Texture2D> traceIcon,
			Cataclysm::Ref<Cataclysm::Texture2D> infoIcon,
			Cataclysm::Ref<Cataclysm::Texture2D> errorIconFaded,
			Cataclysm::Ref<Cataclysm::Texture2D> warningIconFaded,
			Cataclysm::Ref<Cataclysm::Texture2D> traceIconFaded,
			Cataclysm::Ref<Cataclysm::Texture2D> infoIconFaded
			);
	};
}

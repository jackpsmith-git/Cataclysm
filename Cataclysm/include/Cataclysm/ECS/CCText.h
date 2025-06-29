#pragma once

#include "Cataclysm/Vesuvius/VSFont.h"
#include <glm/glm.hpp>

namespace Cataclysm
{
	struct Text
	{
		std::string TextString;
		CCRef<Vesuvius::VSFont> FontAsset = Vesuvius::VSFont::GetDefault();
		glm::vec4 Color{ 1.0f };
		float Kerning = 0.0f;
		float LineSpacing = 0.0f;
		std::string FontPath = "assets/fonts/opensans/OpenSans-Regular.ttf";

		Text() = default;
		Text(const Text&) = default;

		void Reset()
		{
			TextString.clear();
			FontAsset = Vesuvius::VSFont::GetDefault();
			Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			Kerning = 0.0f;
			LineSpacing = 0.0f;
			FontPath = "assets/fonts/opensans/OpenSans-Regular.ttf";
		}
	};
}

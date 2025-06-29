#pragma once

#include <glm/glm.hpp>

namespace Cataclysm::Vesuvius
{
	struct VSTextParams
	{
		glm::vec4 Color{ 1.0f };
		float Kerning = 0.0f;
		float LineSpacing = 0.0f;
	};
}

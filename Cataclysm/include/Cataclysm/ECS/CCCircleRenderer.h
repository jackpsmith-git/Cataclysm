#pragma once

#include <glm/glm.hpp>

namespace Cataclysm
{
	struct CircleRenderer
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float Thickness = 1.0f;
		float Fade = 0.005f;

		CircleRenderer() = default;
		CircleRenderer(const CircleRenderer&) = default;

		void Reset()
		{
			Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			Thickness = 1.0f;
			Fade = 0.005f;
		}
	};
}

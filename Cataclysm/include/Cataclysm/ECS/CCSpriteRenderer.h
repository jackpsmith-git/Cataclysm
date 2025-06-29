#pragma once

#include "Cataclysm/Vesuvius/VSTexture.h"
#include <glm/glm.hpp>

namespace Cataclysm
{
	struct SpriteRenderer
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		CCRef<Vesuvius::VSTexture2D> Texture;
		float TilingFactor = 1.0f;

		SpriteRenderer() = default;
		SpriteRenderer(const SpriteRenderer&) = default;
		SpriteRenderer(const glm::vec4& color)
			: Color(color) {}

		void Reset()
		{
			Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			Texture = NULL;
			TilingFactor = 1.0f;
		}
	};
}

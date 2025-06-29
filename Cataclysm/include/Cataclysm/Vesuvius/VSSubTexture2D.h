#pragma once

#include <glm/glm.hpp>

#include "VSTexture.h"

namespace Cataclysm::Vesuvius
{
	class VSSubTexture2D
	{
	public:
		VSSubTexture2D(const CCRef<VSTexture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const CCRef<VSTexture2D> GetTexture() const { return m_Texture; }
		const glm::vec2* GetTexCoords() const { return m_TexCoords; }

		static CCRef<VSSubTexture2D> CreateFromCoords(const CCRef<VSTexture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1, 1 });

	private:
		CCRef<VSTexture2D> m_Texture;

		glm::vec2 m_TexCoords[4];
	};
}

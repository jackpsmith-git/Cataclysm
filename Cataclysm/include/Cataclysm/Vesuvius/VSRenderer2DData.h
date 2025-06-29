#pragma once

#include "Cataclysm.h"

#include "Cataclysm/Vesuvius/VSVertex.h"
#include "Cataclysm/Vesuvius/VSUniformBuffer.h"

namespace Cataclysm::Vesuvius
{
	struct VSRenderer2DData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: Render Capabilities

		CCRef<VSVertexArray> QuadVertexArray;
		CCRef<VSVertexBuffer> QuadVertexBuffer;
		CCRef<VSShader> QuadShader;
		CCRef<VSTexture2D> WhiteTexture;

		CCRef<VSVertexArray> CircleVertexArray;
		CCRef<VSVertexBuffer> CircleVertexBuffer;
		CCRef<VSShader> CircleShader;

		CCRef<VSVertexArray> LineVertexArray;
		CCRef<VSVertexBuffer> LineVertexBuffer;
		CCRef<VSShader> LineShader;

		CCRef<VSVertexArray> TextVertexArray;
		CCRef<VSVertexBuffer> TextVertexBuffer;
		CCRef<VSShader> TextShader;

		uint32_t QuadIndexCount = 0;
		VSQuadVertex* QuadVertexBufferBase = nullptr;
		VSQuadVertex* QuadVertexBufferPtr = nullptr;

		uint32_t CircleIndexCount = 0;
		VSCircleVertex* CircleVertexBufferBase = nullptr;
		VSCircleVertex* CircleVertexBufferPtr = nullptr;

		uint32_t LineVertexCount = 0;
		VSLineVertex* LineVertexBufferBase = nullptr;
		VSLineVertex* LineVertexBufferPtr = nullptr;

		uint32_t TextIndexCount = 0;
		VSTextVertex* TextVertexBufferBase = nullptr;
		VSTextVertex* TextVertexBufferPtr = nullptr;

		float LineWidth = 2.0f;

		std::array<CCRef<VSTexture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		CCRef<VSTexture2D> FontAtlasTexture;

		glm::vec4 QuadVertexPositions[4];

		VSRenderer2DStats Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		CCRef<VSUniformBuffer> CameraUniformBuffer;
	};
}
